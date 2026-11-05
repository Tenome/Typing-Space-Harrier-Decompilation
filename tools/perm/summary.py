import json, os, sys
root = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "build", "M", "out")
rows = []
for d in sorted(os.listdir(root)):
    p = os.path.join(root, d, "result.json")
    if os.path.exists(p):
        r = json.load(open(p))
        rows.append(r)
for r in rows:
    print("%s\t%s\traw %s\tbest %s\tevals %s\t%s" % (r["addr"], r["file"], r["raw"], r["best"], r["evals"], ",".join(r["hist"])[:80]))
