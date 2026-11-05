# Lane D report (0x412b90-0x4188f0, 55 functions)

## Counts
- Bytes-matched: 13 of 55 -- 412e80, 413610, 413670, 4139f0, 413a20, 413a50, 413a90, 4152e0, 4178f0, 417ee0,
  417ef0, 417fd0, 4188f0.
- Matched under the rules: 13 (no tricks; the MCI channel table is a typed `extern "C"` array named through
  symbols_D.tsv: mciChannelOpen = 0xa4b134, mciCdOpen = 0xa4b0e0).
- MATCH-PENDING: none.
- Open (source present in src\D_open.cpp, each with an OPEN note): 412b90, 412ec0, 412fe0, 413280, 413350, 413430,
  413780, 413900, 4136f0, 413870, 4147c0, 414830, 4149d0, 4185a0, 416100, 4180f0 (16).
- 4180f0 (MCI window proc) is the nearest miss: 1197 bytes = original size, differs only by one hoisted `hwnd`
  load (wall class register/schedule) in the 'Play %s%d From 1 Notify' branch; for/do-while variants tried.
- Not transcribed (too large / not attempted beyond reading the Ghidra output): 413ad0 (3267 bytes), 414bd0,
  414f00 (scaled blits, Bresenham loops, 4 variants each), 415320 (3552), 416200-417d20 (22 clipped/scaled blit
  helpers of 415320) and 417d20. Expected cause if attempted: same register-allocation/temporary-count walls as
  414830/4149d0/416100 (original keeps one more row-counter copy or fewer temporaries than a direct transcription).

## Files
src\D_font.cpp (glyph sheet helpers), src\D_blit.cpp (list walkers, stubs), src\D_mci.cpp (MCI BGM), src\D_open.cpp,
game_prelude_D.h (types PaletteTable, SheetImage, GlyphSheet, BlitRequest, BlitJob, ClipJob; real IDirectDraw* /
IDirectDrawSurface* / IDirectDrawPalette* via <ddraw.h> with the default DIRECTDRAW_VERSION 0x500), symbols_D.tsv
(malloc, free, the two MCI names). Flags: default /O2 only.

## Findings
- DAT_00a4a890 is a 0x800-byte PaletteTable {PalEntry[256]; int used[256]}; 4139f0/413a20 are struct copies.
- 412b90 renders a font into a system-memory surface and stashes its state in DAT_00a4a830..a4a85c; 412ec0 finishes
  it and returns the SheetImage* (Ghidra's register return is wrong). 413430 builds a GlyphSheet (0x20-byte header
  + SheetImage) with cell size = font height.
- 413280/413350: the Lock retry loop is emitted once with a back-jump in the original; every do/while, for(;;) and
  goto form duplicates the call. The fill/copy bodies match.
- 413780: the table-init loop matches (index form, sum in a local); the lookup tail differs in the return-0 merge.
- 4136f0/413870: original spills the byte to the stack and widens it into edi.
- 4147c0: original walks char pointers (dst+3 / src+1). 414830/4149d0: second row-counter / register pressure.
- Permutation search script (declaration order) found no improvement for 412b90 (aborted) or 413430.

## Tool problems
tools\fn.sh needs the 8-digit address; from bash `build.bat` needs `cmd //c "set LANE=D&& .\build.bat X.cpp"`;
include\game.h occasionally fails to compile because of other lanes' in-progress members/globals (retry after a
minute); lane E's PadState type for 0xa4b134 conflicts with the original dword accesses (worked around with the
renamed mciChannelOpen array above).
