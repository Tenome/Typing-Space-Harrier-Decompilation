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

## Second pass (coordinator request: continue down the list)
Matched additionally:
| address | file | change |
|---|---|---|
| 0x409d10 | N_i.cpp | `int none = DAT_00430658;` declared after `i`, loop written as `for (i = 0; i < 0x40; i++)` with the early `return i` |
| 0x4103c0 | N_j.cpp | result variable `found` declared first (own stack slot), counted `for (i = 0; i < span; i++, base++)` instead of do/while |

Lesson: declare the returned/spilled result local first, and write counted loops as `for` rather than do/while after a guard.

Remaining best (instruction-diff score from the permuter scale; 0 = match):
- 0x410c20: 4 (locals `ok`, `file`, `name`, `size` in that order, `while (1)` with trailing break; only the `mov esi,table` placement differs).
- 0x4121a0: 10 (orig keeps mask 0x80 in ecx through `and`/`mov [mask]`; mask/bits locals did not change it).
- 0x41e190: 2 (orig emits `xor eax,eax` before `mov ecx,8`).
- 0x40ba00: 8 (orig `test eax,eax` and zero in edx; `n >= 0` hoists the zero into edi).
- 0x41d0d0: 30 (only `span = center - first` placement after the `steps` test differs).
- 0x405bd0: 9, 0x406be0: 10, 0x4042a0: 14, 0x4013b0: 23 (permuter bests), 0x4076a0: 22, 0x4090a0: 24, 0x40bbe0: 25.
- Not attempted: 0x40c340, 0x404490, 0x412400, 0x41c8a0, 0x403d80, 0x402f10 and the rest of the list below 0x405b80 (large or low fuzzy).

## Third pass
Matched additionally:
| address | file | change |
|---|---|---|
| 0x41e190 | N_k.cpp | the four scalar clears first, then the two element-clear loops (independent statements reordered) |
| 0x405b80 | N_l.cpp | real data model: `for (i = 0; table[i].kana[0] != 0; i++) { n = 0; for (k = 0; k < 4; k++) { if (table[i].rom[k][0] == 0) break; n = k + 1; } counts[i] = n; }` instead of pointer walking |
| 0x406540 | N_m.cpp | `if (x == 1) {clamp} else {wrap}` written as `switch (x) { case 0: wrap; break; case 1: clamp; break; }` |
| 0x4042a0 | N_n.cpp | flag `expired` (set when tick >= limit), condition inverted so the finish block comes first with `if (m_39c != 0) 0xb else 2`, counting block in the else |

Lessons: model the loop semantics (separate counter, result assigned from counter+1) rather than the decompiler's pointer walk; a `dec/je` on a 0/1 selector is a switch; a hoisted `xor ebx,ebx` shared with an early zero store is a flag local; the block placed after `ret` is the else branch.

Remaining best: 0x410c20 4, 0x4121a0 10, 0x4121f0 9 (needs `unsigned bit`), 0x40ba00 8, 0x406be0 about 10 (register naming only: count lands in edx, orig ecx; idx-- before p--, `unsigned value`), 0x4090a0 about 30 (ebx/edi swap, mode spilled), 0x40c340 28 with ones[12] (frame is 0x150 in the original; late `push edi`), 0x41d0d0 30, 0x40bc60 11, 0x4013b0 23.
Not attempted or not reachable in the time: 0x404490, 0x412400, 0x41c8a0, 0x403d80, 0x402f10 and the D/H/I lanes' large functions (blit, MCI, DirectDraw): their parked sources are still far (fuzzy below 50).

## Fourth pass (large functions and near-misses; no new matches)
- 0x404490: 53 (parked 63) with the `goto bump/restart` labels replaced by inline copies; the original lays out case 1's tail separately and falls case 3 into the restart block, mine still places the restart/bump block differently (1064 vs 1050 bytes). Not matched.
- 0x412400: 7 (parked 14). Writing `if (DAT_00a4b0a8 == 1) return 1; { ... } ` with literal 1 instead of an `ok` variable gets 7; remaining difference is the original keeping eax=1 as a live value (`cmp ecx,eax`, `mov eax,1; mov [DAT],eax` at the end). Making `ok` non-constant (assigned 0 on the failure paths) did not change it.
- 0x41c8a0: 36 and 0x403d80: 67 (permuter 4 min); 0x402f10 not reached.
- 0x410c20: 4 (best variant: nested `if (*name != 0) { file = table; if (size != 0) while (1) {...} }`; a do-while bottom test makes VC6 peel the first iteration).
- 0x4121f0: 9; `top *= 2` after the loop made it worse (22).
- 0x406be0: 11 after trying all 240 local-declaration orders with int/unsigned value.
- 0x410440: 2, unchanged.
