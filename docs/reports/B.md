# Lane B report (0x406ef0-0x40cc70, 112 functions)

Status: 60 of 112 byte-matched (all 60 also pass the code rules); not finished. Matched code: src\B_player.cpp,
B_effect.cpp, B_script.cpp, B_stage.cpp, B_timer.cpp, B_misc.cpp. Written near-misses are parked in src\B_open.cpp
with // OPEN: notes (17 functions). Support: game_prelude_B.h, game_methods_B.txt, game_fields_B.tsv, slot_fields_B.tsv,
symbols_B.tsv.

## Bytes-matched (60)
407040 407230 407590 407740 407850 407c90 407e40 407f70 4082f0 408460 4084d0 408540 4085d0 4085f0 408650 408980 408cb0 409210 409300 409ca0 409cc0 409d50 40a100 40a160 40a1d0 40a210 40a490 40a520 40a5c0 40a640 40a660 40a8c0 40af40 40aff0 40b0e0 40b330 40b350 40b370 40b3d0 40b3f0 40b410 40b430 40b4d0 40b520 40b7b0 40b800 40b940 40be00 40be30 40bea0 40bee0 40bf30 40bf80 40c250 40c2a0 40c6e0 40c710 40c760 40c970 40c9c0

Forms worth a reader's note: 409300 keeps a named intermediate (stops VC6 folding 0.5f * -134.0f); 40af40/40aff0 cast
m_87cbc4->m_0 + n*400 and m_640000 + n*20 to the record type (lane A's Field is byte-addressed); 40c710/40c9c0
cast lane C's int BankRec::m_00[1] to a pointer.

## Open, in B_open.cpp (17; register/schedule class unless noted)
406ef0 (c reloaded from stack), 4071c0 and 40ba00 (zero constant hoisted into a register; original: test + late xor),
4076a0, 407cd0 (only SIB order [eax+ecx] vs [ecx+eax]), 407ed0, 409270, 409ce0, 409d10, 409db0, 40a6d0,
40bbe0 and 40bc60 (original keeps 4 callee-saved regs; loop shape), 40c9f0 (bool locals),
40c650 and 40c6a0 (loop end must compare signed (int)p < const; index form changes structure),
4089a0 (whole function after the inventory fix; original interleaves extra arg pushes and one deferred add esp,0x24:
call-set/CFG class). permute was not run (no permuter tooling in this project); wall-identifier classes above are
from reading the diffs. Each got at least 2-4 source variants (see git-less history in this report only).
FUN_004185a0 is declared with 5 args in lane E's prelude but called with 2 here; mine is an overload.

## Not started / blocked
- 40a7b0: needs an int list member at Field+0x7c5210 (lane A's Field still has padding there).
- Not attempted (large, data heavy): 407fd0 408130 4081b0 408320 408660 408ad0 408b70 408d00 408f60 4090a0 409190
  4093b0 409700 409e00 40a270 40a900 40b140 40b5c0 40b850 40b8b0 40ba50 40bce0 40bfa0 40c340 40c780 40ca80 40cc70.

## Findings
- DAT_009a5e98/DAT_009a5e9c are one struct (aliasing). Per-case calls in a switch give push N; jmp common tails.
- Explicit range checks (kind < 4) exist in the original; compare operand order matters (n >= field vs field <= n);
  double literals for float compares; strlen() is unsigned (shr vs sar).
- memset/memcpy give rep stos/movs. Per-file flags: none.

## Tool problems
- /tmp shared between workers. Other lanes' temporary Game/Field layout errors made matched functions flip to DIFF
  until fixed; re-run builds before trusting a regression. Duplicate-declaration warnings from other lanes.
