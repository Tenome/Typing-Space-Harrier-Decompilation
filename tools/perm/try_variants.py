"""try_variants.py <hexaddr> <file-with-variants>  : variants separated by lines '#### name'; each is a function body."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fnsrc, score
addr = sys.argv[1]
f = fnsrc.find_file(addr)
flags = ["/GX"] if f == "E_app.cpp" else []
t = fnsrc.Target(f, addr)
W = os.path.join(fnsrc.DECOMP, "build", "M", "scratch")
cur, name, vs = [], None, []
for ln in open(sys.argv[2]).read().split("\n"):
    if ln.startswith("####"):
        if name: vs.append((name, "\n".join(cur)))
        name, cur = ln[4:].strip(), []
    else:
        cur.append(ln)
if name: vs.append((name, "\n".join(cur)))
for n, b in vs:
    r = score.compile_and_score(t.source_for(b), int(addr, 16), W, flags, "tv")
    print(n, r.get("score"), r.get("exact"), (r.get("error") or "")[-200:])
