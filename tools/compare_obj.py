"""Compare functions in a COFF .obj (MSVC 7.1 output) byte for byte with re/bin/tsh.exe.

A function symbol is mapped to its original address through config/symbols*.tsv (name<TAB>address), or by
the naming convention FUN_xxxxxxxx / sub_xxxxxxxx. Data symbols follow the same rules (DAT_xxxxxxxx), imports
resolve through tsh.exe's import table (__imp__name). Relocations to resolved symbols must point at the same
address as the original; unresolved ones (string literals, static locals) are masked and counted.

Usage: python tools/compare_obj.py <file.obj> [--source src/foo.c] [--update] [--verbose]
"""
import argparse
import csv
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tshimg import EXE, ROOT, Image  # noqa: E402

import capstone  # noqa: E402

DECOMP = ROOT
CONFIG = os.path.join(ROOT, "config")
INVENTORY = os.path.join(CONFIG, "functions.tsv")
PROGRESS = os.path.join(ROOT, "build", "progress.tsv")

IMAGE_REL_I386_DIR32 = 0x06
IMAGE_REL_I386_REL32 = 0x14
ADDRESS_NAME = re.compile(r"^(?:FUN|sub|DAT|PTR|LAB|M|G)_([0-9a-fA-F]{8})$")
MANGLED_ADDRESS = re.compile(r"^\?(?:[A-Za-z]+_)?(?:FUN|sub|DAT|PTR|LAB|M|G)_([0-9a-fA-F]{8})@")
PADDING = (0xCC, 0x90)


def undecorate(name):
    if name.startswith("__imp_"):
        return name
    if name.startswith("?"):
        return name
    if name.startswith(("_", "@")):
        name = name[1:]
    return re.sub(r"@\d+$", "", name)


class CoffObject:
    def __init__(self, path):
        d = open(path, "rb").read()
        machine, nsec, _, symptr, nsym, optsize, _ = struct.unpack_from("<HHIIIHH", d, 0)
        if machine != 0x14C:
            raise SystemExit(f"{path}: not an i386 COFF object (machine {machine:#x})")
        strtab = symptr + 18 * nsym
        self.sections = []
        for i in range(nsec):
            o = 20 + optsize + 40 * i
            name = d[o:o + 8].rstrip(b"\0").decode("latin-1")
            size, raw, relptr, _, nrel, _, chars = struct.unpack_from("<IIIIHHI", d, o + 16)
            if name.startswith("/"):
                off = int(name[1:])
                name = d[strtab + off:d.index(b"\0", strtab + off)].decode("latin-1")
            relocs = []
            for r in range(nrel):
                va, sym, typ = struct.unpack_from("<IIH", d, relptr + 10 * r)
                relocs.append((va, sym, typ))
            self.sections.append({"name": name, "data": d[raw:raw + size] if raw else b"\0" * size,
                                  "relocs": relocs, "chars": chars})
        self.symbols = {}
        i = 0
        while i < nsym:
            o = symptr + 18 * i
            raw_name = d[o:o + 8]
            if raw_name[:4] == b"\0\0\0\0":
                off = struct.unpack_from("<I", raw_name, 4)[0]
                name = d[strtab + off:d.index(b"\0", strtab + off)].decode("latin-1")
            else:
                name = raw_name.rstrip(b"\0").decode("latin-1")
            value, secnum, typ, sclass, naux = struct.unpack_from("<IhHBB", d, o + 8)
            self.symbols[i] = {"name": name, "value": value, "section": secnum, "type": typ, "class": sclass}
            i += 1 + naux

    def functions(self):
        by_section = {}
        for s in self.symbols.values():
            if s["section"] > 0 and (s["type"] >> 4) == 2 and s["class"] in (2, 3):
                by_section.setdefault(s["section"], []).append(s)
        for secnum, syms in by_section.items():
            syms.sort(key=lambda s: s["value"])
            sec = self.sections[secnum - 1]
            for k, s in enumerate(syms):
                end = syms[k + 1]["value"] if k + 1 < len(syms) else len(sec["data"])
                yield s["name"], sec, s["value"], end


