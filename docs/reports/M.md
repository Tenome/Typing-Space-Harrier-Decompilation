# Lane M report: permuter and near-miss search

Lane M owns `tools\perm\`, `src\M_*.cpp`, `reports\M.md`, `build\M\`, `progress_M.tsv`. Nothing outside those was edited.

## Result summary

Matched (byte-exact with `build.bat`, `LANE=M`, reader test passed, all in `src\M_loops.cpp` and `src\M_app.cpp`):

| address | function | winning change against the parked copy |
|---|---|---|
| 0x403650 | `Game::M_00403650` (shift string left, max 0x1f) | loop written `while (1) { if (*p == 0) break; ...; if (i >= 0x1f) break; }` |
| 0x408130 | `Game::M_00408130` (find matching stage row) | `while (1)` with the sentinel test `if (m_10 < 0 && m_1c < 0) break;` after the two row loads, `i++` at the loop end, `return 0` after the loop |
| 0x40c650 | `Game::M_0040c650` (clear BankRec table) | `while (1) { ...; i++; if (i >= 0x50) break; }` instead of do/while |
| 0x40c6a0 | `Game::M_0040c6a0` (clear GroupRec table) | same loop form, and the bound is **72** (array size, the parked copy used 71: it was a wrong loop count, not only a spelling) |
| 0x40e2a0 | `Game::M_0040e2a0` (upper-case a string) | walk with a local `char *p = str;` instead of advancing the parameter |
| 0x41e300 | `EzWindow::M_0041e300` | RECT initialised in member order: `rc.left = 0; rc.top = 0; rc.right = wc.width; rc.bottom = wc.height;` |
| 0x40a270 | `Game::M_0040a270` (depth bucket sort, 538 B) | see below |
| 0x41ed50 | `EzWndClass::M_0041ed50` | RECT initialised `right, bottom, left, top`; locals declared `int h; int w;` and `w` computed before `h` |

That is 8 functions (0x40a270 is in `src\M_depth.cpp`: decl order a,j,n before b; `m_7b5004[bucket]++` before `placed++`; sort test written `A[a] > A[c]`). `src\M_app.cpp` has to repeat the class definitions of `E_app.cpp` (EzWndClass, EzWindow, TypingWindow...) because they are local to that file; the orchestrator should move the two winning bodies into `E_app.cpp` (replacing the `// OPEN` copies) and delete `M_app.cpp`. Build flag: `/GX` for `M_app.cpp` (like `E_app.cpp`).
`M_loops.cpp` needs no flags and no other declaration (types come from `game.h`); the parked copies in `A_open`, `B_open`, `C_open`, `F_open` are untouched, the orchestrator removes them.

MATCH-PENDING: none. Every win above is plain source (no dummy variables, no asm, no pragmas). Two spelling choices the orchestrator may want to look at:
- `while (1) { ... if (i >= N) break; }` in 0x40c650/0x40c6a0/0x403650/0x408130: VC6 generates different code for this than for `do {} while (i < N)`, `for` or `while (cond)`. Lesson learned: a loop with the exit test written as an explicit `break` is a distinct compiler input, try it for every "loop rotation / induction / zero register" wall.
- `char *p = str;` in 0x40e2a0 (the parameter is not modified, an extra cursor is a normal idiom).

Best scores of the rest (aligned instruction differences against the original, 0 = bytes equal; see "Scoring"): see the table at the end.

## The tool (`tools\perm\`)

