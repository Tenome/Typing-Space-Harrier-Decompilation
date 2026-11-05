# qperm.py <src.cpp> <func marker> <max variants> <hex addr> : permute order of non-array local declarations
import sys, re, itertools, subprocess, os, random
src, marker, maxn, addr = sys.argv[1], sys.argv[2], int(sys.argv[3]), sys.argv[4]
text = open(src).read()
a = text.index(marker)
b = text.index("{\n", a) + 2
e = text.index("\n\n", b)
decl = text[b:e].split("\n")
fixed = [d for d in decl if "[" in d]
movable = [d for d in decl if "[" not in d]
variants = list(itertools.permutations(range(len(movable))))
random.seed(1); random.shuffle(variants)
best = 0
tmp = "src/Q_perm_tmp.cpp"
for v in variants[:maxn]:
    new = text[:b] + "\n".join(fixed + [movable[i] for i in v]) + text[e:]
    open(tmp, "w").write(new)
    out = subprocess.run(["sh", "tools/q.sh", "Q_perm_tmp.cpp"], capture_output=True, text=True).stdout
    tl = [l for l in out.split("\n") if re.match(r"(MATCH|DIFF) +" + addr + " ", l)]
    if tl and tl[0].startswith("MATCH"):
        print("MATCH with order", [movable[i].strip() for i in v]); open(src.replace(".cpp", "_perm.txt"), "w").write(new); break
    m = re.search(r"similarity ([0-9.]+)%", out)
    if m and float(m.group(1)) > best:
        best = float(m.group(1)); print(best, [movable[i].strip() for i in v])
os.remove(tmp)
