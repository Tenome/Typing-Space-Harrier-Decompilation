# Lane H report (DirectDraw/sound glue and big blits, 0x410900-0x415320, 25 functions)

Bytes-matched: 13 of 25. Matched under the rules: 13 (no dummies, asm or pragmas). MATCH-PENDING: none.
Files: H_list.cpp (build with /GX: SEH frame in ~NodeList), H_ddraw.cpp, H_stream.cpp, H_caption.cpp, H_open.cpp (parked).
Support: game_prelude_H.h (ListNode/NodeList/GroupTable, StreamHeader, SoundLoader, DDraw globals), game_methods_H.txt,
symbols_H.tsv (vtables, deleting destructors, DirectDrawCreate thunk 0x41eea6, PaletteChangeHook = 0xa4a878).

## Matched (13)
410900 (Game::M_00410900, caption slide + letter draw; `char letter[16]`, `ch == ' '` branch listed first),
410e30 410e50 410e70 410ed0 410ef0 410f80 (ListNode / NodeList intrusive list with vtables 0x4271e0/0x4271e8),
411b50 411bf0 (stream header check / seek on lane C's StreamRec), 412730 4127a0 (extern "C" DisplayReady, see request) 4127d0 412880.

## Open (in H_open.cpp, each with an // OPEN note)
- 410ea0, 410f40 (register/schedule): original clears the 5 group slots through a pointer (`lea edx,[esi+0x18]` / `add edi,0x18`), mine addresses members directly; unrolled loop in source gets the pointer but ordering differs by a lea.
- 412800 (CFG): loop head emitted once with `jne end; jmp top`; every for/while/do form duplicates the head (same wall as lane D 413280).
- 4119a0 (register/schedule): identical except register naming in the final write-mode tail; `exists` reusing the path param slot already works.
- 411e80 (CFG/layout): all calls match, original lays out [close B][fail: close A; ret 0][success] with early failures jumping into the middle; VC6 inlines the success block first.
- 412400 (DDraw init, 818 vs 816): residue = eax=1 hoisted before the prologue, palette-clear loop induction variable starting at the table address (signed `jl` against 0xa4ac90), register naming in the colour-key tail.
- 4129a0 (BMP loader, 497 vs 489): param-slot reuse for the palette count, ebp image pointer, dword-and-0xffff load of biBitCount.
- 4114d0 (WAV loader, 898 vs 914): slot pointers and zero register differ, duplicated failure tails.
- 414bd0 (873 vs 812), 414f00 (958 vs 985, draft): 4-variant Bresenham scaled blits, same register/spill walls as lane D's 414830/4149d0.
- 413ad0 (3267 bytes) and 415320 (3552 bytes): NOT attempted. 413ad0 is the BlitRequest clip/dispatch (256-int key table on the stack, clip arithmetic, switch over source/destination surface kinds, DDraw Lock retry loop = the 412800/413280 wall) and calls the leaf blits that are themselves open; 415320 is its sibling for the other kind of request.

## Levers found
- VC6 reuses a dead parameter slot for an address-taken local: 412880 matches with `HDC dc` local + `COLORREF color` parameter (no cast needed); the same applies to buffer/len outputs in 4114d0 and `exists` in 4119a0.
- `for (i = 0; i < 5; i++) slot[i] = 0` is unrolled into a pointer walk with `lea` when the array is a member (410ea0/410f40 need the exact placement).
- Deleting destructors (`??_G...`) are compiler generated; map them in symbols_H.tsv to the vtable slot addresses.
- Classes with SEH frame destructors need /GX per file (H_list.cpp).

## Requests to other lanes / orchestrator
- game_prelude_00.h declares `void FUN_004127a0();` but the function returns int (callers test eax): change to `int`; then rename `DisplayReady` in H_ddraw.cpp/symbols_H.tsv back to FUN_004127a0.
- Lane D: `extern int DAT_00a4a878;` is really `int (*)()` (called by 4127a0); I use `PaletteChangeHook` mapped to the address. FUN_00417ee0 then takes the function pointer.
- Lane J declares FUN_004129a0 as int(char*,uint,uint*); the function returns SheetImage* (my overload is in H_open.cpp).
- Lane C: SoundMgr could get `int FUN_004114d0(const char*, int)` directly (lane E has SoundMgrExt::M_004114d0, I have SoundLoader::M_004114d0 in game_prelude_H.h).
