#!/usr/bin/env python3
"""Print code progress from build/progress.tsv (written by tools/build_all.py)."""
import collections
import csv
import os

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
path = os.path.join(ROOT, "build", "progress.tsv")
rows = list(csv.DictReader(open(path, encoding="utf-8"), delimiter="\t"))
count = collections.Counter((r["region"], r["status"]) for r in rows)
size = collections.Counter()
for r in rows:
    size[(r["region"], r["status"])] += int(r["size"])
for region, label in (("game", "game code"), ("crt", "C runtime (libc.lib, not decompiled)")):
    total = sum(v for (g, _), v in count.items() if g == region)
    done = count[(region, "matching")]
    print("%-38s %3d/%3d functions matching (%.1f%%)  %6d/%6d bytes  non-matching %3d  not started %3d" % (
        label, done, total, 100.0 * done / max(total, 1), size[(region, "matching")],
        sum(v for (g, _), v in size.items() if g == region), count[(region, "non-matching")], count[(region, "todo")]))
