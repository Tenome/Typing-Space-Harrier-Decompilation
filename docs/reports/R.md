# Lane R report (round 4)

New files: `src/R_*.cpp`. Parked copies in `*_open.cpp` untouched.

## Matched (clean)
| address | file | change against the parked copy |
|---|---|---|
| 0x4071c0 | R_clear.cpp | the four `DAT_00a46264[k] = 0` stores written as `memset(DAT_00a46264, 0, sizeof(DAT_00a46264))` |
| 0x40ba00 | R_clear.cpp | the five record stores written as `memset(m_87cbec, 0, 0x14)`, condition `n >= 0 && ...` |
| 0x410ea0 | R_clear.cpp | `group.Clear()` replaced by `memset(&group, 0, sizeof(group))` |
| 0x410f40 | R_clear.cpp | same |

Lever: VC6 `/O2` expands `memset(p, 0, small_const)` inline (intrinsic) with its own `xor reg,reg` and a base register
(`lea edx,[esi+0x18]` / `add edi,0x18`); explicit zero stores or an element loop let the allocator hoist the zero
constant (cmp reg,zero / extra push) instead. Try memset for every "zero register hoisted / pointer to the block
computed first" wall.

| 0x411db0 | R_stream.cpp | no `pos` temporaries: `stream->m_14++; if (stream->m_14 > stream->m_18) stream->m_18 = stream->m_14;` (VC6 keeps the incremented value in a register by itself and flips the compare), and the putc argument is `(unsigned char)c` (the macro then pushes `c & 0xff` to `_flsbuf` and keeps `c` for the `r != c` test) |

## Still open (best state; score = permuter scale, 0 = match)
| address | best | wall |
|---|---|---|
| 0x40d850 | 6 | orig loads `xor eax,eax` before the second loop and stores `[i],eax`; also `jle` jumps into the epilogue with eax=0; mine stores an immediate. swapped counter names, while/for, extra locals: no change |
| 0x4043d0 | 2 | orig loads m_8ac into eax before `cmp eax,edi`; flipped compares, local copy, if/else: no change |
| 0x401da0 | 4 | orig loads the value argument between the two leas (after the first push); no spelling found |
| 0x4189a0 | 3 | `movsx` + `and 1` kept in the original, VC6 narrows to `mov al`; casts/int/unsigned/`!= 0` tried |
| 0x410440 | 2 | argument loaded into eax (orig) vs edx; struct-row spelling and `int` return tried |
| 0x41ad10 | 5 | `shl eax,3; [eax+abs]` store; not reproduced |
| 0x4096b0 | 7 | `for` form with `DAT[i]` reaches 7: only the lazy `push esi` after the early return differs |
| 0x406ef0 | ~2 | structure is right (parked copy); only `mov eax,edi` vs reload of `c` from its stack slot in the `frame = c` path (jump table bytes also listed as diff by the comparer) |
| 0x4081b0 | 7 | strength-reduced base `&row.m_10` vs `m_0`; reordering the field reads, direct field uses: no change |
| 0x40ef00 | 6 | index register eax vs ecx; index local, `& 0x1f`, declaration orders: no change |
| 0x4119a0 | 9 | register naming in the write tail; permuter 21 |
| 0x411e80 | 36 | exit-chain layout; duplicated-return and nested-if forms are far worse (122, 45) |
| 0x41cc80 | 32 | x/y live in esi/edi in the original with `dx/2` kept in ebx across the dy computation; temporaries and inline forms did not help |
| 0x4076a0 | 21 | grade spill and loop pointer; indexing `DAT[len*4+i]` is no better |
| 0x4090a0 | 24 | ebx/edi swap |
Others in the lane list were not reached in this round (4052e0, 404490, 402f10, 41d0d0, 4180f0, 41d9e0, 41df40, 41d460, 40bfa0).

## Lessons this round
- `memset(p, 0, small_const)` is an inlined intrinsic with its own zero register; try it whenever a run of zero stores shows a hoisted-zero or `test` vs `cmp` wall.
- Updating a member directly (`s->a++; if (s->a > s->b) s->b = s->a;`) lets VC6 keep the value in a register itself; a `pos` temporary changed the register plan and the compare direction.
- `putc((unsigned char)c, fp)`: the macro then pushes the masked value to `_flsbuf` and keeps `c` for later compares.
- VC6 sometimes pushes a callee-saved register lazily after an early return (4096b0 original) or inside a block (40c340 original); mine pushes at the top. Cause not found.

## Second session (after merging master)
Matched (src/R_clear2.cpp):
| address | change |
|---|---|
| 0x409ce0 | the five timer stores became `memset(DAT_004efa18, 0, 0x14)` |
| 0x410760 | the zero-word loop became `memset(DAT_008f544c, 0, sizeof(*DAT_008f544c))` (through the global pointer, not `&DAT_0099d6c0`, so the later field stores reload the pointer as the original does) |

Memset retried without a match: 0x41c8a0 (36 to 25 with memset on the 3-int clear, but the original keeps a hoisted zero in edi, so the explicit stores stay; remaining wall is the register rotation of the tail argument loads), 0x412ec0 (58 to 48; the original clears the palette with a pointer loop, not memset), 0x411340 (variable-size memset is a call, worse), 0x409e00 (no change; the original indexes `short DAT[c*2]` through `lea ecx,[eax+eax]`).
Other tries without result: 0x4013b0 (original pushes esi only after the first test: the lazy callee-saved push seen in 4096b0 and 40c340), 0x4012a0, 0x40b140 (index form 73), permuter 10 min on 4012a0, 405c50, 403d80, 41d460, 41d0d0 (best 67, 126, 67, 55, 30).
