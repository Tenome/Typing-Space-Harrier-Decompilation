# Lane W report (round 5; lanes\W.tsv, 6 large functions)

**Bytes-matched: 2 of 6 (0x40d400, 0x40a900).** Parked: 0x40dd60 (88.6 %), 0x409700 (71.8 %), 0x4189d0 (49.9 %); 0x4193c0 not attempted
(2728 B, 0x834 frame, blit engine; needs its own data model first).
New files: src\W_group.cpp, src\W_a900.cpp, src\W_open.cpp, config\game_prelude_W.h, docs\reports\W.md.

## Declaration edits in existing files (one word each, needed to build; please keep them when merging)
- config\game_methods_G.txt: `void M_0040d400(...)` -> `int M_0040d400(...)` (returns the last spawned id, `mov eax,ebx` at the end).
- config\game_methods_C.txt: `void M_00410440(int index)` -> `int M_00410440(int index)` (0x40dd60 tests the result; request 2 of report Q).
- config\game_methods_B.txt: `void M_00409700(int a, int b, int c)` -> `int M_00409700(int a, int b, int flag)` (returns 1 / DAT_00430658 / 0).

## Matched
- 0x40d400 (W_group.cpp): word-group picker, called by the group runner 0x40ca80 with n = 0..3 (flag 0) and 1..5 (flag 1). Picks `(n+1)` entries from the
  bank (`pick * (n+1)` base), longest word length via M_0040c970, shuffles an index order (`rand()%(n+1)+n` swaps of positions 1..n), spawns each through
  M_0040dd60, lists the ones with `flag ? v > 0 : v == 1`, then runs the second pass that sets slide-in timers. `order[0x21]` fixes the 0x98 frame
  (29 would round down), `word = ...m_14[base + i]; len = M_0040c970(word)` (a named pointer local moves the load into eax), `entry = v + base` gives the
  scaled addressing, `if (flag != 0) { if (v > 0) listed = 1; } else if (v == 1) listed = 1;` is the block shape of the original.
- 0x40a900 (W_a900.cpp): stage object draw + action code. `switch (action)` has 60 sparse cases (-810175..-810040, jump table with byte index) mapped to
  10..69 plus -800005/-800000/-800001/-800002; case order in the source follows the arms in the binary. `tileEnd = tileBase + 0x26;` computed right after
  the call (lea before the tests), `goto skipDraw` for the blink skip (nested if/else form does not match), `m_87cbc8->m_14 == 0 && (m_a795c == 4 || 3 || 8)`.
  `tile` is the parameter slot reuse (out param of M_0040b4d0 key 3).

## Parked (OPEN notes in src\W_open.cpp)
- 0x40dd60: 88.6 %, same length. The frame (0x24) only fits with the box metrics in one `int box[7]` (textW, textH, two option sizes, three slots nothing
  reads). As scalars VC6 reuses the dead `reading`/`weight` parameter slots for textW/textH (frame 0x10); dead locals get no slot. `textRows >= 6` fixes a jl/jle.
  Left: zero register ebx vs esi (register/schedule). Permuter 4 min best 54.
- 0x409700: 71.8 %. Whole flow transcribed (uses block-wide `buf[0x40]` of M_0040bbe0 ids; the flag path has the 1/1024 fixed point fan over DAT_004e246c).
  Slot layout of the scalars and the register holding x differ. Permuter 5 min best 69.
- 0x4189d0: 49.9 %. Loader fully typed (StageRec 168 B records, 16-byte KeyImageIds, 36-byte KeyPartIds, 12-byte KeyPartRamp, Rgb ramps of lane K,
  limit in the dead `path` slot already matches). Original keeps i in edi, i<<4 in ebp, length reloaded into ebx, and the constant 1 in ecx across the switch.

## Lessons
- Dead parameter slots are reused for *spilled scalars* too (not only address-taken locals); an array local keeps its own frame slots. Frame size of a
  function that only differs by N*4 usually means the real source had a bigger array/struct (0x40dd60: +3 slots).
- `dec`-counted do/while loops over a `rand()`-driven count: `for (i = 0; i <= k; i++)` with k = rand()%m + n gives `add; js; inc; ... dec; jne`.
- A named pointer local (`word = tbl[i]; f(word)`) can change which scratch register the load uses.
- Tools: scratch\fd.py (full instruction diff with relocation operands masked in my side only) in the worktree (untracked).
