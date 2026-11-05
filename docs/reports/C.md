# Lane C report (0x40cdd0-0x4129a0, 128 functions)

Status: 65 byte-matched (progress_C.tsv), 65 matched under the rules (no dummies, no asm, no pragmas, no MATCH-PENDING),
20 written but parked in src/C_open.cpp (// OPEN note each), 43 not yet attempted.

Source files: C_group, C_layout, C_misc, C_state, C_table, C_bgm, C_stream, C_sound (SoundMgr), C_lz (LZSS decoder), C_scan, C_open.

## Matched
40cf20 40cf50 40d110 40d130 40d980 40d990 40da60 40e2d0 40e340 40e360 40f030 40f060 40f330 40f380 40f840 40f860 40f880 40f890 40fa00 40fa70 40fa80 40fc40 40fc50 4100c0 410200 410240 410260 410280 410320 410360 4104e0 410580 410730 410af0 410b00 410ba0 410bd0 410c00 410ca0 411020 411070 411100 411120 411190 411260 4118f0 411910 411920 411930 411b20 411cd0 411ce0 411d20 411da0 412040 412060 412080 412190 4122a0 4122d0 412300 412350 4123b0 4123d0 4123f0

## Open (written, not matching); code and residue per function in src/C_open.cpp
40cdd0 40d160 40d850 40db00 40e2a0 40f8a0 410340 4103c0 410440 410460 410760 410c20 4111f0 411340 411db0 4120e0 412118 4121a0 4121f0 412250
Classes: register/schedule (cdd0 d160 db00 4103c0 410440 4121f0 410460), CFG/block layout (f8a0 12250 d850 e2a0 410c20 4111f0 411340),
value forwarding / constant folding (410340 410760 4121a0 411db0). 4120e0 includes the Ghidra-split 0x412118 (one 170-byte function).

## Not attempted
40d2c0 40d400 40dd60 40e380 40e570 40e7a0 40e900 40e9f0 40eb70 40ec90 40edc0 40ef00 40f0a0 40f1c0 40f3a0 40f720 40fab0 40fd10 40ff90 4100e0 410600 410800 410900 410e30 410e50 410e70 410ea0 410ed0 410ef0 410f40 410f80 4114d0 4119a0 411b50 411bf0 411e80 412400 412730 4127a0 4127d0 412800 412880 4129a0
Blockers: 410e30-410f80 are a class with vtable and SEH frame (needs per-file /GX; not started); 4114d0 is a very large mmio/dsound
loader; the e380-ef00 drawing group and dd60/d400/d2c0 word-bank functions were not started.

## Requests and cross-lane notes
- Rec_87cbd4::m_2c[0x59] from lane A is enough (+0x8c is m_2c[24], +0x64 is m_2c[14]); no further request.
- Slot::m_1494 is char[0x1400] (ends at 0x2894); lane A may declare m_2894.
- Thiscall methods (call sites show mov ecx, esi): M_0040d980 M_0040da60 M_00410240 M_0040f030 M_0040f880 M_0040f890 M_0040fa70
  M_00410af0 M_00410ba0 M_00410bd0. Free: FUN_00411910 FUN_00411920 FUN_0040f060; FUN_0040d130 is stdcall.
- M_0040e340/M_0040e360 return int; m_8eced8/m_8ef6d8/m_8ef6dc are int* (lane B request, done).
- Lane E's SoundSystem DAT_00a460a0 is my SoundMgr (C_sound.cpp); both coexist through the address mapping.
- Levers that worked: DAT_xxx->field for global record pointers (forces reloads), "a = a + 1" through a pointer local, "x ? 1 : 2" for
  neg/sbb, switch for the sub/dec chain, operand order (limit > v), CloseHandle shared tail, DAT global used as the loop iterator.
