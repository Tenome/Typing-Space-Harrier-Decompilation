# Lane G report (0x40c340-0x410800, 27 functions)

Counts: bytes-matched 13 of 27 (12 build clean in the shared tree; 0x40c780 matches only with request 1 below, verified in a private
copy of the tree). Matched under the rules: 13 (no dummies, asm, pragmas). MATCH-PENDING: none. ORIGINAL-ASM: none.

## Matched
G_draw.cpp: 40e7a0 40e900 40e9f0 40eb70 40ec90 40edc0 40f0a0 (draw-request builders; `DrawRequest` in game_prelude_G.h, Game::Req0()/Req1()
are inline views of m_8ef790/m_8ef794; 40eb70/40e7a0 reuse the `tile` parameter slot as the width out value, which matches).
G_menu.cpp: 4100e0 410600 410800.  G_group.cpp: 40ca80 40cc70 (both switch/loop forms found; ca80 is an index loop over DAT_008f5450).
G_bankload.cpp: 40c780 (needs request 1).

## Open (G_open.cpp, each with an // OPEN: note)
- 40c340 (782/780 B): register/schedule; list loop is strength-reduced, edi saved in prologue instead of mid-function.
- 40d2c0 (312 = 312 B): register/schedule, load order of bank->m_10 / m_14 vs DAT_004f052c->m_b0.
- 40ef00 (303 = 303 B): one register (eax vs ecx for the table index). Declaration order, unsigned %, extra local tried.
- 40f720 (284/277 B): register/schedule; for+break shape right; 290 random declaration permutations did not hit.
- 40fab0 (391 = 391 B): register/schedule (ebx holds m_7d522c until the loop).
- 40ff90 (295/303 B): register/schedule + byte-width fill expression.
Blocked on requests (G_blocked.cpp, does not compile in the shared tree until the request is applied):
- 40f1c0 (333/361 B): needs request 2; original reads the lead byte through a dword stack slot (byte store, dword load & 0xff).
- 40fd10 (617/638 B): needs request 3; original keeps five values in stack slots, two more locals (register/schedule).
Not attempted (large, need data models first): 40d400 (1097 B; slot fields 0x44-0x5c, 0x84, 0x50e0 not modelled), 40dd60 (1329 B),
40e380 (needs int arrays overlapping lane A/B fields 0x87cbfc..0x87d3fc, see request 4), 40e570, 40f3a0 (unknown structs at 0xa3d468,
0x8f1010).
40c690: `jmp 0x40ca80`, a linker (incremental link) thunk, not source.

## Requests (orchestrator)
1. game_methods_B.txt: `int M_0040c9f0(BankRec *bank, int store);` (it returns the entry count; B_open.cpp's body needs `return count`).
2. game_methods_A.txt: `int M_0040f1c0(int x, int y, char *text);` (returns 1/0; callers ignore it).
3. game_prelude_A.h Field: replace `char m_pad7c5210[0x10000];` by `int m_7c5210[0x4000];` (also asked by lane B for 40a7b0).
4. Game fields: 0x87cbfc int[0x100] and 0x87cffc int[0x100] (and 0x87e7fc / 0x87cbf0..) are arrays that overlap B's m_87cbfc[0xb],
   m_87cc28, m_87cc2c, m_87cdf4 declarations; needed for 40e380.
5. game_fields_C.tsv: m_8ef790/m_8ef794 are `DrawRequest *` (see game_prelude_G.h); DAT_00a2330c (prelude C) is a `GlyphSheet *`
   (40f0a0 casts it today). Optional: after that the Req0()/Req1() accessors and the cast become redundant.

## Decisions
- New types in game_prelude_G.h: DrawRequest (0x58 bytes, same layout as lane E's DrawObj but typed), overloads
  `FUN_00413a90(DrawRequest*)` and `FUN_00417ee0(void(*)())`; symbols_G.tsv: blendHalf = 0x419e70 (blend callback), __alloca_probe.
- Game fields added: m_87fbfc, m_a7968 (game_fields_G.tsv).
- Per-file flags: none.

## Tools
build\G\b.sh (build), fd.sh (full diff), perm.py (random declaration permutations), build\G_tree\ is a private copy of the tree for
experiments that need the requests above (tshimg.py patched to absolute paths).
