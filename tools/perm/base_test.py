import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fnsrc, score
W = os.path.join(fnsrc.DECOMP, "build", "M", "scratch")
for a in sys.argv[1:]:
    f = fnsrc.find_file(a)
    if not f:
        print(a, "no file"); continue
    flags = ["/GX"] if f == "E_app.cpp" or "E_app" in f else []
    t = fnsrc.Target(f, a)
    tgt = int(a, 16)
    r1 = score.compile_and_score(t.raw_source(), tgt, W, flags, "raw_" + a)
    r2 = score.compile_and_score(t.source_for(t.tree_body(t.tree)), tgt, W, flags, "rt_" + a)
    print(a, f, "raw", {k: r1.get(k) for k in ("score", "size", "error")}, "roundtrip", {k: r2.get(k) for k in ("score", "size", "error")})
