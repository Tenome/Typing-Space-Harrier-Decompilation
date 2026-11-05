# Lane U report (round 4; lanes\U.tsv, 14 large never-matched functions)

**Bytes-matched: 0 of 14. Matched under the rules: 0. MATCH-PENDING: none.** Three drafts parked with OPEN notes, eleven not attempted.
New files: config\game_prelude_U.h, config\game_methods_U.txt, src\U_open.cpp, src\U_open_409e00.cpp, src\U_open_40f3a0.cpp.

## Parked
- 0x409e00 (U_open_409e00.cpp): 756 vs 754 bytes (53% instruction similarity, every block present, same 0x140 frame). Facts: `slot` is an
  `int[0x40]` of which only 0x1d entries are zeroed (the frame size only fits that); `slot[0..1]` are overwritten by the `M_00409300` results
  and `slot[0..3]` feed the two `M_0040b0e0` calls per pair; the three record loops run over `(&m_87cbc8->m_2c)[0..24]` and a 0x40-entry
  loop at record index 0x24 (first 0x1d from slot, rest 0); `M_004096b0` is called with `this` in ecx, so it is a method (declared in
  game_methods_U.txt) although F_open.cpp has it as a free `__stdcall`. Residue: the original stores the `M_00409300` result into slot[i]
  between the argument pushes and keeps the pair id in edi; the draft keeps a second stack home for the length (register/schedule).
- 0x40f3a0 (U_open_40f3a0.cpp): 856 vs 866 bytes (50%). Camera/effect update. Typed views `CamState`/`CamEffect` (8 effects of 0x1c bytes at
  +0x28, see game_prelude_U.h) are declared as `G_008f1010` etc. (the comparer accepts `G_<hex>` names) because DAT_008f1010 is an `int *` in
  prelude C (request 5 of report Q). Original keeps `this` and the slot-search counter in stack slots and shares one zero register in the
  switch arms.
- 0x416f30 (U_open.cpp): 334 vs 357 bytes. Clip y, wrap x, keyed read-back copy of the blit family. Findings beyond P.md: the original loop has
  the extra test `srcRow < baseEnd`, the wrap source end is `src + (baseW - x)` (a `run` local), but the frame (0x28 vs 0x20) and prologue
  order remain; same register/schedule wall as the other lanes' copies.

## Not attempted
0x40d400, 0x40a900, 0x40dd60 (see Q_open_balloon.cpp), 0x4189d0, 0x409700, 0x4193c0, 0x413ad0, 0x415320, 0x417d20, 0x417530, 0x4176f0.
Reason: each needs callee signatures and typed tables (and the blit family hits the register wall); the budget went to the three drafts above.

## Tools (build\U\, untracked)
b.sh (build one file + full diff), fd.py (full instruction diff against the original, with similarity ratio; `build_all.py --verbose` prints
only the first 12 differing lines).
