"""Lane K: emit src/K_*.cpp (typed initialised data) from the data in orig/tsh.exe.

The generator only decides *types and extents* (SEGMENTS below, plus the automatic splitter for pointer/int runs);
the values come from the image. Run:  python tools/data/gen_data.py
"""
import glob
import os
import re
import struct
import sys

BS = chr(92)
HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from tshimg import EXE, Image  # noqa: E402

im = Image(EXE)
STRING_POOL = (0x4E2980, 0x4EEE49)
TEXT = (0x401000, 0x426B2D)


def u32(a):
    return struct.unpack("<I", im.read(a, 4))[0]


def s32(a):
    return struct.unpack("<i", im.read(a, 4))[0]


def u16(a):
    return struct.unpack("<H", im.read(a, 2))[0]


def s16(a):
    return struct.unpack("<h", im.read(a, 2))[0]


def cstring(a):
    d = im.read(a, 600)
    return d[:d.index(b"\0")] if b"\0" in d else d


def c_literal(raw):
    out = []
    for b in raw:
        if b == 0x22:
            out.append(BS + '"')
        elif b == 0x5C:
            out.append(BS + BS)
        elif b == 0x3F:
            out.append(BS + "?")
        elif 0x20 <= b < 0x7F:
            out.append(chr(b))
        elif b == 0x0A:
            out.append(BS + "n")
        elif b == 0x0D:
            out.append(BS + "r")
        elif b == 0x09:
            out.append(BS + "t")
        else:
            out.append(BS + "%03o" % b)
    return '"' + "".join(out) + '"'


def decoded_comment(raw):
    if all(b < 0x80 for b in raw):
        return ""
    return " /* " + raw.decode("cp932", "replace").replace("*/", "* /") + " */"


def printable_string(raw):
    if not raw:
        return False
    i = 0
    while i < len(raw):
        b = raw[i]
        if 0x20 <= b < 0x7F or b in (0x0A, 0x0D, 0x09):
            i += 1
        elif (0x81 <= b <= 0x9F or 0xE0 <= b <= 0xFC) and i + 1 < len(raw) and raw[i + 1] >= 0x40:
            i += 2
        elif 0xA1 <= b <= 0xDF:
            i += 1
        else:
            return False
    return True


def load_prelude_decls():
    decls = {}
    pat = re.compile(r"^\s*extern\s+(?P<type>[A-Za-z_][\w ]*?[\s\*]+)(?P<name>DAT_(?P<addr>[0-9a-f]{8}))\s*(?P<arr>\[[^\]]*\])?\s*;")
    for path in sorted(glob.glob(os.path.join(ROOT, "config", "game_prelude_*.h"))):
        if path.endswith("_K.h"):
            continue
        for line in open(path, encoding="utf-8", errors="replace"):
            m = pat.match(line)
            if m:
                a = int(m.group("addr"), 16)
                t = re.sub(r"\s+", " ", m.group("type")).strip()
                t = t.replace(" *", " *")
                arr = m.group("arr")
                decls.setdefault(a, (t, arr, os.path.basename(path)))
    return decls


DECLS = load_prelude_decls()
WIDTH = {"int": 4, "unsigned int": 4, "short": 2, "unsigned short": 2, "char": 1, "unsigned char": 1}
PTR = ("char *", "const char *", "const unsigned char *")


class Seg:
    def __init__(self, addr, end, kind, ctype, name=None, fields=None, count=None, rows=1):
        self.addr, self.end, self.kind, self.ctype = addr, end, kind, ctype
        self.name = name or "DAT_%08x" % addr
        self.fields = fields
        self.count = count


SEGS = []
NAMES = {}


def add(addr, end, kind, ctype, **kw):
    s = Seg(addr, end, kind, ctype, **kw)
    SEGS.append(s)
    NAMES[addr] = s
    return s


