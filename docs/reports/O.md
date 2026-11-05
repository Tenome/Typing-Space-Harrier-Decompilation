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
| 0x41d950 | `FUN_0041d950` | O_e3.cpp | `for (slot = 0; slot < 3; slot++) { if (FUN_004189a0(slot + 1) == 0) break; }` instead of the hand-peeled for(;;) |
| 0x406a50 | `Game::M_00406a50` | O_a3.cpp | `for (i = 0; i < 0x100; i++)` with `DAT_00a3d6ec->m_0` tested through the stored pointer; `notFound = DAT_00430658` cached before the loop |
| 0x404040 | `Game::M_00404040` | O_a4.cpp | `if (M_00406660() != 0) expired = 1; if (expired) {reset} else {counter}` instead of `done != 0 || expired` |
| 0x41cb30 | `FUN_0041cb30` | O_e1.cpp | walk the `text` parameter itself (no `p`/`alpha` copies), four plain out locals, `while (*text != 0)`, SJIS lead test `(*text & ~0x1f) == 0x80` |

MATCH-PENDING: none (plain source everywhere).

Bytes identical but reported DIFF only because of the function inventory: 0x4120e0 (`src\O_c2.cpp`). `config/functions.tsv` lists it as 56 bytes plus a separate 0x412118 (114 bytes); the compiled function is the 170-byte merge (Ghidra split it, see docs/DECOMP_WORKFLOW.md), the comparer says `size 170 vs inventory 56` with no byte difference. Fix: merge the two rows in functions.tsv (as was done for 0x4089a0). Winning change: `dist -= copied;` before `rest -= copied;`.

## Levers confirmed this round
- Loop exits written as `while (1) { ...; if (c) break; }` (412800, 413350) -- same as lane M.
- Invariant subtraction hoisted into its own local before the loop (`rowSkip`), declared where the original computes it.
- A pointer cursor that is stepped separately from a counter (`*out++`) can be replaced by indexing with the counter (409270).
- `switch` with the `default` label first reproduces the original "fall-through block, then case blocks in source order" layout (40f8a0).
- Dropping helper locals (`p = text`, `alpha = fade`) that only exist to carry the parameter often fixes the prologue (41cb30).
- A parked copy that is the wrong size usually has a missing statement (413430 terminators, 4147c0 fourth byte, 41a820 `-2`): count bytes first.

## Still open (best state; wall class)
| address | best | wall class / what is known |
|---|---|---|
| 0x41ad10 | 7 diff lines | register/schedule: original stores through `shl eax,3; [eax+abs]`, VC6 folds `[eax*8+abs]` for every spelling (int/unsigned/short slot, 2-D `[row][col]`, pointer to element); permuter best 5 |
| 0x4089a0 | 286 vs 290 B | inline/call set: original pushes three dead dwords (0,0,1) before the strcat run and cleans `add esp,0x24` once; nothing in the source explains them |
| 0x4081b0 | 312 B, 5 diff lines | masked/referent-like: strength-reduced base is `&row.m_10` in the original, mine `m_0` (row pointer spelling gives `m_1c`); permuter 3 min no change |
| 0x4119a0 | 410 vs 383 B | CFG: VC6 duplicates the shared read-tail (`m_14=0; m_10=0; m_18=size; m_1c=size; return 1`) into the file branch; the original jumps (`jmp tail`); goto form works for the write tail only |
| 0x4189a0 | 42 vs 44 B | masked: original keeps `movsx` of the byte flag before `& 1`, VC6 narrows to `mov al`; tried int local, signed char, casts, `== 1`, pointer |
| 0x4043d0 | 187 vs 189 B | register/schedule: original loads m_8ac into eax before the compare and walks an absolute-address pointer (signed `jl`); int and pointer loops tried; the table is declared `int[8]` in lane A (a `{x,y}[4]` view would need a new declaration) |
| 0x401da0 | 83% | evaluation order: original loads the value argument after the two `lea`s; local/param-slot spellings and permuter (60 s) fail |
| 0x40fab0 | 391 B, many diffs | register/schedule: original keeps `x` in ebx up to the loop and spills it there |
| 0x411e80 | 443 vs 435 B | CFG: failure ladder placed after the success block; goto and fail-inside-if spellings tried |
| 0x4180f0 | 1197 B equal size | register/schedule: hwnd hoisted to edi in the `Play ... Notify` loop in the original; permuter 4 min best 14 (parked copy score kept) |
| 0x4071c0 | 108 B | register: zero constant hoisted to esi (mine) vs fresh `xor ecx,ecx` after the idiv (original); permuter 2 min no change |
| 0x410ea0 / 0x410f40 | 43 vs 45 / 59 B | class wall: original builds the group-slot pointer (`lea edx,[esi+0x18]` / `add edi,0x18`) before the unrolled clears; index/pointer loops tried |
| 0x410460 | 115 B | register swap i/p (eax/ecx) in the counting loop; 8 spellings tried |
| 0x409ce0 | 34 vs 35 B | original loads the argument into eax (`mov eax,[esp+4]; push eax`) where VC6 emits `push [esp+4]` |
| 0x41cc80 | 230 vs 235 B | register allocation (ebp/esi/edi/ebx), dx/dy half-step temporaries; permuter 1 min best 12 |
| 0x41ce30 | 311 vs 304 B | register allocation and loop form (not retried after the simplifications that fixed 41cb30) |
| 0x412b90 | 732 vs 738 B | register/stack slot order of the colour temporaries |
| 0x406490 | 168 vs 166 B | register allocation (menu pointer in eax, count spilled) |
| 0x403570 | 242 vs 219 B | register pressure (ebp=this, esi caches DAT_00430658) |
| 0x40a6d0 | 205 vs 209 B | this in esi/zero constant hoisted to the function top in mine vs inside the branch in the original |
| other lane-O entries (40d160, 40f720, 403940, 416350, 416440, 408660, 407310, 40ff90, 40fd10, 404... etc.) | not retried this round | |

