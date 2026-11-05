# Orchestrator log (decomp)
- 2026-10-29/30: round 2 lanes F–J. Applied cross-lane fixes: FUN_004127a0 int (prelude_00), M_00404d80 int (C), struct Player moved to
  prelude_00, M_0040f1c0 int (A), M_0040c9f0 int + B_open body returns count, Field::m_7c5210 int[0x4000] (A), FUN_00419e80 int (A).
  Not applied (risk of regressing E's matched functions): DAT_0101e56c int, TextHost int m_08[], DAT_0101fc98 array, FUN_0041d940 int.
  G requests 4/5 (int arrays 0x87cbfc/0x87cffc overlapping B's fields; DrawRequest*/GlyphSheet* member types) not applied.
- Applied lane F requests 1–4 (returns int for M_004093b0/0040b330/0040b350/0040b3f0, M_004081b0(int scanOnly), Field depth-sort tables);
  full rebuild of every src\*.cpp: 308 matches reproduced, 0 regressions, 0 compile errors. Not applied: request 5/7 (Rec_87cbc8
  m_2c..m_8c → int m_2c[0x19]: B's matched code uses the individual member names, needs per-function edits; unlocks 4093b0, 40b5c0).
- Round-2 results: F 11/29, G 13/27, H 13/25, I 0/26, J 0/4. Merged: 308/484 bytes-matched.
