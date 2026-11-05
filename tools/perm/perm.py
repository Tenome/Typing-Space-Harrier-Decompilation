"""Lane M permuter for the tsh.exe matching decompilation.

usage: python perm.py <hexaddr> [--file X_open.cpp] [--minutes N] [--workers N] [--flags "/GX"] [--seed S]
                      [--pop K] [--keep-others] [--stop-at-zero]
Compiles random mutated variants of one parked near-miss function with VC6 (same flags as build.bat) and scores them
against tsh.exe (aligned instruction diff; 0 = byte match). Results go to build/M/out/<addr>/.
"""
import argparse
import concurrent.futures as cf
import copy
import difflib
import hashlib
import json
import os
import random
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import cparse as P  # noqa: E402
import fnsrc  # noqa: E402
import mutate as M  # noqa: E402
import score as S  # noqa: E402

DECOMP = fnsrc.DECOMP


def worker(job):
    text, target, flags, base, tag = job
    wd = os.path.join(base, "w%d" % os.getpid())
    try:
        r = S.compile_and_score(text, target, wd, flags, "v")
    except Exception as e:  # pragma: no cover
        r = {"error": repr(e)}
    return tag, r


def key(r):
    return (r["score"], r["size_delta"])


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("addr")
    ap.add_argument("--file")
    ap.add_argument("--minutes", type=float, default=10)
    ap.add_argument("--workers", type=int, default=8)
    ap.add_argument("--flags", default="")
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--pop", type=int, default=10)
    ap.add_argument("--keep-others", action="store_true")
    ap.add_argument("--max-evals", type=int, default=10 ** 9)
    ap.add_argument("--focus", help="regex: bias expression/swap mutations to statements matching it")
    ap.add_argument("--from-best", help="start from a previous best.cpp body file")
    args = ap.parse_args()

    if args.focus:
        import re
        M.FOCUS = re.compile(args.focus)
    addr = args.addr.lower().zfill(8)
    target = int(addr, 16)
    fn = args.file or fnsrc.find_file(addr)
    if not fn:
        raise SystemExit("function not found")
    flags = args.flags.split() if args.flags else (["/GX"] if fn == "E_app.cpp" else [])
    tgt = fnsrc.Target(fn, addr, keep_others=args.keep_others)
    import mutate2
    mutate2.PARAMS[:] = mutate2.parse_params(tgt.header)
    base_dir = os.path.join(DECOMP, "build", "M", "out", addr)
    os.makedirs(base_dir, exist_ok=True)
    scratch = os.path.join(DECOMP, "build", "M", "scratch")
    rng = random.Random(args.seed)
    log = open(os.path.join(base_dir, "log.txt"), "a")

    def say(*a):
        s = " ".join(str(x) for x in a)
        print(s, flush=True)
        log.write(s + "\n")
        log.flush()

    start_tree = tgt.tree
    if args.from_best:
        start_tree = P.parse_function_body(open(args.from_best).read())
    base_text = tgt.source_for(P.print_body(start_tree))
    raw = S.compile_and_score(tgt.raw_source(), target, scratch, flags, "raw_" + addr)
    if "error" in raw:
        raise SystemExit("baseline does not compile: " + raw["error"])
    say("== %s in %s flags=%s seed=%d raw baseline score=%d size=%d (inventory delta %d)" % (addr, fn, flags, args.seed, raw["score"], raw["size"], raw["size_delta"]))

    seen_text = set()
    pool = []  # dicts: r, tree, hist
    r0 = S.compile_and_score(base_text, target, scratch, flags, "base_" + addr)
    pool.append({"r": r0, "tree": start_tree, "hist": []})
    seen_text.add(hashlib.md5(base_text.encode()).hexdigest())
    say("normalised baseline score=%d" % r0["score"])
    best = pool[0]
    deadline = time.time() + args.minutes * 60
    evals = 0
    errors = 0
    t0 = time.time()
    nworkers = args.workers
    with cf.ProcessPoolExecutor(max_workers=nworkers) as ex:
        while time.time() < deadline and evals < args.max_evals:
            batch = []
            attempts = 0
            while len(batch) < nworkers * 2 and attempts < 200:
                attempts += 1
                # tournament parent selection biased to the best
                cand = rng.sample(pool, min(len(pool), 3))
                parent = min(cand, key=lambda p: key(p["r"]))
                tree = parent["tree"]
                hist = list(parent["hist"])
                nmut = rng.choices([1, 2, 3], [70, 22, 8])[0]
                ok = True
                for _ in range(nmut):
                    res = M.mutate(tree, rng)
                    if not res:
                        ok = False
                        break
                    tree, name = res
                    hist.append(name)
                if not ok:
                    continue
                body = P.print_body(tree)
                text = tgt.source_for(body)
                h = hashlib.md5(text.encode()).hexdigest()
                if h in seen_text:
                    continue
                seen_text.add(h)
                batch.append((tree, hist, body, (text, target, flags, scratch, len(batch))))
            if not batch:
                say("no new mutants")
                break
            results = list(ex.map(worker, [b[3] for b in batch]))
            for (tree, hist, body, job), (tag, r) in zip(batch, results):
                evals += 1
                if "error" in r:
                    errors += 1
                    continue
                state = {"r": r, "tree": tree, "hist": hist, "body": body}
                if key(r) < key(best["r"]):
                    best = state
                    say("[%d evals %ds] new best score=%d size=%d delta=%d hist=%s" % (evals, time.time() - t0, r["score"], r["size"], r["size_delta"], ",".join(hist)))
                    with open(os.path.join(base_dir, "best_%d.cpp" % r["score"]), "w") as f:
                        f.write("// score %d hist %s\n" % (r["score"], ",".join(hist)) + "\n".join(tgt.comment) + "\n" + tgt.header + "\n{\n" + body + "\n}\n")
                    if r["exact"]:
                        say("EXACT MATCH FOUND")
                # insert in pool: unique object hash
                if not any(p["r"]["hash"] == r["hash"] for p in pool):
                    if len(pool) < args.pop or key(r) <= key(pool[-1]["r"]):
                        pool.append(state)
                        pool.sort(key=lambda p: key(p["r"]))
                        del pool[args.pop:]
                else:
                    # replace if the shorter history gives the same object: keep first
                    pass
            if best["r"]["exact"]:
                break
            if evals % (nworkers * 10) < nworkers * 2:
                say("[%d evals %ds errors=%d] pool=%s" % (evals, time.time() - t0, errors, [p["r"]["score"] for p in pool]))
    say("done: %d evals (%d compile errors) in %ds; best score=%d hist=%s" % (evals, errors, time.time() - t0, best["r"]["score"], ",".join(best["hist"])))
    out = os.path.join(base_dir, "best.cpp")
    body = best.get("body") or P.print_body(best["tree"])
    with open(out, "w") as f:
        f.write("\n".join(tgt.comment) + "\n" + tgt.header + "\n{\n" + body + "\n}\n")
    base_body = P.print_body(tgt.tree)
    d = "\n".join(difflib.unified_diff(base_body.split("\n"), body.split("\n"), "baseline", "best", lineterm="", n=2))
    open(os.path.join(base_dir, "best.diff"), "w").write(d + "\n")
    json.dump({"addr": addr, "file": fn, "flags": flags, "raw": raw["score"], "best": best["r"]["score"], "hist": best["hist"],
               "evals": evals}, open(os.path.join(base_dir, "result.json"), "w"))
    # asm diff of best
    text = tgt.source_for(body)
    S.compile_and_score(text, target, scratch, flags, "best_" + addr)
    import io
    import contextlib
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        S.show_diff(os.path.join(scratch, "best_" + addr + ".obj"), target)
    open(os.path.join(base_dir, "best.asmdiff"), "w").write(buf.getvalue())


if __name__ == "__main__":
    main()
