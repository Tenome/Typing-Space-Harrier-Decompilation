# Lane E report (0x4189a0-0x41eeb0)

Bytes-matched: 45 of 70 (final). Matched under the rules: 45 (no tricks, no pragmas, no asm). MATCH-PENDING: none.
Files: E_misc, E_ui, E_keyicon, E_game2 (plain), E_app (window classes, build with `/GX`), E_open (parked near-misses).

## Open, attempted (OPEN notes in E_open.cpp / E_app.cpp)
- 0x4189a0: flow matches; original keeps movsx+and 1, VC6 emits mov al (masked/type class).
- 0x41a820: inc edi vs lea edi,[eax+1] (register/schedule).
- 0x41ad10: `shl eax,3` + absolute store not reproduced (register/schedule).
- 0x41adc0: register allocation (key al, name ebx).
- 0x41c8a0: first 0x12a bytes exact; tail register rotation, 2 bytes.
- 0x41cc80, 0x41ce30: register allocation / param-slot reuse (ce30 reuses params as out values).
- 0x41d950: original loop is not peeled, mine is (constant initial slot); 155 vs 142 bytes.
- 0x41d9e0 (TypingWindow::V20), 0x41df40 (V16), 0x41e190, 0x41e300, 0x41ed50: register choices / BOOL menu flag shape (jne/xor vs setne).
## Inspected, not matched
- 0x41cb30 (parked in E_open): shl-form stores match when the 0x58-byte table is indexed directly (`T[n].m_0c = ...`, not through a pointer local); remaining prologue/`and ecx,0xffffffe0` differences.
- 0x4189d0 (1353 B, loader with 18-way switch), 0x418f70 (913 B, struct copies of 0x14/0xa8-byte records), 0x4193c0, 0x419e80, 0x41a880 (draw loop, ~40 globals), 0x41aef0 (6102 B), 0x41d0d0, 0x41d460 (inline strcpy/strcat): read and analysed but not reconstructed; each needs dozens of typed globals/struct types first.
## Not source
0x41eea0 DirectSoundCreate, 0x41eea6 DirectDrawCreate (import-lib thunks, `jmp [iat]`), 0x41eeb0 _alloca_probe (CRT hand assembly: ORIGINAL-ASM).
## Notes
- Window code: class EzWndClass (WNDCLASSEX + creation params, ctor = 0x41ec30), EzWindow (vtable slot 0 WndProc 0x41e530 not in lane, slot 16 = 0x41df40, slot 20 = 0x41d9e0 idle), TypingWindow derived. WinMain constructs it inline. Symbols for these in symbols_E.tsv (ctor, V16, V20, vtable 0x4271f0).
- `Ghidra < 0x100` is `<= 0xff` in source (compare shows jg).
- 4189a0 returns int (callers test eax), 0x41d940 is `char` (no movsx).
- DAT_00a46264 is declared int[4] by lane B but 0x41e190 clears 8 words.
- Sound object 0xa460a0: E now uses lane C SoundMgr (FUN_ methods); E re-declares `extern SoundMgr DAT_00a460a0` because C dropped it, and adds SoundMgrExt for M_00411340 / M_004114d0 which C does not list yet.
- Per-file flags: E_app.cpp needs /GX (SEH frame around `new Game`).
- Tool problems: none; tools/fn.sh needs sh, bash heredoc mangles backslash char literals (use 0x5c).
- Cross-lane conflicts (not edited): lane D declares FUN_00413610 with a GlyphSheet* second parameter (E uses it); lane A declares DAT_00a46174 as a scalar int (E clears 8 words from its address); lane B declares DAT_00a46264[4]; SoundSystem/DAT_00a460a0 is declared by E only so far.
- Hints learned: direct struct-array indexing gives `shl reg,3`+abs stores for non-power-of-two strides; char/short index types also give shl. 0x41ad10 with `short slot` is within a movsx (171 vs 168 bytes).

## Pass 3
- Menu flag: `BOOL f = TRUE; if (menu == 0) f = FALSE;` (declared right before the call) gives the original `mov eax,1; jne; xor eax,eax`; 0x41e300 and 0x41ed50 are now size-exact (379/221), remaining diff is register naming (ebx/edi) only.
- 0x41d460 modelled (NameRow/NameTiming/digit tables, strcpy/strcat/strncpy intrinsics): 737 vs 735 bytes, flow identical, original hoists a zero register (ebx) that mine does not. Parked in E_open.cpp.
- 0x41d0d0, 0x418f70, 0x4189d0, 0x4193c0, 0x419e80, 0x41a880, 0x41aef0 still not reconstructed (no time for the data models). No permuter tool is available in this environment, so permute/wall-identifier were applied by hand only.
