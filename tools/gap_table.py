r"""Write gaps.tsv: every inter-function gap of tsh.exe's game code with its kind.

kind = nop (all 0x90), junkcall (ff 15 <IAT slot> + copy of the previous function's last bytes), other (jump tables/data).
Run with Python 3.13: python tools\gap_table.py
"""
import csv, os, sys
sys.path.insert(0, os.path.dirname(__file__))
from tshimg import EXE, Image
img = Image(EXE)
here = os.path.join(os.path.dirname(__file__), "..")
os.makedirs(os.path.join(here, "build"), exist_ok=True)
fs = sorted((int(r["address"], 16), int(r["size"])) for r in csv.DictReader(open(os.path.join(here, "config", "functions.tsv")), delimiter="\t"))
with open(os.path.join(here, "build", "gaps.tsv"), "w", newline="") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["after_function", "gap_address", "length", "kind", "iat_slot", "tail_copy_len"])
    for (a, s), (b, _) in zip(fs, fs[1:]):
        if b <= a + s or a >= 0x41eec0:
            continue
        g = img.read(a + s, b - a - s)
        if all(x == 0x90 for x in g):
            w.writerow(["%08x" % a, "%08x" % (a + s), len(g), "nop", "", ""])
        elif g[:2] == b"\xff\x15":
            slot = int.from_bytes(g[2:6], "little")
            k = len(g) - 6
            ok = k <= 0 or g[6:] == img.read(a, s)[-k:]
            w.writerow(["%08x" % a, "%08x" % (a + s), len(g), "junkcall" if ok else "junkcall-irregular", "%08x" % slot, max(k, 0)])
        else:
            w.writerow(["%08x" % a, "%08x" % (a + s), len(g), "other", "", ""])
