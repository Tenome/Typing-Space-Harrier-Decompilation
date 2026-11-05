# Lane O report (round 3 near-miss matching)

Tools: `tools\build_all.py --file O_x.cpp --verbose`, `tools\perm\perm.py`. All wins are in new files `src\O_*.cpp`
(the parked copies in `*_open.cpp` are untouched; delete them in a cleanup commit).

## Matched (clean, reader test passed)

| address | function | file | change against the parked copy |
|---|---|---|---|
| 0x409270 | `Game::M_00409270` | O_stage.cpp | output cursor `out++` replaced by `m_8eccc8[found] = cur; found++` (strength reduction then builds the pointer after the loop-entry test) |
| 0x413430 | `FUN_00413430` | O_glyph.cpp | the original terminates the glyph text: `text[1] = 0` after the single-byte lookup, `text[2] = 0` after the double-byte one; no `cellH` local (`sheet->cellH = font->lfHeight` twice); `count` computed after `halfCellW` |
| 0x41a820 | `FUN_0041a820` | O_misc.cpp | `strcpy(dir, GetCommandLineA() + 1)`; last-character pointer is `dir + len - 1` (parked copy had `- 2`) |
| 0x4066d0 | `Game::M_004066d0` | O_a1.cpp | no temporaries: `m_3a0 += n;` and `m_3ac + m_3a8` used directly |
| 0x4147c0 | `FUN_004147c0` | O_d1.cpp | the palette comparison covers all four bytes (r, g, b and flags) |
| 0x412800 | `FUN_00412800` | O_flip.cpp | `while (1) { ...; if (result != DDERR_WASSTILLDRAWING) break; }` (the Flip retry loop) |
| 0x413350 | `FUN_00413350` | O_lock.cpp | Lock retry written `while (1) { hr = Lock; if (hr == DD_OK) break; if (hr == SURFACELOST) return 0; if (hr != WASSTILLDRAWING) return 0; }` and then the copy; `rowSkip = lPitch - dwWidth` computed once before the row loop |
| 0x4111f0 | `SoundMgr::FUN_004111f0` | O_snd.cpp | result is the parameter: `if (buf != 0 && buf->SetFrequency(freq) != 0) freq = 0; GlobalUnlock(); return freq;` |
| 0x40f8a0 | `Game::M_0040f8a0` | O_c1.cpp | outer `switch (DAT_00a3d6f0)` with `default` first, then `case -1`, `case -2` (block order of the original) |
| 0x41cb30 | `FUN_0041cb30` | O_e1.cpp | walk the `text` parameter itself (no `p`/`alpha` copies), four plain out locals, `while (*text != 0)`, SJIS lead test `(*text & ~0x1f) == 0x80` |

MATCH-PENDING: none.

## Levers confirmed this round
- Loop exits written as `while (1) { ...; if (c) break; }` (412800, 413350) -- same as lane M.
- Invariant subtraction hoisted into its own local before the loop (`rowSkip`), declared where the original computes it.
- A pointer cursor that is stepped separately from a counter (`*out++`) can be replaced by indexing with the counter (409270).
- `switch` with the `default` label first reproduces the original "fall-through block, then case blocks in source order" layout (40f8a0).
- Dropping helper locals (`p = text`, `alpha = fade`) that only exist to carry the parameter often fixes the prologue (41cb30).
- A parked copy that is the wrong size usually has a missing statement (413430 terminators, 4147c0 fourth byte, 41a820 `-2`): count bytes first.

## Still open (best state; wall class)
Filled in at the end of the round (see the table below).
