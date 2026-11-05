# Lane Q report (round 3; lanes\Q.tsv, 22 large functions)

**Counts: bytes-matched 5 of 22; matched under the rules 5 of 22.** One of them (0x40e380) needs a declaration request to build in the
official build (below). Files: src\Q_b5c0.cpp, Q_sprite.cpp, Q_stage.cpp, Q_progress.cpp, Q_script.cpp (matched), Q_open.cpp and
Q_open_balloon.cpp (parked, each with an `// OPEN:` note). New config: game_prelude_Q.h, game_fields_Q.tsv, slot_fields_Q.tsv, game_methods_Q.txt.

## Matched
- 0x40b5c0 (Q_b5c0.cpp): slot-move pass over the 6-row table at 0x4e261c. The table is an `int[24]` indexed by a stride-4 variable `k` whose
  `k / 4 + 1` is not folded (that is where the stack counter and the `sar ecx,2` come from); `(&m_87cbc8->m_2c)[...]` indexes the item array.
  One `goto` pair (failed/finish) because the failure exit skips the rest of the pass.
- 0x40e380 (Q_sprite.cpp): SPRITE.HJT loader (path from DAT_011215bc + DAT_0042e11c + "SPRITE.HJT", reads the file, loads the 127 sprite groups
  through 0x40e570). `m_87cbfc[group]` is stored after the call. Loop `for (group...) { if (name[group][0] == 0) break; ... }` over a `char *[0x7f]`.
- 0x4093b0 (Q_stage.cpp): from the lane F draft; fixes: index loop for the fetch (`serials[got]`), `failed`/`unit` initialised after the
  random block, `if (cap > 0) ... else if (limit > 0) ... else`, third loop as `for (j...)`, `return serials[0]`.
- 0x408d00 (Q_progress.cpp): stage progress pass. It contains an inline copy of 0x408f60 *without* the percent parameter and without the
  final M_004090a0 call (that call is made by the caller for every row, which is why every exit of the inlined copy jumps to it). Modelled as
  `inline void Game::PruneStageSlots(int stage)` (declared in game_methods_Q.txt); it is not the same function as 0x408f60.
- 0x4078f0 (Q_script.cpp): stage script interpreter; switch on `DAT_004f065c[0]` (-100000..-99990), tracks 0..6; the 4th track (-99993) is
  the last case in source order (the jump table is sorted but the code layout follows the source order); `sprite = M_0040bf80(...)` is a
  local so that the call is evaluated before the other arguments.

## Parked (details in the OPEN notes)
- 0x40bfa0 (Q_open.cpp): text drawer with `@`-escapes. Both switch tables and every block match; only the loop test differs (the original
  does not forward the loaded `*text` into the body). Unknown characters are mapped with `c = ' '` (that gives the shared advance block).
- 0x40e570 (Q_open.cpp): 563 vs 560, register/schedule (row pointer kept in a stack slot, `group` not register-allocated).
- 0x40dd60 (Q_open_balloon.cpp, needs request 2): 81% similar, frame 0x24 vs 0x10, zero register in ebx, and the out-pointer for M_00401d70
  lives in the dead `text` parameter slot.
- 0x417b80 (Q_open.cpp): first transcription of the scaled blit (16.16 fixed point, wrap loops); the prologue load order does not match yet.

## Not attempted
00416f30 00417d20 00417530 004176f0 (blit family, same walls as lane I), 004189d0 (18-way loader, ~40 typed globals), 00409700, 00409e00
(float math), 0040a900 (float), 0040f3a0 (camera, needs a struct for DAT_008f1010), 0040d400, 004193c0, 00413ad0, 00415320.
Reason: time. 0x40bfa0-style data (jump tables) is handled by the comparer.

## Requests (declaration changes in other lanes' files)
1. game_methods_C.txt: `void M_0040e380();` -> `int M_0040e380();` (returns the number of loaded sprites; Q_sprite.cpp does not compile otherwise).
2. game_methods_C.txt: `void M_00410440(int index);` -> `int M_00410440(int index);` (0x40dd60 tests the result; Q_open_balloon.cpp).
3. game_prelude_B.h `Rec_87cbc8`: members m_2c..m_8c are one `int m_2c[0x19]` array (0x40b5c0/0x4093b0 index it with `(&p->m_2c)[i]`).
4. game_fields_B.tsv: `m_87cbfc` should be `int m_87cbfc[0x100]` (0x87cbfc..0x87cffc, indexed up to 0x7f by 0x40e380); m_87cc28/2c/df4 overlap it.
5. game_prelude_C.h: `DAT_008f1010` is a struct pointer (0x40f3a0 uses fields 0..0x24 and eight 0x1c-byte effect records at +0x28), not `int *`.
6. 0x408f60 (F_misc.cpp) and the inline body in 0x408d00 are different code; nothing to merge.

## Lessons (new)
- Loads in the prologue follow the declaration order of the initialised locals; ordering the initialisers like the original loads was the
  lever for 0x4093b0 and 0x4078f0.
- `f(g(a), b, c)` evaluates `g` last; an original that calls `g` first used a local (`sprite = g(); f(sprite, ...)`).
- A byte-index + dword jump table is generated for sparse cases; the code layout of the arms follows the *source order* while the table is
  sorted by case value.
- Setting the loop variable to the "default" value (`c = ' '`) lets VC6 thread the jump straight to the shared block instead of duplicating it.
- A stride-4 induction variable that is also divided by 4 keeps both (0x40b5c0).

## Tools (build\Q\, untracked)
q.sh (build + full diff), qd.py (full instruction diff using tools/compare_obj.py), qperm.py (declaration-order permuter), rd.py (read data
from tsh.exe), overrides.txt (private declaration patches applied to include/game.h for requests 1 and 2).
Build note: use `sh build/Q/q.sh Q_x.cpp`; the official `tools/build_all.py --file` works for Q_b5c0, Q_stage, Q_progress, Q_script.