def import_slots(img):
    d = img.data
    pe = struct.unpack_from("<I", d, 0x3C)[0]
    rva, _ = struct.unpack_from("<II", d, pe + 24 + 96 + 8)
    slots = {}
    o = rva
    while True:
        desc = img.read(img.base + o, 20)
        oft, _, _, name_rva, ft = struct.unpack("<IIIII", desc)
        if not name_rva:
            break
        dll = img.read(img.base + name_rva, 64).split(b"\0")[0].decode().upper()
        k = 0
        while True:
            entry = struct.unpack("<I", img.read(img.base + (oft or ft) + 4 * k, 4))[0]
            if not entry:
                break
            slot = img.base + ft + 4 * k
            if entry & 0x80000000:
                slots[f"{dll}#{entry & 0xFFFF}"] = slot
            else:
                fname = img.read(img.base + entry + 2, 256).split(b"\0")[0].decode()
                slots[fname] = slot
            k += 1
        o += 20
    return slots


def load_symbol_map():
    import glob
    names = {}
    for path in sorted(glob.glob(os.path.join(CONFIG, "symbols*.tsv"))):
        for row in csv.reader(open(path, encoding="utf-8"), delimiter="	"):
            if len(row) >= 2 and not row[0].startswith("#") and row[0] != "name":
                names[row[0]] = int(row[1], 16)
    return names


def resolve(name, names, slots):
    if name in names:
        return names[name]
    base = undecorate(name)
    if base in names:
        return names[base]
    if base.startswith("__imp_"):
        target = undecorate(base[len("__imp_"):])
        if target in names:
            return names[target]
        return slots.get(target)
    m = ADDRESS_NAME.match(base) or MANGLED_ADDRESS.match(name)
    return int(m.group(1), 16) if m else None


def compare_function(img, obj, names, slots, name, sec, start, end, address):
    code = bytearray(sec["data"][start:end])
    while code and code[-1] in PADDING and len(code) > 1:
        code.pop()
    original = bytearray(img.read(address, len(code)))
    masked = 0
    reloc_errors = []
    for va, symidx, typ in sec["relocs"]:
        if not start <= va < start + len(code):
            continue
        off = va - start
        target_name = obj.symbols[symidx]["name"]
        target = resolve(target_name, names, slots)
        addend = struct.unpack_from("<i", code, off)[0]
        if target is None:
            masked += 1
            original[off:off + 4] = code[off:off + 4]
            continue
        if typ == IMAGE_REL_I386_DIR32:
            expected = (target + addend) & 0xFFFFFFFF
        elif typ == IMAGE_REL_I386_REL32:
            expected = (target + addend - (address + off + 4)) & 0xFFFFFFFF
        else:
            masked += 1
            original[off:off + 4] = code[off:off + 4]
            continue
        actual = struct.unpack_from("<I", original, off)[0]
        if actual != expected:
            reloc_errors.append(f"+{off:#x} {target_name}: original {actual:#010x}, expected {expected:#010x}")
        original[off:off + 4] = code[off:off + 4]
    first_diff = next((i for i in range(len(code)) if code[i] != original[i]), None)
    next_byte = img.read(address + len(code), 1)
    size_ok = not next_byte or next_byte[0] in PADDING or (address + len(code)) in names.values()
    return code, first_diff, reloc_errors, masked, size_ok


def show_diff(address, code, original, first_diff):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    a = [(i.address - address, f"{i.mnemonic} {i.op_str}") for i in md.disasm(bytes(code), address)]
    b = [(i.address - address, f"{i.mnemonic} {i.op_str}") for i in md.disasm(bytes(original), address)]
    lo = max(0, next((k for k, (o, _) in enumerate(a) if o >= first_diff), 0) - 3)
    for k in range(lo, min(lo + 12, max(len(a), len(b)))):
        left = f"{a[k][0]:+5x} {a[k][1]}" if k < len(a) else ""
        right = f"{b[k][0]:+5x} {b[k][1]}" if k < len(b) else ""
        print(f"    {left:<44} | {right}")


