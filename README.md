<p align="center"><img src="TSH%20Banner.png" alt="Typing Space Harrier decompilation" width="720"></p>

# Typing Space Harrier — matching decompilation

A work-in-progress **matching decompilation** of *Typing Space Harrier* (SEGA, 2002, Japanese PC release, `tsh.exe`): C++ source
that Visual C++ 6.0 compiles into byte-identical code, function by function.

This repository contains no game files. You need your own copy of the game.

| | |
|---|---|
| Game code (506 functions, 115,864 bytes) | **417 functions match** (82.4 %), 57,976 bytes (50.0 % of the code) |
| Initialised data | 771,252 bytes reproduced (generated from your exe, see below) |
| C runtime (193 functions) | statically linked VC6 `libc.lib`, not decompiled |
| Compiler | Visual C++ 6.0, cl 12.00.8168 (`/O2`), linker 6.00 |

Open work: ~82 functions that differ only in register allocation or block layout, ~7 unstarted (mostly large, data-heavy
ones), the string-literal pool, the post-link junk bytes between functions, and the final link of the whole executable.
See `docs/DECOMP_WORKFLOW.md` and `docs/reports/`. Time and token statistics for the work so far are in `docs/PROJECT_STATS.md` (regenerate with `python tools/usage_report.py`).

## Building

You need
- Windows,
- **Visual C++ 6.0** (not included; cl 12.00.8168, the folder that contains `VC98\`),
- the original `tsh.exe` as `orig/tsh.exe` (SHA-1 `185fec0816def349de984d6bdb402753004dfc1e`, see `orig/README.md`),
- Python 3.9+ with `pip install -r requirements.txt` (capstone).

```
set VC6_DIR=C:\Program Files (x86)\Microsoft Visual Studio
python tools/build_all.py                       compile every src/*.cpp and compare each function with the original
python tools/build_all.py --data                also generate the data sources (src/K_*.cpp) from your exe and check them
python tools/build_all.py --file A_game.cpp --verbose      one file, with side-by-side diffs
python tools/build_all.py --report build/jp/report.json    write an objdiff-format progress report
```

Results are written to `build/` (`progress.tsv`, `results.tsv`, `report.json`). A function is a *match* when its bytes and its
relocation targets are identical to the original.

## Layout

| Path | Contents |
|---|---|
| `src/` | decompiled sources (`<letter>_<topic>.cpp`; `*_open.cpp` hold functions that do not match yet, each with an `// OPEN:` note) |
| `config/` | function list, struct members, method/global declarations, symbol names, per-file compiler flags |
| `tools/` | build driver, comparer, report generator, data generator, Ghidra export scripts, a small permuter (`tools/perm`) |
| `docs/` | findings about the executable (`RE_NOTES.md`), workflow, and per-partition reports of what was tried |
| `orig/` | put your own `tsh.exe` here (git-ignored) |

`include/game.h` is generated from `config/` by `tools/gen_game_h.py`. `src/K_*.cpp` (typed initialised data, including the stage
script) are generated from your exe by `tools/data/gen_data.py` and are not committed.

## Progress report and CI

The progress report is `progress/jp/report.json`, generated locally with
`python tools/build_all.py --data --report progress/jp/report.json --version jp` and committed. `.github/workflows/report.yml`
uploads it as the artifact `jp_report` on every push to `master`, which is what ByteMatch reads. It follows the structure of
objdiff's `report.json` (one unit per source file, unstarted functions as 0 % units); fuzzy percentages for non-matching
functions come from our own instruction-level similarity, not objdiff's algorithm. It holds only names, addresses, sizes and
percentages.

Neither VC6 nor the original executable can be placed on GitHub-hosted runners, so the build itself is not run there.
`.github/workflows/build.yml` (manual) rebuilds and verifies on a **self-hosted Windows runner** with the label `vc6`; set the
repository variables `VC6_DIR` and `TSH_ORIG_PATH` (see the comments in the workflow).

## Notes on the game

- Window code, a stage timeline interpreter and DirectDraw/DirectSound glue make up most of the code; the whole game state is one
  large `Game` object (0x21EFBF8 bytes) with `__thiscall` methods.
- Word banks (`WORD/word_NN.dat`) are cp932 text, XOR-obfuscated unless the file starts with CR LF; see `tools/tsh_words.py`.
- The game runs at modern resolutions through a DirectDraw wrapper such as dgVoodoo (640x480, 8-bit palettised mode).

## Contributing

Read `AGENTS.md`: matches must also read like authored source (no fakematches), names stay address-based until the meaning is
certain, and no game files or generated data go into git.

## License

MIT, see `LICENSE` (covers the tools and the original source written for this project; the game and its code belong to SEGA).
