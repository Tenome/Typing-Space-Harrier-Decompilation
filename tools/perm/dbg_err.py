import sys, os, random, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fnsrc, score, mutate as M, cparse as P
a = sys.argv[1]; n = int(sys.argv[2])
f = fnsrc.find_file(a); t = fnsrc.Target(f, a)
rng = random.Random(5)
W = os.path.join(fnsrc.DECOMP, "build", "M", "scratch")
cnt = collections.Counter()
shown = 0
for i in range(n):
    res = M.mutate(t.tree, rng)
    if not res: continue
    tree, name = res
    r = score.compile_and_score(t.source_for(P.print_body(tree)), int(a, 16), W, [], "dbg")
    cnt[(name, "error" in r)] += 1
    if "error" in r and shown < 4:
        shown += 1
        print(name, r["error"][-300:])
        print(P.print_body(tree)[:1500])
print(sorted(cnt.items()))
