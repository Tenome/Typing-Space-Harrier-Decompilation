# Lane I report (0x416200-0x4193c0, 26 functions)

Bytes-matched: 0 of 26. Matched under the rules: 0. MATCH-PENDING: none.
Files: game_prelude_I.h (types), src\I_open.cpp (4 parked near-misses). Flags: default /O2.

## Parked near-misses (src\I_open.cpp, each with an OPEN note)
- 00418f70 (stage setup, 913 B): 912 vs 913, flow identical, only push order/register naming differ. Model: four region
  structs (StageData 0xf4c328 incl. the 0xa8-byte StageRec, OverlayData 0xa4b168, LayerData 0x101de48, StageTable 0xfb1d70)
  because the original's loads/reloads only fit field-sensitive aliasing inside one object; struct assignments give the
  rep movsd copies; `n` local = extra count, per-field reads elsewhere. Best next step: register naming (ebp pushed late).
- 00416350 (clamped row copy, 239 B): 241 vs 239. VC6 strength-reduces `buf + w` (hoisted lea + add per row); original
  recomputes it per row and keeps w live (one more spill slot). Tried: for/while/do shapes, dstEnd local/inline/block scope/const,
  dstRow copy (removes hoisting but adds a store), int/unsigned w, `row + w` helper, increments in for-step. Wall: register/schedule (iv SR).
- 00416440 (keyed variant, 303 B): 292 vs 303. 004169f0 (x-clamped row copy, 221 B): 225 vs 221.
- 00416200 (keyed, 326 B): best draft 328 (not saved; see below).

## Finding for the family (0x416200-0x417d20)
All variants with a source-x window (416200, 416440, 416670, 4167a0, 4169f0, 416be0, ...) compute the destination pointer as
`src0 - row - x + buf` and the source limit as `src0 + (baseW - x)` or `row + baseW`: the signature of a column-index loop
`for (i = 0; i < w; i++)` whose index was eliminated in favour of two pointer induction variables (loop test `dst < buf + w`,
guard `buf < buf + w`, comparisons on `x + i` turned into pointer compares against row/row+baseW). Index-style source
(`buf[i]`, `row[x+i]`) keeps the index in VC6 (wider code); pointer-style source (`*dst++`) yields d0 = buf. Neither gave the
original; the exact spelling that makes VC6 eliminate i but start d from s is still unknown. Non-window variants (416350,
416570) are plain pointer loops and fail only on the `buf + w` strength-reduction residue.
Vertical wrap variants (416570, 416670, 416be0) use `y % baseH`-style wrap: `y += (1 - y / h) * h` for y < 0, then `y -= (y / h) * h`.

## Not attempted (no draft written)
416570, 416670, 4167a0, 4168b0, 416ad0, 416be0, 416ce0, 416e00, 416f30, 4170a0, 4171b0, 4172e0, 4173f0, 417530, 4176f0,
417900, 417a20, 417b80, 417d20 (scaled/wrapped/16- and 32-bpp blit siblings; 417d20 = scaled keyed blit with 16.16 fixed
point and `%`-free wrap loops), 4189d0 (1353 B, stage table loader: 18-way switch writing StageTable.rec[i] fields from
tables at 0x4ebd68/0x4ebe90/0x4ec118/0x4ec160..., now typed by StageTable/StageRec in the prelude), 4193c0 (2728 B).
Reason: time; they share the walls above (register/schedule, iv elimination).

## Tools
Scratch in build\I\ (b.sh build, dd.py side-by-side diff, perm.py/permdecl.py/variants.py/grid350.py search harness).
No tool problems. Prelude warnings about duplicate method declarations come from other lanes.
