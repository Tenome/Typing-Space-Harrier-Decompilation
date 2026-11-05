# Lane V report (round 5: 0x413ad0, 0x415320, 0x417530, 0x4176f0)

**Bytes-matched: 0 of 4.** Everything is parked in src\V_open.cpp (new files only: src\V_open.cpp, config\game_prelude_V.h, this report).

| address | best | note |
|---|---|---|
| 0x417530 | 447 = 447 B, 87.4 % | distortion blit; same code except prologue load order, three spill slots (width/basePitch/base) and the x-wrap register choice |
| 0x4176f0 | 465 vs 503 B, 51 % | keyed twin (`c = pixel; if (keys[c] == 0) *dst = c;`); original keeps keys in ebp and reloads the wrap limit each iteration |
| 0x413ad0 | 3392 vs 3267 B, ~33 % | complete transcription (clip with 4-bit fixed-point scaling, colour-key table, switch over 0,1/2,4/8,5/6/9/10, Lock retry loops, Blt/BltFast) |
| 0x415320 | not drafted | switch on `flags & 0x800f0000` with ~70 callees (all T-family clip helpers, 0x417530/0x4176f0/0x417b80/0x417a20/0x417900/0x417d20); too large for the effort |

## Model recovered
- DistortJob / DistortRow (config\game_prelude_V.h): rows of 12 bytes `{lineShift, rowShift, divisor}`; step = (width << 16) / divisor; source row = lineShift + y wrapped
  into baseH, start x = x - (rowShift * step >> 16) wrapped into baseW; inner loop walks a 16.16 position wrapped at baseW << 16.
- BlitCommand (derived from BlitRequest): flags bits 0-1 destination kind (0 image, 1 front surface, 2 back surface), bits 2-3 source kind (0 image, 4/8 surfaces),
  0x10 explicit source rect, 0x20 scaled, 0x40/0x80 mirror, 0x100 rotation, 0x200 blend variant, 0x400 colour key range. Image kinds are SheetImage (+0x50 source, +0x54 destination).
- Findings: the original reads `request->flags` directly everywhere (CSE temporaries at [esp+0x30]/[0x4c]/[0x88]/[0x8c] are compiler made); the clip scaling is
  `((n << 4) * (d << 4) / (s << 4) + 15) >> 4`; the key table loop compares the three RGB bytes of the palette entry with the low/high key bytes.

## What was tried on 0x417530
Hill climbing over declaration order (scratch/climb.py, ~1500 compiles) reached 447 B at 87-90 %. Spelling variants of the x wrap (`x -= (x / baseW - 1) * baseW` gives 443 B / 92 %).
Inline `baseW << 16` / `width << 16` is not hoisted by VC6 (424 B); named locals are needed for the 0x2c/0x30 spill slots. The original load order (0xc,4,8,0x18,0x38,0x14,0x10,0x20,0x28,0x1c,0x24)
is not reproduced by any declaration order tried.
