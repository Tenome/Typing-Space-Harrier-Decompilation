# Lane P report (round 3: 21 unattempted small/medium functions)

## Matched (byte-exact)
- 0x4120e0 + 0x412118 `FUN_004120e0` (src/P_lz.cpp): the LZSS token decoder. Ghidra split it into 56 + 114 bytes; the whole 170
  bytes are identical to the original. The comparer still prints DIFF only because functions.tsv lists the two halves
  (see Request 1). The winning change against the parked copy in C_open.cpp: in the copy loop, `dist -= copied` is
  written before `rest -= copied`, and the loop is a plain `for (i = 0; i < len; i++)`.
- 0x40bce0 `Game::M_0040bce0` (src/P_needs_requests.cpp): byte-exact once Requests 2-4 are applied (reset of the
  tile/palette tables: two int[256] and two short[0xa00] tables cleared, nine RGB palette entries from the bit pattern
  of the index, tile sizes of 6 tiles + one extra tile through M_0040be30). The tile loop is
  `for (i = 0; i < 6; i++, tile++)` (the step list order decides whether `inc ebx` comes before or after `add edi,4`).

## Not source (no code to write)
- 0x40c690 `thunk_FUN_0040ca80`: `jmp 0x40ca80`, incremental-link style thunk (already noted in report G).
- 0x41eea0 DirectSoundCreate, 0x41eea6 DirectDrawCreate: import-library thunks `jmp [iat]`.
- 0x41eeb0 `_alloca_probe`: CRT hand assembly, ORIGINAL-ASM.

## Open (src/P_open.cpp, each with an OPEN note)
All are members of the clipped/wrapped/edge-clamped read-back blit family of FUN_00415320; the drafts have the right
semantics and flow, and the dst = src - row - x + buf loop start of the original is reproduced by plain pointer loops
(`for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++)` with an `if (src >= srcRow + baseW) break;` inside).
So the "column index eliminated" wall noted in report I is not a source-spelling problem; what remains is register
allocation (class: register/schedule):
| address | mine | orig | note |
|---|---|---|---|
| 0x416200 | 340 | 326 | clip y, clip x, keyed (fuzzy 38.8) |
| 0x416570 | 255 | 247 | wrap y, clip x, plain (41.9) |
| 0x416670 | 317 | 296 | wrap y, clip x, keyed (48.0) |
| 0x416be0 | 229 | 243 | wrap y, clamp x, plain (44.2) |
| 0x416ce0 | 289 | 277 | wrap y, clamp x, keyed (49.8) |
Facts found (useful for whoever continues):
- The original recomputes `srcRow + baseW` inside the inner loop (baseW read from its stack slot, add the row pointer),
  keeps dstEnd (`dstRow + width`) in ecx, dst in edx, src in edi, dstRow in eax, srcRow in ebp, bufEnd in esi. Every draft
  spills one value more (the `srcRow + baseW` temp or dstEnd). A hoisted/local `srcEnd` is worse.
- Index-style inner loops (`dstRow[i] = srcRow[x + i]`) are far off (wider code).
- Declaration-order shuffles (120 random orders of the nine job fields and seven pointers) and the tools/perm permuter
  (about 2500 evaluations, 5 workers) stay at 77+ differing lines for 0x416570.
- The y variants: clip (`height += y; buf -= y * bufPitch; y = 0`, loop while both rows are inside), wrap
  (`y += (1 - y / baseH) * baseH` for y < 0, `y -= y / baseH * baseH`, `if (srcRow >= baseEnd) srcRow = base` per row),
  clamp (0x416ad0: row pointer clamped to [base, baseEnd - basePitch]). The x variants: clip, clamp
  (`src < row ? row : src >= row + baseW ? row + baseW - 1 : src`), wrap (0x4170a0, source pointer folded back to row).
- Job field map (ReadbackJob from game_prelude_I.h): 4 base, 8 baseW, 0xc baseH, 0x10 basePitch, 0x14 buf, 0x18 width,
  0x1c height, 0x20 bufPitch, 0x24 x, 0x28 y, 0x3c keys (int[256], 0 = copy the pixel).

Not drafted (read only; same family and same wall): 0x4167a0 (clip y, clamp x, plain), 0x4168b0 (clip y, clamp x, keyed),
0x416ad0 (clamp y, clamp x, keyed), 0x416e00, 0x4170a0 (wrap y, wrap x, plain), 0x4171b0, 0x4172e0, 0x4173f0,
0x417900, 0x417a20.

## Requests
1. functions.tsv: merge `00412118 FUN_00412118 114` into `004120e0 FUN_004120e0` (size 56 -> 170) and drop the 0x412118
   row, so the comparer reports MATCH for src/P_lz.cpp. Then delete the parked copy in C_open.cpp.
2. config/game_fields_B.tsv: `0x87cbfc 0x2c int m_87cbfc[0xb];` -> `0x87cbfc 0x400 int m_87cbfc[0x100];` (the table at
   0x87cbfc is cleared as 256 ints by 0x40bce0). m_87cc28, m_87cc2c, m_87cdf4 (config/game_fields_A.tsv) fall inside it:
   they become m_87cbfc[11], [12], [0xfd] (src/A_open.cpp uses them). 0x40bce0 itself uses m_87cbfc[11] and m_87cbfc[13].
3. config/game_fields_G.tsv: `0x87fbfc 4 int m_87fbfc;` -> `0x87fbfc 0x28 int m_87fbfc[10];` (ten colour entries; G_draw.cpp
   reads `m_87fbfc` four times as the value of entry 0, so those become `m_87fbfc[0]`).
4. config/game_fields_A.tsv: `0x87fc24 4 int m_87fc24;` -> `0x87fc24 0x40 int m_87fc24[0x10];` and
   `0x87fc64 4 int m_87fc64;` -> `0x87fc64 0x40 int m_87fc64[0x10];` (tile width/height tables; 0x40bce0 fills six
   entries of each); m_87fc28, m_87fc2c, m_87fc68, m_87fc6c become element 1/2 of those arrays (src/A_open.cpp).
   Until 2-4 are applied src/P_needs_requests.cpp does not compile; that is why it is a separate file.
New fields added in config/game_fields_P.tsv: m_87cbf0, m_87cbf4, the int[256] at 0x87cffc, the two short[0xa00] tables at
0x87d3fc/0x87e7fc, and the two ints at 0x87fca4/0x87fca8.

Tools used: build/fd.py (full diff of one function), build/var.py (variant batch scorer), tools/perm/perm.py.
