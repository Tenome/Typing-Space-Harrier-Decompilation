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

## Open
| address | state | wall |
|---|---|---|
| 0x416e00, 0x4170a0, 0x4171b0, 0x4172e0, 0x4173f0 | right size or within 8 B, 43-68 % | wrap-x family: x/baseH (or x/base) get swapped registers (esi/edi, ebp/edi); permuter 6 min x2 no change |
| 0x417900 | 284 = 284 B, 46 % | scaled 16.16 wrap blit, draft in T_open.cpp; original keeps `width` in memory for the first idiv |
| 0x417a20 | not drafted | scaled keyed sibling of 0x417900 |

Not source (ignored): 0x40c690, 0x41eea0, 0x41eea6 thunks; 0x41eeb0 `_alloca_probe` is ORIGINAL-ASM.