def pointer_expr(v):
    if v == 0:
        return "0"
    if v in NAMES:
        return NAMES[v].name
    for a, s in NAMES.items():
        if s.addr < v < s.end and s.kind == "ints" and (v - s.addr) % WIDTH[s.ctype] == 0:
            return "&%s[%d]" % (s.name, (v - s.addr) // WIDTH[s.ctype])
    if STRING_POOL[0] <= v < STRING_POOL[1]:
        raw = cstring(v)
        if printable_string(raw):
            return c_literal(raw)
    if v in DECLS:
        return "DAT_%08x" % v
    raise ValueError("pointer %#x has no expression" % v)


def words_text(vals, per_line=8, indent="    "):
    lines = []
    for i in range(0, len(vals), per_line):
        lines.append(indent + ", ".join(vals[i:i + per_line]))
    return ",\n".join(lines)


def is_scalar(seg):
    d = DECLS.get(seg.addr)
    return d is not None and d[1] is None


def render(seg):
    if is_scalar(seg) and seg.kind in ("ints", "ptrs"):
        return render_scalar(seg)
    width = WIDTH.get(seg.ctype)
    n = (seg.end - seg.addr)
    if seg.kind == "ints":
        count = n // width
        reader = {4: s32, 2: s16, 1: lambda a: im.read(a, 1)[0]}[width]
        if seg.ctype.startswith("unsigned"):
            reader = {4: u32, 2: u16, 1: lambda a: im.read(a, 1)[0]}[width]
        vals = [str(reader(seg.addr + i * width)) for i in range(count)]
        vals = [v if not v.startswith("-2147483648") else "(-2147483647 - 1)" for v in vals]
        arr = "[]"
        return "%s %s%s = {\n%s\n};\n" % (seg.ctype, seg.name, arr, words_text(vals))
    if seg.kind == "ptrs":
        count = n // 4
        cells = []
        for i in range(count):
            v = u32(seg.addr + 4 * i)
            cell = pointer_expr(v) + ("," if i + 1 < count else "")
            if cell.startswith('"'):
                cell += decoded_comment(cstring(v))
            cells.append("    " + cell)
        return "%s%s%s[] = {\n%s\n};\n" % (seg.ctype, "" if seg.ctype.endswith("*") else " ", seg.name,
                                           "\n".join(cells))
    if seg.kind == "struct":
        size = sum(4 for _ in seg.fields)
        count = n // size
        rows = []
        for r in range(count):
            vals = []
            for k, f in enumerate(seg.fields):
                a = seg.addr + r * size + 4 * k
                vals.append(pointer_expr(u32(a)) if f == "p" else str(s32(a)))
            rows.append("    { " + ", ".join(vals) + " }")
        return "%s %s[] = {\n%s\n};\n" % (seg.ctype, seg.name, ",\n".join(rows))
    if seg.kind == "custom":
        return seg.fields(seg)
    raise ValueError(seg.kind)


def render_scalar(seg):
    if seg.kind == "ptrs":
        v = u32(seg.addr)
        cell = pointer_expr(v)
        return "%s%s%s = %s;%s\n" % (seg.ctype, "" if seg.ctype.endswith("*") else " ", seg.name, cell,
                                     decoded_comment(cstring(v)) if cell.startswith('"') else "")
    width = WIDTH[seg.ctype]
    v = s32(seg.addr) if width == 4 else s16(seg.addr)
    return "%s %s = %d;\n" % (seg.ctype, seg.name, v)


PARKED = []


def park(seg, reason, ctype=None):
    if ctype:
        seg.ctype = ctype
    PARKED.append((seg, reason))


def main():
    exec(open(os.path.join(HERE, "data_spec.py"), encoding="utf-8").read(), globals())
    outputs = {}
    parked = {id(s) for s, _ in PARKED}
    for fname, header, segs in FILES:
        text = ['#include "game.h"', ""]
        if header:
            text.insert(1, header)
        for s in segs:
            if id(s) not in parked:
                text.append(render(s))
        outputs[fname] = "\n".join(text)
    if PARKED:
        text = ["/* Data that cannot be defined next to the other lanes' declarations yet. Each entry names the conflict.",
                "   Compiled without game.h so the objects exist; the mangled names differ from the externs until",
                "   the other lane changes its declaration. */", ""]
        for s, reason in PARKED:
            text.append("// OPEN: " + reason)
            text.append(render(s))
        outputs["K_open.cpp"] = "\n".join(text)
    for fname, text in outputs.items():
        path = os.path.join(ROOT, "src", fname)
        with open(path, "w", encoding="utf-8", newline="\r\n") as fh:
            fh.write(text)
        print("wrote", path, len(text))


def classify_word(v):
    if v == 0:
        return "Z"
    if STRING_POOL[0] <= v < STRING_POOL[1] and im.read(v - 1, 1) == b"\0" and printable_string(cstring(v)):
        return "P"
    if v in DECLS and DECLS[v][0] in ("char", "char *"):
        return "P"
    if v in NAMES:
        return "P"
    return "I"


def auto_segments(lo, hi, skip=()):
    """Split [lo, hi) at declared objects and where pointer runs change to integer runs."""
    cuts = set()
    for a, (t, arr, _) in DECLS.items():
        if lo <= a < hi:
            cuts.add(a)
            if arr is None and t in WIDTH:
                cuts.add(a + WIDTH[t])
            elif arr is None and t.endswith("*"):
                cuts.add(a + 4)
            elif arr and arr != "[]" and t in WIDTH:
                cuts.add(a + WIDTH[t] * int(arr[1:-1], 0))
            elif arr and arr != "[]" and t.endswith("*"):
                cuts.add(a + 4 * int(arr[1:-1], 0))
    cuts.add(hi)
    kinds = {}
    a = lo
    while a < hi:
        kinds[a] = classify_word(u32(a))
        a += 4
    segs = []
    start = lo
    cur = None
    pending_z = lo
    a = lo
    while a < hi:
        k = kinds[a]
        if a in cuts and a != start:
            segs.append((start, a))
            start = a
            cur = None
        if k != "Z":
            if cur is None:
                cur = k
            elif k != cur:
                # type change: cut where the new run begins (zeros stay with the previous run)
                segs.append((start, a))
                start = a
                cur = k
        a += 4
    segs.append((start, hi))
    out = []
    for s, e in segs:
        if (s, e) in skip or e <= s:
            continue
        has_p = any(kinds[x] == "P" for x in range(s, e, 4))
        decl = DECLS.get(s)
        if has_p:
            ctype = decl[0] if decl and decl[0].endswith("*") else "char *"
            out.append(add(s, e, "ptrs", ctype))
        else:
            ctype = decl[0] if decl and decl[0] in WIDTH else "int"
            out.append(add(s, e, "ints", ctype))
    return out


if __name__ == "__main__":
    main()