NUMBER = re.compile(r"0x[0-9a-f]{5,}")


def fuzzy_percent(code, original, address):
    """Instruction-level similarity (0-100) of two byte strings; large constants (addresses) are ignored."""
    import difflib
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)

    def lines(b):
        return [NUMBER.sub("ADDR", f"{i.mnemonic} {i.op_str}") for i in md.disasm(bytes(b), address)]
    a, b = lines(code), lines(original)
    if not a and not b:
        return 100.0
    return 100.0 * difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


def update_progress(results, source, path):
    rows = list(csv.DictReader(open(path, encoding="utf-8"), delimiter="\t"))
    by_addr = {r["address"]: r for r in rows}
    for address, ok in results:
        r = by_addr.get(f"{address:08x}")
        if r:
            if r["status"] == "matching" and not ok:
                continue
            r["status"] = "matching" if ok else "non-matching"
            if source:
                r["source_file"] = source
    with open(path, "w", encoding="utf-8", newline="") as fh:
        w = csv.DictWriter(fh, fieldnames=list(rows[0]), delimiter="\t", lineterminator="\n")
        w.writeheader()
        w.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("obj")
    ap.add_argument("--source", help="source file recorded in progress.tsv")
    ap.add_argument("--update", action="store_true", help="write matching/non-matching into progress.tsv")
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("--progress", default=PROGRESS, help="progress file to update")
    ap.add_argument("--results", help="append one row per function (address, symbol, sizes, status, fuzzy, source) to this TSV")
    args = ap.parse_args()

    img = Image(EXE)
    obj = CoffObject(args.obj)
    names = load_symbol_map()
    slots = import_slots(img)
    inventory = {int(r["address"], 16): int(r["size"])
                 for r in csv.DictReader(open(INVENTORY, encoding="utf-8"), delimiter="\t")}

    results = []
    for name, sec, start, end in obj.functions():
        address = resolve(name, names, slots)
        if address is None:
            print(f"skip  {name}: no address (add it to symbols.tsv)")
            continue
        code, first_diff, reloc_errors, masked, size_ok = compare_function(
            img, obj, names, slots, name, sec, start, end, address)
        size_ok = size_ok or (address + len(code)) in inventory or inventory.get(address) == len(code)
        expected_size = inventory.get(address)
        size_note = "" if expected_size in (None, len(code)) else f", size {len(code)} vs inventory {expected_size}"
        ok = first_diff is None and not reloc_errors and size_ok
        status = "MATCH" if ok else "DIFF "
        detail = f"{len(code)} bytes, {masked} masked relocs{size_note}"
        if first_diff is not None:
            detail += f", first byte diff at +{first_diff:#x}"
        print(f"{status} {address:08x} {name}: {detail}")
        for e in reloc_errors:
            print(f"    reloc {e}")
        if first_diff is not None and args.verbose:
            show_diff(address, code, img.read(address, len(code)), first_diff)
        results.append((address, ok))
        if args.results:
            original_all = img.read(address, expected_size or len(code))
            fuzzy = 100.0 if ok else fuzzy_percent(code, original_all, address)
            os.makedirs(os.path.dirname(os.path.abspath(args.results)), exist_ok=True)
            new = not os.path.exists(args.results)
            with open(args.results, "a", encoding="utf-8", newline="") as fh:
                w = csv.writer(fh, delimiter=chr(9), lineterminator=chr(10))
                if new:
                    w.writerow(["address", "symbol", "obj_size", "orig_size", "status", "fuzzy", "source"])
                w.writerow([f"{address:08x}", name, len(code), expected_size or len(code), "match" if ok else "diff",
                            f"{fuzzy:.2f}", args.source or ""])

    if args.update and results:
        update_progress(results, args.source, args.progress)
    return 0 if results and all(ok for _, ok in results) else 1


if __name__ == "__main__":
    sys.exit(main())
