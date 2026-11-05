# Lane N report (round 3)

New files: src/N_a.cpp .. N_h.cpp, N_t410340.cpp, N_t4068a0.cpp. Nothing else edited; parked copies in *_open.cpp still exist.

## Matched (clean, plain source)
| address | file | change against the parked copy |
|---|---|---|
| 0x407cd0 | N_b.cpp | byte-offset clear loop replaced by `for (i = 0; i < 25; i++) (&m_87cbc8->m_2c)[i] = 0;` (SIB order follows the indexed form) |
| 0x407ed0 | N_c.cpp | `m_8ecec8 = 0;` first, then the 0x80-int clear written as an element loop instead of memset |
| 0x402960 | N_d.cpp | `line[0x1402]` (text size + "@R"): frame padding of the buffer, not a code change |
| 0x405fe0 | N_f.cpp | `last = DAT/5;` then `if (last - 2 <= col) col = last - 2;` (the -2 is applied after the col<2 test) |
| 0x412250 | N_g.cpp | `while (read == 0) zeros++; for (i = 0; i < zeros; i++) value = read + value * 2;` |
| 0x405220 | N_h.cpp | `if (got != size) return DAT_00430658; return size;` instead of a result variable |
| 0x410340 | N_t410340.cpp | `DAT_004f0530 = 0;` before the memset |
| 0x4068a0 | N_t4068a0.cpp | plain `for` with `DAT_00a3d6ec = &table[i]` then tests through that pointer |
| 0x40d850 | (not matched) | see below |

MATCH-PENDING: none.

## Lessons
- SIB base/index order and `rep stos` use depend on indexed element access versus byte-offset casts or memset.
- A frame size off by 4 means an array whose size rounds up (0x1400+2).
- A `result` variable that is only returned often makes VC6 push registers early; early returns fix it.
- Loops with a counted second phase: `for (i = 0; i < n; i++)` after a `while (cond) n++` removes jump threading.

## Open, best state (walls)
- 0x40d850 (score 6): second loop counter initialised through `mov [slot],0` instead of via eax=0 shared register (register/schedule). Tried: while/for, extra locals, permuter 3 min.
- 0x40ef00 (6): index kept in ecx instead of eax, final lea uses edx instead of ecx (register). Variants and permuter did not move it.
- 0x40bc60 (11): original keeps a zero in ecx for the early return and the loop exit falls to a separate `xor eax,eax` (variable-vs-constant return; tried result variables).
- 0x40d2c0 (9-11): load order of DAT_00a45e34 members vs pick and register for reading/text.
- 0x4096b0 (9): original pushes esi only after the early return (i declared late); restructuring tried.
- 0x410440 (2): value loaded into eax (orig) vs edx; struct-row access tried.
- Not attempted: 0x41d0d0, 0x40c340 and everything below 0x412400 in the lane list.
