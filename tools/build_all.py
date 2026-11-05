#!/usr/bin/env python3
"""Build and check the whole decompilation.

  python tools/build_all.py                 compile every src/*.cpp with VC6 and compare each function with orig/tsh.exe
  python tools/build_all.py --file A_game.cpp [B_misc.cpp ...]
  python tools/build_all.py --data          also generate and check the initialised data (src/K_*.cpp)
  python tools/build_all.py --report build/report.json --version jp

Needs: Visual C++ 6.0 (cl 12.00.8168) in VC6_DIR (default C:\\Program Files (x86)\\Microsoft Visual Studio),
the original tsh.exe in orig/ (or TSH_EXE), and `pip install -r requirements.txt` (capstone).
Outputs (all under build/): obj/*.obj, progress.tsv, results.tsv, progress_data.tsv, report.json.
"""
import argparse
import csv
import hashlib
import os
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
TOOLS = os.path.join(ROOT, "tools")
BUILD = os.path.join(ROOT, "build")
EXPECTED_SHA1 = "185fec0816def349de984d6bdb402753004dfc1e"
DEFAULT_FLAGS = ["/nologo", "/c", "/O2", "/D", "WIN32", "/D", "NDEBUG", "/D", "_WINDOWS"]


def vc6_env():
    vc = os.environ.get("VC6_DIR", r"C:\Program Files (x86)\Microsoft Visual Studio")
    cl = os.path.join(vc, "VC98", "Bin", "CL.EXE")
    if not os.path.exists(cl):
        sys.exit("VC6 not found: %s (set VC6_DIR to the folder that contains VC98\\)" % cl)
    env = dict(os.environ)
    env["PATH"] = os.pathsep.join([os.path.join(vc, "VC98", "Bin"), os.path.join(vc, "Common", "MSDev98", "Bin"), env["PATH"]])
    env["INCLUDE"] = os.pathsep.join([os.path.join(ROOT, "include"), os.path.join(vc, "VC98", "Include")])
    env["LIB"] = os.path.join(vc, "VC98", "Lib")
    return env


def check_original():
    exe = os.environ.get("TSH_EXE") or os.path.join(ROOT, "orig", "tsh.exe")
    if not os.path.exists(exe):
        sys.exit("original executable not found: %s (see orig/README.md)" % exe)
    sha1 = hashlib.sha1(open(exe, "rb").read()).hexdigest()
    if sha1 != EXPECTED_SHA1:
        print("WARNING: %s has SHA-1 %s, expected %s; results are only meaningful for the original build" % (exe, sha1, EXPECTED_SHA1))
    return exe


def py(*args, **kw):
    return subprocess.run([sys.executable] + list(args), cwd=ROOT, **kw)


def per_file_flags():
    flags = {}
    path = os.path.join(ROOT, "config", "flags.tsv")
    if os.path.exists(path):
        for line in open(path, encoding="utf-8"):
            line = line.strip()
            if line and not line.startswith("#"):
                name, f = line.split("\t", 1)
                flags[name] = f.split()
    return flags


def fresh_progress():
    rows = list(csv.DictReader(open(os.path.join(ROOT, "config", "functions.tsv"), encoding="utf-8"), delimiter="\t"))
    path = os.path.join(BUILD, "progress.tsv")
    with open(path, "w", encoding="utf-8", newline="") as fh:
        w = csv.writer(fh, delimiter="\t", lineterminator="\n")
        w.writerow(["address", "name", "size", "region", "status", "source_file"])
        for r in rows:
            w.writerow([r["address"], r["name"], r["size"], r["region"], "todo", ""])
    return path


def compile_one(env, src, flags, quiet):
    obj_dir = os.path.join(BUILD, "obj")
    os.makedirs(obj_dir, exist_ok=True)
    cmd = ["cl"] + DEFAULT_FLAGS + flags + ["/Fo" + obj_dir + os.sep, os.path.join(ROOT, "src", src)]
    r = subprocess.run(cmd, cwd=ROOT, env=env, capture_output=True, text=True)
    if r.returncode != 0:
        print("COMPILE FAILED:", src)
        print(r.stdout[-3000:], r.stderr[-1000:])
        return None
    return os.path.join(obj_dir, os.path.splitext(src)[0] + ".obj")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--file", nargs="*", help="only these src files (names or paths)")
    ap.add_argument("--data", action="store_true", help="generate src/K_*.cpp from the original exe and check the data too")
    ap.add_argument("--report", help="write an objdiff-format report to this path")
    ap.add_argument("--version", default="jp", help="version name recorded in the report")
    ap.add_argument("--verbose", action="store_true")
    args = ap.parse_args()

    exe = check_original()
    env = vc6_env()
    env["TSH_EXE"] = exe
    os.makedirs(BUILD, exist_ok=True)

    if args.data:
        print("== generating data sources from", exe)
        if py(os.path.join("tools", "data", "gen_data.py"), env=env).returncode != 0:
            sys.exit("data generation failed")
    if py(os.path.join("tools", "gen_game_h.py")).returncode != 0:
        sys.exit("gen_game_h failed")

    sources = sorted(f for f in os.listdir(os.path.join(ROOT, "src")) if f.endswith(".cpp"))
    code_sources = [f for f in sources if not f.startswith("K_")]
    data_sources = [f for f in sources if f.startswith("K_")]
    if args.file:
        wanted = {os.path.basename(f) for f in args.file}
        code_sources = [f for f in code_sources if f in wanted]
        data_sources = [f for f in data_sources if f in wanted]

    flags = per_file_flags()
    progress = os.path.join(BUILD, "progress.tsv")
    results = os.path.join(BUILD, "results.tsv")
    if not args.file:
        fresh_progress()
        for p in (results, os.path.join(BUILD, "progress_data.tsv")):
            if os.path.exists(p):
                os.remove(p)
    elif not os.path.exists(progress):
        fresh_progress()

    failed = []
    for src in code_sources:
        obj = compile_one(env, src, flags.get(src, []), not args.verbose)
        if obj is None:
            failed.append(src)
            continue
        cmd = [os.path.join("tools", "compare_obj.py"), obj, "--source", "src/" + src, "--progress", progress,
               "--results", results, "--update"]
        if args.verbose:
            cmd.append("--verbose")
        r = py(*cmd, env=env, capture_output=True, text=True)
        lines = [l for l in r.stdout.splitlines() if l.startswith(("MATCH", "DIFF", "skip"))]
        print("%-22s %3d match, %3d diff" % (src, sum(l.startswith("MATCH") for l in lines), sum(l.startswith("DIFF") for l in lines)))
        if args.verbose:
            print(r.stdout)

    for src in data_sources:
        obj = compile_one(env, src, flags.get(src, []), True)
        if obj is None:
            failed.append(src)
            continue
        r = py(os.path.join("tools", "compare_data.py"), obj, "--source", src, "--quiet", env=env, capture_output=True, text=True)
        print("%-22s %s" % (src, (r.stdout.strip().splitlines() or ["(no output)"])[-1]))

    py(os.path.join("tools", "progress.py"))
    if args.report:
        py(os.path.join("tools", "report.py"), "--out", args.report, "--version", args.version)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
