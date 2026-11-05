# Lane J report (round 2)
Bytes-matched: 0 of 4. Matched under rules: 0. All four attempted and parked in src\J_open.cpp (one OPEN note each).
Closest byte counts (mine vs original): 0x41d0d0 737 vs 737 instruction bytes (same 259 instructions, 5 differ; the compare tool's
788 includes the 48-byte jump table), 0x41a880 1168 vs 1124, 0x419e80 2442 vs 2426 (measured as a separate unit before merging), 0x41aef0 6040 vs 6102.

- 0x41d0d0: key-selection animation. Walls: head of the second loop (original tests steps, then computes span; ebp counter) and stack-slot order of steps/center. Class: register/schedule.
- 0x41a880: draw list. Walls: induction-variable base of the 8-case switch stores (original [1].m_20, always the second reference of case 1 -- same in 0x419e80's init switch, so it is a real compiler rule I could not trigger from source), add vs lea in one block, scheduling of the last block.
- 0x419e80: loader + init, fully transcribed (palette, LOGFONT, sprite table, DrawObj init loops, NameRow). Walls: same IV-base rule, zero-register pattern for the DAT_0101e55c/DAT_01121590 stores, `free(buf)` hoisted before the m_34 test.
- 0x41aef0: tutorial state machine, fully transcribed with gotos expanded. Wall: original keeps constant 1 in edi and never hoists 0; mine hoists 0 in several arms (register allocation / constant hoisting).
Declarations: game_prelude_J.h. Casts used: BlitRequest*, `(int *)DAT_0101fdd0` for the sprite table (TextHost only models +0x34), `(int)DAT_0101e56c`.
## Requests for the orchestrator
- game_prelude_A.h: `void FUN_00419e80();` must be `int FUN_00419e80();` (J_open works around it with a #define around the include).
- lane E: DAT_0101e56c should be `int`; TextHost should expose `int m_08[...]`; FUN_0041d940 is called without movsx, i.e. callers see `int` (declared int locally in J_open.cpp only).
- E declares DAT_0101fc98 as one DrawObj; the original is a 2-element array (J indexes `(&DAT_0101fc98)[i]`).
## Tools (build\J\)
cmpfull.py (full instruction-level diff with summary), try.sh (build + diff), dis.bat, gen_state.py (generates the 0x41aef0 source), rd.py (read data from tsh.exe).
