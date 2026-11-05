"""enum_open.py: baseline score of every function in the *_open.cpp files (and E_app OPEN-marked). Writes build/M/open_scores.tsv"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fnsrc, score, cparse as P
from concurrent.futures import ProcessPoolExecutor
SRC = os.path.join(fnsrc.DECOMP, "src")
W = os.path.join(fnsrc.DECOMP, "build", "M", "scratch")
def job(a):
    addr, fn = a
    try:
        flags = ["/GX"] if fn in ("E_app.cpp", "H_list.cpp") else []
        t = fnsrc.Target(fn, addr)
        r = score.compile_and_score(t.raw_source(), int(addr, 16), os.path.join(W, "e%d" % os.getpid()), flags, "b")
        return addr, fn, r.get("score"), r.get("size"), (r.get("error") or "")[-80:].replace("\n", " ")
    except BaseException as e:
        return addr, fn, None, None, repr(e)[:80]
if __name__ == "__main__":
    items = []
    for fn in sorted(os.listdir(SRC)):
        if not (fn.endswith("_open.cpp") or fn == "E_app.cpp"): continue
        t = open(os.path.join(SRC, fn), encoding="latin-1").read()
        for seg in P.split_functions(t):
            if seg[0] == "func":
                if fn == "E_app.cpp" and not any("OPEN" in c for c in seg[1]): continue
                m = re.search(r"(?:M|FUN)_([0-9a-f]{8})\(", seg[2])
                if m: items.append((m.group(1), fn))
    with ProcessPoolExecutor(10) as ex:
        res = list(ex.map(job, items))
    res.sort(key=lambda r: (r[2] is None, r[2] if r[2] is not None else 0))
    with open(os.path.join(fnsrc.DECOMP, "build", "M", "open_scores.tsv"), "w") as f:
        for r in res: f.write("\t".join(str(x) for x in r) + "\n")
    print(len(res))
