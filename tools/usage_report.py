#!/usr/bin/env python3
"""Project time and token report from Claude Code session transcripts.

  python tools/usage_report.py [--session ID] [--projects DIR] [--out docs/PROJECT_STATS.md]

Reads the main session transcript (<projects>/<project>/<ID>.jsonl) and every subagent transcript below it
(<projects>/<project>/<ID>/subagents/*.jsonl), de-duplicates streamed messages by id, and writes a Markdown report:
wall-clock span, estimated active time (gaps over 15 minutes count as idle), tokens by category per day and per kind
(main conversation, subagents), plus the repository's commit and function counts. Re-run it to update the report.
"""
import argparse
import collections
import datetime as dt
import glob
import json
import os
import subprocess

IDLE_GAP = 15 * 60
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
DEFAULT_SESSION = "8e7f34e9-a3d9-4eaf-9a76-d07b2197d319"


def parse_ts(value):
    return dt.datetime.fromisoformat(value.replace("Z", "+00:00"))


def scan(path):
    """Yield (timestamp, usage dict) per distinct assistant message, and all event timestamps."""
    seen = {}
    stamps = []
    with open(path, encoding="utf-8", errors="replace") as fh:
        for line in fh:
            try:
                rec = json.loads(line)
            except ValueError:
                continue
            ts = rec.get("timestamp")
            if ts:
                try:
                    stamps.append(parse_ts(ts))
                except ValueError:
                    pass
            msg = rec.get("message")
            if isinstance(msg, dict) and msg.get("role") == "assistant" and isinstance(msg.get("usage"), dict):
                key = msg.get("id") or rec.get("uuid")
                seen[key] = (parse_ts(ts) if ts else None, msg["usage"], msg.get("model"))
    return list(seen.values()), stamps


def active_seconds(stamps):
    stamps = sorted(stamps)
    total = 0.0
    for a, b in zip(stamps, stamps[1:]):
        gap = (b - a).total_seconds()
        if gap <= IDLE_GAP:
            total += gap
    return total


def fmt_hours(seconds):
    return "%.1f h" % (seconds / 3600.0)


def tokens(usage):
    return (usage.get("input_tokens", 0) or 0, usage.get("output_tokens", 0) or 0,
            usage.get("cache_creation_input_tokens", 0) or 0, usage.get("cache_read_input_tokens", 0) or 0)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--session", default=DEFAULT_SESSION)
    ap.add_argument("--projects", default=os.path.join(os.path.expanduser("~"), ".claude", "projects"))
    ap.add_argument("--out", default=os.path.join(ROOT, "docs", "PROJECT_STATS.md"))
    args = ap.parse_args()

    main_files = glob.glob(os.path.join(args.projects, "*", args.session + ".jsonl"))
    if not main_files:
        raise SystemExit("session transcript not found under " + args.projects)
    main_file = main_files[0]
    sub_files = sorted(glob.glob(os.path.join(os.path.dirname(main_file), args.session, "subagents", "*.jsonl")))

    per_day = collections.defaultdict(lambda: collections.defaultdict(lambda: [0, 0, 0, 0]))
    models = collections.Counter()
    all_stamps = []
    kinds = {"main conversation": [main_file], "subagents": sub_files}
    active = {}
    agent_hours = 0.0
    counts = {}
    for kind, files in kinds.items():
        stamps_kind = []
        n_msgs = 0
        for f in files:
            msgs, stamps = scan(f)
            if kind == "subagents":
                agent_hours += active_seconds(stamps)
            stamps_kind += stamps
            all_stamps += stamps
            for ts, usage, model in msgs:
                if ts is None:
                    continue
                day = ts.astimezone().date().isoformat()
                t = tokens(usage)
                for i in range(4):
                    per_day[kind][day][i] += t[i]
                models[model or "?"] += 1
                n_msgs += 1
        active[kind] = active_seconds(stamps_kind)
        counts[kind] = (len(files), n_msgs)

    first, last = min(all_stamps), max(all_stamps)
    total = [0, 0, 0, 0]
    kind_total = {}
    for kind in kinds:
        kt = [0, 0, 0, 0]
        for day, v in per_day[kind].items():
            for i in range(4):
                kt[i] += v[i]
        kind_total[kind] = kt
        for i in range(4):
            total[i] += kt[i]

    def commits():
        try:
            out = subprocess.run(["git", "log", "--format=%H"], cwd=ROOT, capture_output=True, text=True).stdout.split()
            return len(out)
        except OSError:
            return 0

    progress = ""
    report = os.path.join(ROOT, "progress", "jp", "report.json")
    if os.path.exists(report):
        m = json.load(open(report, encoding="utf-8"))["measures"]
        progress = "%d of %d game functions match (%.1f %% of code bytes)" % (
            m["matched_functions"], m["total_functions"], float(m["matched_code_percent"]))

    days = sorted({d for kind in kinds for d in per_day[kind]})
    lines = ["# Project statistics", "",
             "Generated by `tools/usage_report.py` from the Claude Code session transcripts of this project. Re-run it to update.", "",
             "| | |", "|---|---|",
             "| First activity | %s |" % first.astimezone().strftime("%Y-%m-%d %H:%M"),
             "| Latest activity | %s |" % last.astimezone().strftime("%Y-%m-%d %H:%M"),
             "| Wall-clock span | %s (%.1f days) |" % (fmt_hours((last - first).total_seconds()), (last - first).total_seconds() / 86400),
             "| Estimated active time | %s (main conversation %s; subagents %s; gaps over %d min counted as idle; subagent time overlaps) |" % (
                 fmt_hours(active_seconds(all_stamps)), fmt_hours(active["main conversation"]), fmt_hours(active["subagents"]), IDLE_GAP // 60),
             "| Cumulative subagent working time | %s (each subagent counted separately, so parallel runs add up) |" % fmt_hours(agent_hours),
             "| Git commits | %d |" % commits(),
             "| Progress | %s |" % (progress or "n/a"),
             "| Subagent runs | %d transcripts |" % counts["subagents"][0], "",
             "## Tokens", "",
             "| Kind | Input (uncached) | Output | Cache writes | Cache reads | Total |", "|---|---:|---:|---:|---:|---:|"]
    for kind in kinds:
        kt = kind_total[kind]
        lines.append("| %s | %s | %s | %s | %s | %s |" % (kind, *(f"{x:,}" for x in kt), f"{sum(kt):,}"))
    lines.append("| **all** | %s | %s | %s | %s | **%s** |" % (*(f"{x:,}" for x in total), f"{sum(total):,}"))
    lines += ["", "Input and output are the tokens processed and generated; cache writes and reads are prompt-cache traffic",
              "(re-reading the conversation on each step), which is why the totals are far larger than the visible text.", "",
              "## By day", "", "| Day | Main: output | Main: total | Subagents: output | Subagents: total |", "|---|---:|---:|---:|---:|"]
    for day in days:
        a = per_day["main conversation"].get(day, [0, 0, 0, 0])
        b = per_day["subagents"].get(day, [0, 0, 0, 0])
        lines.append("| %s | %s | %s | %s | %s |" % (day, f"{a[1]:,}", f"{sum(a):,}", f"{b[1]:,}", f"{sum(b):,}"))
    lines += ["", "## Models", ""] + ["- %s: %s assistant messages" % (k, f"{v:,}") for k, v in models.most_common()]
    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    with open(args.out, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines) + "\n")
    print("\n".join(lines))


if __name__ == "__main__":
    main()
