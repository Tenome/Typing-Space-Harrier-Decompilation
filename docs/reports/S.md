# Lane S report (round 4, rewrite of the far drafts)

Files: `src/S_bits.cpp`, `S_bank.cpp`, `S_typed.cpp`, `S_sheet.cpp`. No new declarations were needed. Tools used: `tools\build_all.py --file`, `tools\perm\perm.py`.

## Matched (clean, reader test passed)

| address | function | file | what decided it |
|---|---|---|---|
| 0x4121a0 | `FUN_004121a0` (bit reader) | S_bits.cpp | success path first: `if (v != -1) mask = 0x80; else return -1;` then the shared `store mask; return (buf & mask) != 0`; `mask` is a local copy of the global |
| 0x405bd0 | `Game::M_00405bd0` | S_bits.cpp | `count`/`last` assigned after the three clears, `last = 0` before `count = 0`; `DAT_00a23304` computed before `DAT_00a23300 = last + 1` |
| 0x40c9f0 | `Game::M_0040c9f0` (word bank parser) | S_bank.cpp | modelled as a two-state machine: `switch (state)` with `case 0` (reading) / `case 1` (text, `count++`), `lineStart` flag, `for (i = 0; i < bank->m_00[2]; i++, p++)` with `*p = 0; break;` on 0x1a (do/while and `while (1)` both lay the tail out wrong) |
| 0x410760 | `Game::M_00410760` | S_typed.cpp | the hand-written word-clearing loop is `memset(DAT_008f544c, 0, sizeof(TypedRec))` (VC6 inlines it as `rep stosd`) |
| 0x412ec0 | `FUN_00412ec0` | S_sheet.cpp | permuter found the local declaration order (surf first, `unsigned i`, `i != 256`); green component is `(unsigned char)(bg >> 8)` not `GetGValue` (`mov dl,ah` vs the original `shr edx,8`) |

MATCH-PENDING: none.

