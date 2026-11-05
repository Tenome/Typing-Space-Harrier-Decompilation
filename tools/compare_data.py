"""Compare initialised data symbols in a VC6 COFF .obj with the data in orig/tsh.exe.

Every data symbol (DAT_xxxxxxxx, mangled ?DAT_xxxxxxxx@..., or a name listed in symbols*.tsv) defined in a data
section (.data/.rdata) of the object is placed at its original address and compared byte for byte. Its extent is
the distance to the next symbol of the same section (string literals $SG... count as symbols).
Relocations inside a symbol:
  * to a resolvable symbol (DAT_/FUN_/M_ names, symbols*.tsv, imports): must equal the original pointer value;
  * to a string literal of this object: the original pointer must lead to the same bytes (string and NUL);
  * to another unresolvable symbol: masked and counted.
Writes a progress file (symbol, address, size, status) and prints MATCH/DIFF per symbol plus byte totals.

Usage: python compare_data.py <file.obj> [--progress progress_K.tsv] [--verbose] [--all]
"""
import argparse
import csv
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tshimg import EXE, Image  # noqa: E402
from compare_obj import (CoffObject, DECOMP, IMAGE_REL_I386_DIR32, IMAGE_REL_I386_REL32,  # noqa: E402
                         import_slots, load_symbol_map, resolve)

DATA_SECTIONS = (".data", ".rdata")
STRING_NAME = re.compile(r"^\?\?_C@_|^\$SG")


def section_symbols(obj):
    by_section = {}
    for idx, s in obj.symbols.items():
        if s["section"] > 0 and (s["type"] >> 4) != 2 and s["class"] in (2, 3):
            if obj.sections[s["section"] - 1]["name"] in DATA_SECTIONS:
                by_section.setdefault(s["section"], []).append((idx, s))
    for syms in by_section.values():
        syms.sort(key=lambda p: p[1]["value"])
    return by_section


def c_string_at(data, offset):
    end = data.find(b"\0", offset)
    return None if end < 0 else data[offset:end + 1]


def compare_symbol(img, obj, names, slots, sec, sym_idx, start, end, address, strings_seen):
    local = bytearray(sec["data"][start:end])
    original = bytearray(img.read(address, len(local)))
    if len(original) < len(local):
        original += bytes(len(local) - len(original))
    notes = []
    masked = 0
    string_ok = 0
    reloc_bytes = set()
    for va, symidx, typ in sec["relocs"]:
        if not start <= va < end:
            continue
        off = va - start
        reloc_bytes.update(range(off, off + 4))
        target = obj.symbols[symidx]
        tname = target["name"]
        addend = struct.unpack_from("<i", local, off)[0]
        if typ != IMAGE_REL_I386_DIR32:
            masked += 1
            original[off:off + 4] = local[off:off + 4]
            continue
        actual = struct.unpack_from("<I", original, off)[0]
        address_of_target = resolve(tname, names, slots)
        if address_of_target is not None:
            expected = (address_of_target + addend) & 0xFFFFFFFF
            if actual != expected:
                notes.append(f"+{off:#x} {tname}: original {actual:#010x}, expected {expected:#010x}")
        elif target["section"] > 0 and obj.sections[target["section"] - 1]["name"] in DATA_SECTIONS:
            tsec = obj.sections[target["section"] - 1]
            literal = c_string_at(tsec["data"], target["value"] + addend)
            looks_like_string = STRING_NAME.match(tname) or (literal is not None and tname.startswith("?"))
            if looks_like_string and literal is not None:
                seen = strings_seen.get((tname, addend))
                if seen is not None and seen != actual:
                    notes.append(f"+{off:#x} {tname}: literal used at {seen:#010x} and {actual:#010x}")
                strings_seen[(tname, addend)] = actual
                if img.read(actual, len(literal)) == literal:
                    string_ok += 1
                else:
                    notes.append(f"+{off:#x} string {literal[:24]!r}: original pointer {actual:#010x} leads to "
                                 f"{img.read(actual, len(literal))[:24]!r}")
            else:
                masked += 1
        else:
            masked += 1
        original[off:off + 4] = local[off:off + 4]
    first_diff = next((i for i in range(len(local)) if local[i] != original[i]), None)
    if first_diff is not None:
        # zero bytes at the very end of ours may be the alignment gap before the next object of this object file
        used = len(local)
        while used > 0 and local[used - 1] == 0 and used - 1 not in reloc_bytes and len(local) - used < 8:
            used -= 1
        used = (used + 3) & ~3
        if first_diff >= used:
            del local[used:]
            del original[used:]
            first_diff = None
    return len(local), first_diff, notes, masked, string_ok


def update_progress(path, rows):
    with open(path, "w", encoding="utf-8", newline="") as fh:
        w = csv.writer(fh, delimiter="\t", lineterminator="\n")
        w.writerow(["symbol", "address", "size", "status", "source_file"])
        for r in sorted(rows, key=lambda r: r[1]):
            w.writerow([r[0], f"{r[1]:08x}", r[2], r[3], r[4]])


def load_progress(path):
    if not os.path.exists(path):
        return {}
    return {r["symbol"]: r for r in csv.DictReader(open(path, encoding="utf-8"), delimiter="\t")}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("obj", nargs="+")
    ap.add_argument("--progress", default=os.path.join(DECOMP, "build", "progress_data.tsv"))
    ap.add_argument("--source", default="")
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("--quiet", action="store_true", help="print only DIFF lines and totals")
    args = ap.parse_args()

    img = Image(EXE)
    names = load_symbol_map()
    slots = import_slots(img)
    old = load_progress(args.progress)
    rows = {k: (k, int(v["address"], 16), int(v["size"]), v["status"], v["source_file"]) for k, v in old.items()}

    total = matched = 0
    for path in args.obj:
        obj = CoffObject(path)
        source = args.source or os.path.splitext(os.path.basename(path))[0] + ".cpp"
        strings_seen = {}
        for secnum, syms in section_symbols(obj).items():
            sec = obj.sections[secnum - 1]
            for k, (idx, s) in enumerate(syms):
                if STRING_NAME.match(s["name"]):
                    continue
                address = resolve(s["name"], names, slots)
                if address is None:
                    continue
                end = syms[k + 1][1]["value"] if k + 1 < len(syms) else len(sec["data"])
                size, first_diff, notes, masked, string_ok = compare_symbol(
                    img, obj, names, slots, sec, idx, s["value"], end, address, strings_seen)
                ok = first_diff is None and not notes
                status = "MATCH" if ok else "DIFF"
                total += size
                matched += size if ok else 0
                detail = f"{size} bytes, {string_ok} strings"
                if masked:
                    detail += f", {masked} masked relocs"
                if first_diff is not None:
                    detail += f", first byte diff at +{first_diff:#x}"
                if not (args.quiet and ok):
                    print(f"{status:5} {address:08x} {s['name']}: {detail}")
                for n in notes[:6]:
                    print(f"    {n}")
                if first_diff is not None and args.verbose:
                    a = sec["data"][s["value"] + first_diff:s["value"] + first_diff + 16].hex(" ")
                    b = img.read(address + first_diff, 16).hex(" ")
                    print(f"    ours     {a}\n    original {b}")
                rows[s["name"]] = (s["name"], address, size, status, source)
    update_progress(args.progress, list(rows.values()))
    print(f"data bytes matched {matched} / {total} in this run; "
          f"progress file totals {sum(r[2] for r in rows.values() if r[3] == 'MATCH')} / "
          f"{sum(r[2] for r in rows.values())}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