Pure Python 3.13 (capstone for the diff), no network. Everything compiles with the same command line as `build.bat` (`cl /nologo /c /O2 /D WIN32 /D NDEBUG /D _WINDOWS`, plus `--flags`), each worker in its own directory `build\M\scratch\w<pid>\`.

Run (PowerShell or bash; cmd needs the full Python path):

```
python tools/perm/perm.py 41ed50 --minutes 5 --workers 6 [--file E_app.cpp] [--flags "/GX"] [--seed N] [--pop 10] [--focus REGEX]
tools\perm\run_batch.sh <minutes> <workers per job> <parallel jobs> addr...     (several functions, e.g. 5 6 2 ...)
tools\perm\try_variants.py <addr> <file>   compile hand-written bodies, file = blocks starting with "#### name"
tools\perm\base_test.py <addr>...          baseline score of the parked copy and of its re-printed form
tools\perm\summary.py                      table of the last run per function
```

Output per function in `build\M\out\<addr>\`: `log.txt`, `best.cpp` (whole function), `best.diff` (against the re-printed parked copy), `best.asmdiff`, `best_<score>.cpp` (every improvement), `result.json`. `EXACT MATCH FOUND` is logged and the run stops.

How it works
- `fnsrc.py` finds the function in the `src\*.cpp` file (any lane), keeps the file prelude (includes, externs, struct definitions, `static` helpers) and drops the other function bodies, so one compile takes about 0.4 s for small functions.
- `cparse.py` parses the body into statements (blocks, if/else, for, while, do, switch with labels, declarations) and prints it back (4 space indent, bodies of one statement without braces as in the lane files). The re-printed baseline gives the same score as the raw text for all 25 candidates (checked with `base_test.py`).
- `mutate.py` / `mutate2.py` apply one to three random mutations to a state picked by tournament from a pool of the best distinct object codes (pool size `--pop`); equal-score states with different code are kept so the search can cross plateaus.
- `score.py` compiles, takes the function out of the .obj with `compare_obj.py` (so symbol/relocation resolution is exactly the project's), and scores: aligned instruction diff of mine against the original (difflib on mnemonic+operands, absolute addresses and relocations normalised to `ABS`, branch targets compared through the alignment), plus the relocation-target errors when the text is equal; 0 only if `compare_obj` says MATCH (bytes, relocation targets and size).

Mutations (semantics preserving by heuristic; the independence test knows reads, writes, calls, memory, and treats `local.field` as separate locations, so `rc.left = 0; rc.top = 0;` can be reordered)
- statements: swap adjacent independent statements, move a statement across independent ones, permute a run of 3-6 independent statements;
- declarations: shuffle/swap/split/merge declaration statements and declarators, move a declaration to its first use / the block top / into the inner block that uses it / to the function top, merge `T x; ... x = e;` into `T x = e;` and split back, toggle `unsigned` on int/char/short locals;
- expressions: flip comparison operands (`a < b` to `b > a`), commute `+ * & | ^ == !=` and pure `&& ||` operands (casts understood), `<=`/`<` and `>`/`>=` with a constant (`x <= 4` to `x < 5`, negative constants too), `!x`/`x == 0`/`x != 0`/`x`, `x += e` / `x = x + e`, `x++`/`++x`/`x += 1`, ternary arms swapped, `?:` to if/else and back, fold `v++; if (v ...)` into `if (++v ...)`, fold `v = e; if (v ...)` into `if ((v = e) ...)`, `a = b; c = a;` into `c = a = b;`;
- control flow: if/else with negated condition, `if (a && b)` to nested ifs and back, early return to if/else wrapping and back, `for`<->`while`, `while (c)` to `if (c) do ... while (c)` and to `for (;;) { if (!c) break; ... }`, `do/while` to `for(;;)`, `for (;;)` with a trailing `else return X` to `break` plus `return`, `i < n` to `i != n` in counting loops, switch case-group reorder / merge / split of identical bodies, switch to if-chain and back;
- variables: cache a never-assigned `m_xxx`/`DAT_xxx` in a local (type looked up in `include\game.h`; named `cache_<name>`, needs a human name if it wins), inline a local back, copy a modified parameter into a local cursor (`p`, `cur`, `ptr`), hoist a repeated `a - b` into a local `tN`, inline a single-use local.
Anything that wins and contains a mutation-invented name (`cache_*`, `tN`) is MATCH-PENDING until a human names it.

`--focus REGEX` biases the expression and swap mutations to statements matching the regex.

## Findings about the compiler (for the other lanes)
- `while (1) { if (c) break; ... }` is not the same input as `while (c)` or `do {} while`; it changes loop rotation, which register holds the induction value and which block is placed first.
- Order of the four `RECT` member assignments changes the load schedule of the whole prologue (0x41e300, 0x41ed50).
- A walked parameter versus a local cursor (0x40e2a0) decides whether the first `*p` test is folded into the register load.
- The parked 0x40c6a0 had loop count 71 for a 72-element table; the relocation (`cmp edx, DAT_008f5450+...`) is what exposed it. Check loop bounds against relocation addends in other parked loops.

## Scoring pitfalls found while building the tool (for the record)
- `compare_obj.compare_function` reports `size_ok` false for very small functions whose end is not followed by padding; `main()` accepts them through the inventory size, so the permuter must too (fixed).
- Relocation errors cannot be counted when the code layout differs (offsets misalign), only when the instruction text is already equal.

## Open functions, best scores

(see the table below; "start" = the parked copy as in `*_open.cpp` with the final scoring, "best" = best state reached by the permuter, 0 = match.)

| address | start | best |
|---|---|---|
| 0x407cd0 | 1 | 1 (SIB order base/index) |
| 0x4043d0 | 2 | 2 (load of m_8ac before cmp; jb vs jl in pointer loop) |
| 0x4180f0 | 66 | 14 |
| 0x4096b0 | 9 | 8 |
| 0x4081b0 | 8 | 7 |
| 0x40d850 | 7 | 6 |
| 0x40ef00 | 6 | 6 |
| 0x4071c0 | 10 | 9 |
| 0x40ba00 | 9 | 8 |
| 0x4121f0 | 45 | 9 |
| 0x40d2c0 | 12 | 11 |
| 0x410c20 | 20 | 11 |
| 0x40fab0 | 22 | 13 |
| 0x4090a0 | 31 | 24 |
| 0x404040 | 38 | 27 |
| 0x404490 | 63 | 53 |
| 0x4052e0 | 88 | 61 |

## Tool problems
None in the shared tools. `build.bat` prints duplicate-declaration warnings from other lanes' `game_methods_*.txt` (`M_00408f60`, `M_0040b140`); harmless.
