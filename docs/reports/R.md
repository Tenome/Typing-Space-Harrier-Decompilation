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
