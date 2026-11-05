"""add_win.py <hexaddr>...: take build/M/out/<addr>/best.cpp (must be an exact match) into src/M_<lane>.cpp, rebuild, verify."""
import json, os, re, subprocess, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fnsrc, cparse as P
D = fnsrc.DECOMP
WINS = os.path.join(D, "build", "M", "wins.json")
wins = json.load(open(WINS)) if os.path.exists(WINS) else {}
HDR = '#include <windows.h>\n#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include "game.h"\n'

def tidy(body):
    lines = body.split("\n")
    out = []
    for ln in lines:
        ln = re.sub(r"^(\s+[A-Za-z_][\w ]*?) \* (\w)", r"\1 *\2", ln)
        out.append(ln)
    # blank line after the leading declaration block
    i = 0
    while i < len(out) and re.match(r"^    (unsigned |const |struct )?[A-Za-z_]\w*( \*+|\s+)\*?\w+(\[\w+\])?( = [^;]*)?;$", out[i]) and not out[i].lstrip().startswith(("return", "goto")):
        i += 1
    if 0 < i < len(out) and out[i].strip():
        out.insert(i, "")
    return "\n".join(out)

for a in sys.argv[1:]:
    a = a.lower().zfill(8)
    fn = fnsrc.find_file(a)
    lane = fn[0]
    txt = open(os.path.join(D, "build", "M", "out", a, "best.cpp")).read()
    lines = txt.split("\n")
    lines = [l for l in lines if not l.startswith("//")]
    txt = "\n".join(lines).strip("\n")
    head, body = txt.split("\n{\n", 1)
    body = body.rsplit("\n}", 1)[0]
    wins[a] = {"lane": lane, "file": fn, "text": head + "\n{\n" + tidy(body) + "\n}\n"}
json.dump(wins, open(WINS, "w"), indent=1)
lanes = sorted({w["lane"] for w in wins.values()})
for lane in lanes:
    fs = sorted((a, w) for a, w in wins.items() if w["lane"] == lane)
    flags = ["/GX"] if any(w["file"] == "E_app.cpp" for _, w in fs) else []
    name = "M_%s.cpp" % lane.lower()
    if lane == "E":
        continue
    with open(os.path.join(D, "src", name), "w", newline="\n") as f:
        f.write(HDR + "\n" + "\n".join(w["text"] for _, w in fs))
    env = dict(os.environ, LANE="M")
    r = subprocess.run("set LANE=M&& \"" + os.path.join(D, "build.bat") + "\" " + name, shell=True, cwd=D, capture_output=True, text=True)
    res = [l for l in (r.stdout + r.stderr).split("\n") if l.startswith(("MATCH", "DIFF")) or "error" in l]
    print(name, "\n".join(res))
