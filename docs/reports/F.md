# Lane F report (round 2; lanes\F.tsv, 29 functions)

**Counts: bytes-matched 11 of 29; matched under the rules 11 of 29** (408f60 carries a note, see below).
Files: src\F_misc.cpp (9 matched), src\F_stage.cpp (2 matched), src\F_open.cpp (parked, each with an `// OPEN:` note).

## Build note (important)
The official `build.bat F_x.cpp` fails/diverges until the declaration requests below are applied, because several methods are declared
with the wrong return type in other lanes' `game_methods_*.txt`. I built with `build\F\fb.bat <file>` (identical flags
`/O2 /D WIN32 /D NDEBUG /D _WINDOWS`; it regenerates game.h, then patches a private copy `build\F\inc\game.h` from
`build\F\overrides.txt`, which holds exactly the changes requested here). Helpers: build\F\b.sh, fd.sh, show.sh, t.py, perm.py, park.py.
No per-file compiler flags needed.

## Requests to the orchestrator (declaration changes in other lanes' files)
1. game_methods_B.txt: `void M_004093b0(int a, int b, int c, int d);` -> `int M_004093b0(...)` (returns eax; 409330 uses it). B's definition of
   M_004093b0 does not exist (it is lane F's function), nothing else changes.
2. game_methods_B.txt: `void M_0040b3f0(int a);` -> `int M_0040b3f0(int a);` and B_misc.cpp `return M_0040b430(a, 8, 0);` (bytes unchanged; 40b390 uses the result).
3. game_methods_B.txt: `void M_0040b350(int a);` -> `int ...` and `void M_0040b330(int a);` -> `int ...` (B_misc.cpp: add `return`; callers 40a270, 40b140).
4. game_methods_B.txt: `void M_004081b0(int n);` -> `int M_004081b0(int scanOnly);` (lane F function).
5. game_prelude_A.h `struct Field`: replace `char m_pad7c5210[0x10000];` by `int m_7c5210[0x4000];` (object list; 40a7b0, 40a270). Needed for 40a7b0 to compile in the official build.
6. game_prelude_A.h `struct Field`: replace `char m_pad785000[0x4020c];` by `int m_785000; int m_785004[0x4000]; int m_795004[0x4000]; int m_7a5004[0x4000]; int m_7b5004[0x81]; int m_7b5208; int m_7b520c[0x4000];` (depth-sort tables; 40a270 in F_open).
7. game_prelude_B.h `struct Rec_87cbc8`: members m_2c..m_8c are really one `int m_2c[0x19]` array (indexed by 0x4093b0, 0x40b5c0); only needed to enable the disabled 4093b0 draft.
8. Lane A `m_87cbd0/m_87cbd8` records are indexed 0x28/0x48 etc. through Rec_87cbd4 members (fine as is).
Conflicts seen: lane A/B declare `M_00408320(int a)`, `M_00408660`, `M_004093b0`, `M_00409330` with different parameter names (harmless warnings).

## Bytes-matched (11)
407fd0 408320 (F_stage.cpp); 408ad0 408b70 408f60 409190 409330 40a7b0 40b390 40b850 40b8b0 (F_misc.cpp).
Needs requests 1-3/5 to build officially: 409330 (1), 40b390 (2), 40a7b0 (5).
408f60: the element is read as `short entry` and also as `int value = entry` (the original sign-extends once into ebp and stores ax);
two names for one element is a guess at the original, but the bytes need both widths. Counted as matched.
Lessons recorded: bounded chain walk = `for (guard = 0; guard < 0x40; guard++)` (408b70); direct `Table[n].m_x` indexing, not a row pointer,
gives lea/shl with absolute bases (408ad0); using the parameter itself as the stage keeps it in edi and the dead home slot is reused (408b70);
`if (c) f(-1) else f(x)` as two calls gives the original push order, a ternary does not (409330); order of the first local initialisations
changes scheduling (407fd0: stage, next, phase, song); locals that cache the next row's fields (`stageNo = T[row].m_0; flag = T[row].m_4;` at the loop end)
reproduce the original strength-reduced pointer (408320); for-loop on a count member with no early return puts the zero-count exit last (40a7b0);
float locals passed to float parameters are pushed as raw bits (40b140/40b850).

## Open / parked (src\F_open.cpp), 18 functions
Near misses (draft exact in size or flow):
- 004096b0 (register/schedule): 75 vs 74 B; original pushes esi after the early return; many loop spellings tried.
- 00408130 (CFG): exact except the order of the two return blocks (return 1 block first in the original). Induction base now correct (locals m_0/m_4 trick).
- 004081b0 (register/schedule): exact except the strength-reduced induction base (original m_10, mine m_0); try the cached-locals trick from 408320.
- 004090a0 (register): 234 B exact size, only this/total register swap (edi/ebx); 120 declaration orders tried.
- 0040a270 (schedule): 539 vs 538 B; stack slot assignment and placed++/count increment interleave.
- 0040b140 (register): 478 vs 494 B; original keeps count on the stack, pointer/index in ebp/ebx.
- 0040ba50 (CFG): original shares one `m_c = 0; return 0` block between two paths with a hoisted zero register (goto-like).
- 00407310 (register/schedule): shuffle loop and spawner switch match until the roll; row*3 precomputed before rand(), mode expression compiles differently.
- 00408660 (CFG/register): the id-remap switch gives the same byte-index + dword jump tables, but the original keeps `this` in esi and the parameter in eax and jumps to one shared tail; mine inlines the tail after case 1.
- 004093b0 (register; disabled draft under `#if 0` until request 7).
Analysed, not drafted (need data modelling first; too long for matching without it): 4078f0 (script interpreter, ~30 globals), 408d00 (contains an inlined
copy of 408f60 without the final call: probably a shared static helper), 409700, 409e00, 40a900, 40b5c0 (uses the Rec_87cbc8 int array), 40bce0 (initialises
arrays that overlap lane B's m_87cbfc/m_87cc28 fields: needs B's declarations merged into 0x400-int and 0xa00-short arrays), 40bfa0 (string/colour-code
interpreter with gotos and an inline strlen).

## MATCH-PENDING: none. ORIGINAL-ASM: none.
## Tool problems
- Official build.bat produces duplicate-declaration warnings from other lanes (harmless). `perm.py` with >10 locals is too slow (each build ~2 s).
