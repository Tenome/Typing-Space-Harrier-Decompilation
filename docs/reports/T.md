# Lane T report (round 4: clipped / wrapped / clamped read-back blits, 0x416200..0x417a20)

All winners are in `src\T_blit.cpp`; still-open drafts are in `src\T_open.cpp` (each with an `// OPEN:` note).
Scratch generator and search scripts live in `scratch\` (git-ignored): `bg.py` writes one function from three axes
(y mode clip/wrap/clamp, x mode clip/wrap/clamp, keyed or plain), `sc.py` compiles + scores one file in 0.5 s.

## Matched (byte-exact, plain source)
0x416200 (clip y, clip x, keyed), 0x4167a0 (clip y, clamp x), 0x4168b0 (clip y, clamp x, keyed), 0x416570 (wrap y, clip x),
0x416670 (wrap y, clip x, keyed), 0x416ad0 (clamp y, clamp x, keyed), 0x416be0 (wrap y, clamp x), 0x416ce0 (wrap y, clamp x, keyed).
The lanes' earlier "wall" of this family was a source-spelling problem after all. The winning spelling:

1. The per-row limits are named locals assigned at the top of the row body, `dstEnd` first, then `srcEnd = srcRow + baseW`
   (the clamp/clip tests read `srcEnd`, `srcEnd - 1`); the inner loop is `for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++)`.
   A bare `srcRow + baseW` in the loop makes VC6 strength-reduce it into a second row induction variable and spill one more value.
2. The row loop initialises both cursors in the for-init: `for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd && srcRow < baseEnd; dstRow += bufPitch, srcRow += basePitch)`
   (assigning `srcRow` before the loop swaps two entry copies).
3. `baseEnd` is computed before `srcRow` (stack slot order); for clamp/wrap y modes `bufEnd` is assigned after the x normalisation,
   immediately before the loop (declared uninitialised), for the clip y mode it is the initialiser (it is read before `height += y`).
4. Clamp y: `if (srcRow < base) row = base; else if (srcRow >= baseEnd) row = baseEnd - basePitch; else row = srcRow;` (block order of the original).
5. Keyed: `if (keys[*from] == 0) *dst = *from;` (a `c` local spills; `char c` is wrong).
6. x clamp is `src < srcRow ? srcRow : src >= srcEnd ? srcEnd - 1 : src` as if/else chain on `from`; x clip is `if (src >= srcEnd) break;` in the loop.
7. Wrap y: `if (y < 0) y += (1 - y / baseH) * baseH; if (y >= baseH) y -= y / baseH * baseH;` then `if (srcRow >= baseEnd) srcRow = base;` per row.

The levers suggested at the start: `while (1)` loop forms, struct-pointer-only access, ternary vs if, switch dispatch gave nothing
(all spellings compile to the same code once the locals above are right); the decisive ones were 1-3.

## Round 5 additions
Matched: 0x416350, 0x416440 (clamp y, clip x, plain/keyed), 0x416100 (ClipJob, clip y, clip x), and all five wrap-x helpers
0x416e00, 0x4170a0, 0x4171b0, 0x4172e0, 0x4173f0. The wrap-x ones needed the same per-row `srcEnd = srcRow + baseW;` local
(the inner test is `if (src >= srcEnd) src = srcRow;`); my first drafts read `srcRow + baseW` inline and hit a register-naming wall.
Clip y with wrap x: x is normalised after the clip block and the early `return`; `bufEnd` is the declaration initialiser only for clip y.
Total in `src\T_blit.cpp`: 16 functions.

## Open
| address | best | note |
|---|---|---|
| 0x417900 | 284 = 284 B, 46-60 % | scaled 16.16 wrap blit (draft in the scratch notes: stepX/stepY = (m_2c/m_30 << 16) / width/height, fx/fy `while (f >= limit) f -= limit`); the original keeps `width` in memory for the first idiv |
| 0x417a20, 0x417b80, 0x417d20, 0x417530, 0x4176f0 | not drafted | scaled / distortion blits (12-byte record table in 0x417530) |
| 0x414830 | 390 vs 416 B, 22 % (parked copy 38.7 %) | four-variant copy; the original counts rows as `dec esi; test edi,edi; jbe` then `lea edi,[esi+1]` ... `dec edi; jne`, flags kept in ebx; for/while/do forms tried |
| 0x4149d0, 0x414bd0, 0x414f00 | not retried | BlitJob family |
| 0x413ad0, 0x415320 | not attempted | dispatchers (3267 / 3552 B) |

Not source (ignored): 0x40c690, 0x41eea0, 0x41eea6 thunks; 0x41eeb0 `_alloca_probe` is ORIGINAL-ASM.
