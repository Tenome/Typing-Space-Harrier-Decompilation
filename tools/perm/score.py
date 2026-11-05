"""Compile one source text with VC6 and score the target function against tsh.exe (0 = byte match)."""
import difflib
import hashlib
import os
import re
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
DECOMP = os.path.abspath(os.path.join(HERE, "..", ".."))
sys.path.insert(0, os.path.join(DECOMP, "tools"))
import capstone  # noqa: E402
import compare_obj as C  # noqa: E402
from tshimg import EXE, Image  # noqa: E402

VC = os.environ.get("VC6_DIR", r"C:\Program Files (x86)\Microsoft Visual Studio")
_state = {}
ABS_RE = re.compile(r"0x[0-9a-f]+")


def _init():
    if _state:
        return
    img = Image(EXE)
    _state["img"] = img
    _state["names"] = C.load_symbol_map()
    _state["slots"] = C.import_slots(img)
    _state["md"] = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    env = dict(os.environ)
    env["PATH"] = VC + r"\VC98\Bin;" + VC + r"\Common\MSDev98\Bin;" + env["PATH"]
    env["INCLUDE"] = os.path.join(DECOMP, "include") + ";" + VC + r"\VC98\Include"
    env["LIB"] = VC + r"\VC98\Lib"
    _state["env"] = env


def _patch_relocs(buf, sec, start, n, addr):
    for va, symidx, typ in sec["relocs"]:
        if start <= va < start + n:
            off = va - start
            if typ == C.IMAGE_REL_I386_REL32:
                v = (0x500000 - (addr + off + 4)) & 0xFFFFFFFF
            else:
                v = 0x500000
            buf[off:off + 4] = struct.pack("<I", v)


def instr_list(code, base, md):
    ins = list(md.disasm(bytes(code), base))
    consumed = sum(i.size for i in ins)
    idx = {i.address: n for n, i in enumerate(ins)}
    out = []
    jt = {}
    for n, i in enumerate(ins):
        op = i.op_str
        if i.mnemonic.startswith("j") or i.mnemonic == "loop":
            if op.startswith("0x"):
                t = int(op, 16)
                jt[n] = idx.get(t, -1)
                op = "@"
        op = ABS_RE.sub(lambda m: "ABS" if 0x400000 <= int(m.group(0), 16) < 0x2000000 else m.group(0), op)
        out.append("%s %s" % (i.mnemonic, op))
    rest = bytes(code[consumed:])
    for k in range(0, len(rest), 4):
        w = rest[k:k + 4]
        v = struct.unpack("<I", w)[0] if len(w) == 4 else -1
        out.append("data ABS" if 0x400000 <= v < 0x2000000 else "data " + w.hex())
    return out, jt


def aligned_diff(a, ja, b, jb):
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    amap = {}
    diff = 0
    ops = sm.get_opcodes()
    for tag, i1, i2, j1, j2 in ops:
        if tag == "equal":
            for k in range(i2 - i1):
                amap[i1 + k] = j1 + k
        else:
            diff += max(i2 - i1, j2 - j1)
    for ia, t in ja.items():
        if ia in amap and amap[ia] in jb:
            tb = jb[amap[ia]]
            if t < 0 or tb < 0 or amap.get(t, -2) != tb:
                diff += 1
    return diff, ops


def score_obj(objpath, target):
    _init()
    img, names, slots, md = _state["img"], _state["names"], _state["slots"], _state["md"]
    obj = C.CoffObject(objpath)
    for name, sec, start, end in obj.functions():
        addr = C.resolve(name, names, slots)
        if addr != target:
            continue
        code, first_diff, reloc_errors, masked, size_ok = C.compare_function(img, obj, names, slots, name, sec, start, end, addr)
        original = bytearray(img.read(addr, len(code)))
        # original has relocs forced equal inside compare_function only on its copy; rebuild for text diff
        orig_full = img.read(addr, len(code) + 0)
        inv_size = _inventory_size(addr)
        if inv_size:
            orig_full = img.read(addr, inv_size)
        mine = bytes(code)
        # patch relocation fields so identical targets compare equal
        mine_p = bytearray(mine)
        orig_p = bytearray(orig_full)
        _patch_relocs(mine_p, sec, start, len(code), addr)
        a, ja = instr_list(mine_p, addr, md)
        b, jb = instr_list(orig_p, addr, md)
        diff, _ = aligned_diff(a, ja, b, jb)
        if diff == 0:
            diff = len(reloc_errors)
        size_ok = size_ok or (addr + len(code)) in names.values() or inv_size == len(code)
        exact = first_diff is None and not reloc_errors and size_ok
        if exact:
            diff = 0
        elif diff == 0:
            diff = 1
        size_delta = abs(len(code) - (inv_size or len(code)))
        h = hashlib.md5(mine).hexdigest()
        return {"score": diff, "size": len(code), "size_delta": size_delta, "hash": h, "exact": exact,
                "first_diff": first_diff, "ninstr": len(a)}
    return None


_inv = {}


def _inventory_size(addr):
    if not _inv:
        import csv
        for r in csv.DictReader(open(C.INVENTORY, encoding="utf-8"), delimiter="\t"):
            _inv[int(r["address"], 16)] = int(r["size"])
    return _inv.get(addr)


def compile_and_score(source, target, workdir, flags=(), tag="v"):
    _init()
    os.makedirs(workdir, exist_ok=True)
    src = os.path.join(workdir, tag + ".cpp")
    obj = os.path.join(workdir, tag + ".obj")
    with open(src, "w", newline="\n") as f:
        f.write(source)
    if os.path.exists(obj):
        os.remove(obj)
    cmd = ["cl", "/nologo", "/c", "/O2", "/D", "WIN32", "/D", "NDEBUG", "/D", "_WINDOWS", "/Fo" + obj] + list(flags) + [src]
    p = subprocess.run(cmd, env=_state["env"], capture_output=True, text=True, cwd=workdir)
    if p.returncode != 0 or not os.path.exists(obj):
        return {"error": (p.stdout + p.stderr)[-600:]}
    r = score_obj(obj, target)
    if r is None:
        return {"error": "target function not in obj"}
    return r


def show_diff(objpath, target, limit=60):
    _init()
    img, names, slots, md = _state["img"], _state["names"], _state["slots"], _state["md"]
    obj = C.CoffObject(objpath)
    for name, sec, start, end in obj.functions():
        addr = C.resolve(name, names, slots)
        if addr != target:
            continue
        code = bytearray(sec["data"][start:end])
        while len(code) > 1 and code[-1] in (0x90, 0xCC):
            code.pop()
        inv = _inventory_size(addr) or len(code)
        orig = bytearray(img.read(addr, inv))
        _patch_relocs(code, sec, start, len(code), addr)
        a, ja = instr_list(code, addr, md)
        b, jb = instr_list(orig, addr, md)
        d, ops = aligned_diff(a, ja, b, jb)
        print("score", d)
        n = 0
        for tag, i1, i2, j1, j2 in ops:
            if tag == "equal":
                continue
            print("--- %s mine[%d:%d] orig[%d:%d]" % (tag, i1, i2, j1, j2))
            for x in a[i1:i2]:
                print("  <", x)
            for x in b[j1:j2]:
                print("  >", x)
            n += 1
            if n > limit:
                break
