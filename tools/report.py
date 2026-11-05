#!/usr/bin/env python3
"""Write an objdiff-format progress report (report.json) from build/results.tsv.

One unit per source file; functions that no source file covers yet are listed in the unit `main/unstarted` at 0 %.
The C runtime (statically linked VC6 libc, 193 functions) is not part of the decompilation target and is left out.
Initialised data (build/progress_data.tsv, only when `build_all.py --data` was run) is reported as `.data` sections.
Fuzzy percentages for non-matching functions are an instruction-level similarity (see compare_obj.fuzzy_percent),
not objdiff's own algorithm.
Usage: python tools/report.py [--out build/report.json] [--version jp]
"""
import argparse
import collections
import csv
import json
import os

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
BUILD = os.path.join(ROOT, "build")


def read_tsv(path):
    if not os.path.exists(path):
        return []
    return list(csv.DictReader(open(path, encoding="utf-8"), delimiter="\t"))


def measures(code_items, data_items, units_total=0, units_complete=0):
    total_code = sum(i["size"] for i in code_items)
    matched_code = sum(i["size"] for i in code_items if i["fuzzy"] >= 100.0)
    fuzzy = sum(i["size"] * i["fuzzy"] for i in code_items) / total_code if total_code else 0.0
    total_data = sum(i["size"] for i in data_items)
    matched_data = sum(i["size"] for i in data_items if i["fuzzy"] >= 100.0)
    total_fn = len(code_items)
    matched_fn = sum(1 for i in code_items if i["fuzzy"] >= 100.0)
    complete_code = total_code if total_fn and matched_fn == total_fn else 0
    complete_data = total_data if data_items and matched_data == total_data else 0
    return {
        "fuzzy_match_percent": round(fuzzy, 4),
        "total_code": str(total_code),
        "matched_code": str(matched_code),
        "matched_code_percent": round(100.0 * matched_code / total_code, 4) if total_code else 0.0,
        "total_data": str(total_data),
        "matched_data": str(matched_data),
        "matched_data_percent": round(100.0 * matched_data / total_data, 4) if total_data else 0.0,
        "total_functions": total_fn,
        "matched_functions": matched_fn,
        "matched_functions_percent": round(100.0 * matched_fn / total_fn, 4) if total_fn else 0.0,
        "complete_code": str(complete_code),
        "complete_code_percent": round(100.0 * complete_code / total_code, 4) if total_code else 0.0,
        "complete_data": str(complete_data),
        "complete_data_percent": round(100.0 * complete_data / total_data, 4) if total_data else 0.0,
        "total_units": units_total,
        "complete_units": units_complete,
    }


def item(name, size, fuzzy, address):
    out = {"name": name, "size": str(size), "fuzzy_match_percent": round(fuzzy, 4)}
    if address is not None:
        out["metadata"] = {"virtual_address": str(address)}
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=os.path.join(BUILD, "report.json"))
    ap.add_argument("--version", default="jp")
    args = ap.parse_args()

    functions = {r["address"]: r for r in read_tsv(os.path.join(ROOT, "config", "functions.tsv")) if r["region"] == "game"}
    best = {}
    for r in read_tsv(os.path.join(BUILD, "results.tsv")):
        key = r["address"]
        if key not in functions:
            continue
        score = (r["status"] == "match", float(r["fuzzy"]))
        if key not in best or score > best[key][0]:
            best[key] = (score, r)

    by_unit = collections.defaultdict(list)
    for address, f in functions.items():
        size = int(f["size"])
        if address in best:
            r = best[address][1]
            unit = "main/" + os.path.splitext(os.path.basename(r["source"]))[0]
            fuzzy = 100.0 if r["status"] == "match" else float(r["fuzzy"])
            name = r["symbol"]
        else:
            unit, fuzzy, name = "main/unstarted", 0.0, f["name"]
        by_unit[unit].append({"name": name, "size": size, "fuzzy": fuzzy, "address": int(address, 16)})

    data_by_unit = collections.defaultdict(list)
    for r in read_tsv(os.path.join(BUILD, "progress_data.tsv")):
        unit = "main/" + os.path.splitext(r["source_file"])[0]
        data_by_unit[unit].append({"name": r["symbol"], "size": int(r["size"]),
                                   "fuzzy": 100.0 if r["status"] in ("MATCH", "matching") else 0.0,
                                   "address": int(r["address"], 16)})

    units = []
    all_code, all_data = [], []
    complete_units = 0
    for unit in sorted(set(by_unit) | set(data_by_unit)):
        code = sorted(by_unit.get(unit, []), key=lambda i: i["address"])
        data = sorted(data_by_unit.get(unit, []), key=lambda i: i["address"])
        m = measures(code, data, 1, 0)
        complete = bool(code or data) and m["complete_code"] == m["total_code"] and m["complete_data"] == m["total_data"] \
            and (code or data) and unit != "main/unstarted"
        if complete:
            m["complete_units"] = 1
            complete_units += 1
        sections = []
        if code:
            tot = sum(i["size"] for i in code)
            sections.append({"name": ".text", "size": str(tot), "fuzzy_match_percent": m["fuzzy_match_percent"]})
        if data:
            tot = sum(i["size"] for i in data)
            sections.append({"name": ".data", "size": str(tot), "fuzzy_match_percent": m["matched_data_percent"]})
        units.append({
            "name": unit,
            "measures": m,
            "sections": sections,
            "functions": [item(i["name"], i["size"], i["fuzzy"], i["address"]) for i in code],
            "metadata": {"complete": complete, "source_path": unit.replace("main/", "src/") + ".cpp"},
        })
        all_code += code
        all_data += data

    report = {
        "measures": measures(all_code, all_data, len(units), complete_units),
        "units": units,
        "version": 2,
        "categories": [],
    }
    os.makedirs(os.path.dirname(os.path.abspath(args.out)), exist_ok=True)
    with open(args.out, "w", encoding="utf-8", newline="\n") as fh:
        json.dump(report, fh, indent=1)
    m = report["measures"]
    print("report %s (%s): %d/%d functions, %.2f%% code matched (%s of %s bytes), fuzzy %.2f%%, %d units" % (
        args.out, args.version, m["matched_functions"], m["total_functions"], m["matched_code_percent"],
        m["matched_code"], m["total_code"], m["fuzzy_match_percent"], len(units)))


if __name__ == "__main__":
    main()
