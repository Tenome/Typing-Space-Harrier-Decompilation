# Lane A report (0x401000-0x406d20)

Counts (last full rebuild): see the final message; source files: A_game, A_boot, A_slot, A_text, A_field, A_stagectl,
A_util, A_menu, A_player (matched) and A_open (open/pending, each with an `// OPEN:` note and a first-draft body).

## Matched functions (byte-exact)
All functions in the lane not listed below as open/pending. 2D menu cursor (Menu, DAT_00951fcc), typed-key matching
(402c50/402be0), stage/title screens (403cf0, 404000, 404110, 404940), table builders (404a90), file helpers
(405200/405290), slot helpers (401930-401a40, 4016b0), player-slot setup (406c40-406cb0) etc.

## Counts
Bytes-matched: 88 of 119. Matched under the rules: 87 (406350 stays byte-exact but is counted out: it carries a RULE-EXCEPTION
for the interior-pointer null test). 402e90 and 406c90 are now clean: `int m_2894[0xa00]` is a real Slot member and
Obj_008f1000 holds `Player m_players[4]` (Player now comes from game_prelude_00.h); SLOT_QUEUE and the cast are gone.

## Open (bytes differ; each in src/A_open.cpp with an // OPEN: note and a near-complete body)
- register/schedule class: 401da0, 402200, 402310 (tint/count share a stack slot), 402730 (dead parameter slots reused),
  402960, 402a80, 403570, 403940, 403d80, 4042a0, 405220, 405b80, 405bd0, 405c50 (same size), 405fe0, 406490, 406540,
  406be0, 4052e0 (1035 bytes: one extra `mov esi,edi` in the cursor update, rest aligned).
- CFG/layout class: 404040 (reset block placed first), 403650, 4043d0, 4066d0, 4068a0, 406a50, 406910, 406a80,
  404490 (1064 vs 1050: switch layout right, orig keeps case 1's tail un-merged).
- known walls kept in A_game.cpp: 4012a0, 4013b0.
- 402f10 (1220 bytes): first draft in A_open.cpp, 1402 vs 1220+tables; types fixed (signed id, byte flags), the byte-table switch and
  hoisted key load still differ.
- Lever found late: VC6 `sub r,k; je; dec r; je` sequences are `switch` statements (not if/else chains); converting the
  if-chains in 405770 closed it. Cases are laid out in source order; identical case tails are merged by the compiler.

## MATCH-PENDING / exceptions
- 406350 (RULE-EXCEPTION in source): tests `&DAT_00951fcc->m_8 != 0` because the original bytes contain `lea ecx,[eax+8]; test ecx,ecx`.
- 404b20: reinterprets the byte buffer as `const int *` for the word-indexed checksum (natural for a serialized block).
- 402a80 (open) reuses its parameter slot as scratch buffer for M_004034c0 (original behaviour).

## Cross-lane changes done at request of lane B
- `Field` (Game+0xa7984) now exposes m_7c520c, m_7d522c, m_7d5230, m_7d523c; `Rec_87cbd4` is a 100-int record with
  `int m_2c[0x59]`.

## Requests to other lanes (done unless listed)
- Lane B: `DAT_004bd608` is an int array of option counts, not a scalar (0x4052e0 indexes it through `(&DAT_004bd608)[sel]`).

## Naming / modelling decisions
- Slot::m_5094 is a flat `char[0x48]` of 12 six-byte romaji candidates (KEY_LEN).
- Menu (0xbc bytes, DAT_0099cfd8[8], current DAT_00951fcc) has paired arrays m_8[2] (wrap flags), m_14[2] (cursor),
  m_1c[2] (size), strings m_3c[0x20].
- RomajiEntry DAT_00429040[0x19e] (kana[7], rom[4][7]); POS_NONE sentinel; StageRow from lane B reused for DAT_004bddf8.

## Tool notes
- build\A\fd.sh <obj-basename> <addr>: full instruction diff (difflib) of one function; build\A\mv.py moves a function to A_open.
- /tmp is shared between workers; scripts live in build\A.