## Levers confirmed
- `if (c) {success} else return X;` ordering fixes which block is placed first (4121a0).
- A hand-rolled `for (n--) *p++ = 0` over a struct is `memset` of the struct (410760).
- State variable with `sub ecx,ebx; je; dec ecx; jne` is a `switch (state)` (40c9f0).
- `GetGValue` compiles to `mov dl,ah`; `(unsigned char)(c >> 8)` gives `shr` (412ec0).
- `unsigned char` widened into an `int` that is also needed as byte (4136f0): VC6 stores the byte to the stack and reloads it as a dword & 0xff (this is VC6's way to widen from `bl` to `edi`), so the spill is not a hint of an inline helper.
- Failure paths that set the accumulator to -1 and `break` (return the accumulator) share the loop-exit `mov eax,esi`; an early `return -1` before the accumulator exists is separate (4136f0 layout).
- Strength-reduction base: the member that the original keeps as the loop pointer is not the first one in source; see 406910/406a80 below.

## Still open (best state, wall)
| address | state |
|---|---|
| 0x4136f0 | structure now equal (133 B region: `if (i & 1) {odd} else {even}`, `base = -1; break;`, `return base`); remaining: `code`/`base` registers swapped (esi/edi) and `and al,0xe0; cmp eax,0x80` (mine `and eax,0xe0`). Types short/char/long/unsigned for `code` tried |
| 0x413870 | same pointer-induction wall (orig keeps `esi = &T[i]` plus `ebx = i`), code kept in `edx` |
| 0x4121f0 | loop and tail equal when written `top = 1; while (top * 2 < count) { top *= 2; value = bit() + value * 2; } top *= 2;`; register naming (count in ebx, top esi, value edi in the original) differs; all declaration orders and 3 min permuter (best 20) fail |
| 0x41adc0 | induction base fixed by reading the key once into `entryKey`; only register naming differs (key in al, name in ebx in the original); permuter best 30 |
| 0x406be0 | permuter best 10; original idx=eax, p=edx |
| 0x4013b0 | structure right (`push esi` late, `mov esi,eax` inside), original keeps `eax` as the cursor and copies it to `esi` once; 8 spellings tried |
| 0x40bbe0 | original keeps a live zero in ecx (`mov eax,ecx` on the limit<=0 path); a `found = 0` local is constant-folded to `xor eax,eax` |
| 0x411340 | draft lacked the WAVEFORMATEX/DSBUFFERDESC setup (the 128 byte gap); aggregate initialisers `DSBUFFERDESC desc = { 0x14, DSBCAPS_PRIMARYBUFFER };` declared in the middle of the function and `WAVEFORMATEX format = { WAVE_FORMAT_PCM, 2, 22050, 44100, 2, 8, 0 };` give the original store pattern; remaining: the original duplicates the epilogue for every `return 0` (7 copies) where mine shares one, and keeps no hoisted zero |
| 0x406910 / 0x406a80 | strength-reduced pointer is `&row.m_8` / `&row.m_28` in the original; mine picks `m_0` / `m_24`. `<= 0` instead of `< 1` is right |
| 0x40cdd0 | zero kept in esi from the first store on (probably the loop counter `i = 0` hoisted); pushes of esi/edi after the early return; `int i = 0;` after the `if` did not reproduce it |
| 0x40a6d0 | original materialises zero in edi before `cmp stage[0xc], 0` |
| 0x40d160 | original caches constant 1 in ebx and keeps `total` in a stack slot |
| 0x4169f0 | clamp structure equal, 226 vs 221 B; pointer-induction wall |
| 0x416100 | not reached (ClipJob clip arithmetic with stack-slot reuse) |
| others in `config/lanes/S.tsv` | not attempted: blit/DirectDraw/MCI/UI functions above 400 bytes (0x414830, 0x4149d0, 0x414bd0, 0x414f00, 0x4129a0, 0x412fe0, 0x4114d0, 0x418f70, 0x4185a0, 0x41a880, 0x419e80, 0x41aef0, 0x402310, 0x402730) and the small walls already documented in O.md |

Permuter results this round (3 min, 6 workers each): 41adc0 30, 406be0 10, 4121f0 20, 40bbe0 13.

## Session 2 (after merging master; T and R levers retried)
Matched: 0x4169f0 (`src/S_read.cpp`): lane T's spelling (named `row`, `dstEnd`, `srcEnd` locals per row, both cursors in the for-init, `baseEnd` before `srcRow`) gave the byte match first try.
Retried without a match:
- 0x413870: pointer cursor `p` in the for-init next to `i` (`for (i = 0, p = DAT_004ebb34; p < DAT_004ebb34 + 0x48; i++, p++)`, `base += end - p[-1]`) reproduces the original esi/ebx induction pair (126 vs 130 B); all 120 declaration orders still 126; code/base register naming (edx/eax in the original) is what remains.
- 0x406910: a named `obj = &DAT_009a629c[k]` local (assigned, then `DAT_00a3d6ec = obj`, then `if (DAT_00a3d6ec->m_0 != 0 && obj->m_8 > 0)`) moves the strength-reduced pointer to `m_8` as in the original; `count = m_0` first and `n > count` fix the head. Remaining 320 vs 309: the original reloads `DAT_00a3d6ec` after the `m_8 -= n` store (store through a pointer VC6 cannot prove distinct) while mine forwards the pointer register; `int *life` (from obj or from the array) did not break the forwarding.
- 0x40a6d0: `memset(&m_3d8[5], 0, 16)` gives 191 B (the original has four separate reloading stores), so no.
- 0x40d160: named `row = &DAT_004bde14[index * 0x16]` local: 337 vs 338 B (from 335), still no hoisted constant 1 in ebx.
- 0x408660 (sound remap switch): writing the case values into the parameter `sound` instead of an `id` local changes nothing in the head; the original keeps `this` in esi (copied at entry because ecx is reloaded for `DAT_00a45e28`) and the parameter in eax; mine keeps the parameter in esi.
- 0x4136f0, 0x4121f0, 0x41adc0, 0x406be0, 0x40bbe0, 0x4013b0, 0x406a80, 0x40cdd0: no change from the first session (no invariant sub-expression to name; no zero-run to turn into memset; 0x40cdd0 fills with -1 so memset does not apply).
