# Decompilation workflow and findings

## Toolchain
`tsh.exe` was built with **Visual C++ 6.0**: the PE header has linker 6.00 and the Rich header names cl 12.00.8168
(84 C and 24 C++ objects). The C runtime is statically linked VC6 `libc.lib` (strncpy/strncmp bodies verified byte-identical).
Flags that reproduce the code: `cl /nologo /c /O2 /D WIN32 /D NDEBUG /D _WINDOWS`, plus `/GX` for the three files in
`config/flags.tsv`. Padding between functions is 0x90.

## Layout of the executable
- `.text` 0x401000-0x426B2D: game code 0x401000-0x41EEB0 (484 functions, `WinMain` = 0x41E0D0), CRT 0x41EEE0-0x426B2D (193).
- The game is one big object (`Game`, `operator new(0x21efbf8)`, global 0x11216c4) with `__thiscall` methods plus free functions.
- Imports: DirectDraw (`DirectDrawCreate` only, 640x480 8-bit palettised mode), DirectSound, WINMM (MCI/mmio), GDI text, KERNEL32/USER32.
- Ghidra missed some functions: 0x4180F0 (MCI window procedure) was added by hand; 0x4089A0+0x4089C8 and 0x4120E0+0x412118
  are each one function that Ghidra split.

## Findings that affect a byte-identical rebuild
- **Gaps between functions are not plain linker padding.** Of 457 gaps in the game code, 185 are all-0x90, 32 are jump tables or
  data, and 240 are `ff 15 <IAT slot>` (a dead `call dword ptr [import]` to a random import slot) followed by a copy of the
  previous function's last bytes. That looks like a post-link transformation, not VC6 output. `tools/gap_table.py` writes
  `build/gaps.tsv` describing every gap; a documented post-link step driven by it would reproduce them.
- **Initialised data** (`.data`, 345 KB non-zero) is reproduced byte-for-byte as typed C++ by `tools/data/gen_data.py`
  (types and extents in `tools/data/data_spec.py`, values read from your copy of the exe): 771,252 bytes in 133 symbols.
  The ~577 KB stage timeline script (0x430840-0x4BD548) is an int stream of opcode+arguments read by the interpreter at 0x4078F0.
  Not yet covered: the string-literal pool layout (needs the final link), libc data, `.rdata`, `.rsrc`.
- **Word banks** (`WORD/word_NN.dat`): XOR-obfuscated cp932 text; a file starting with CR LF is read as plain text. See `tools/tsh_words.py`.

## Status
See the numbers printed by `python tools/build_all.py` (also in `README.md`). Open work is mostly register-allocation or
block-layout near-misses (see `docs/reports/`), ~40 unstarted large functions, the blit-loop family (VC6 turns a column-index
loop into two pointer induction variables; the source spelling is not found yet), the string pool, and the final link.

## Ghidra export and missed functions
`tools/ghidra/run_ghidra_export.bat` writes `build/ghidra/export/tsh.exe/decompiled.c` (read with `tools/fn.sh <addr>`).
Coverage check with the GameFrame plugin's `tools/triage/exportcov.py` found 92.5% of the code bytes inside listed functions;
most of the rest is jump/byte tables inside functions, but 21 real functions were missed (the window procedure 0x41E530,
vtable stubs 0x41EBB0-0x41EC20 and a few helpers). They are listed in `config/lanes/X.tsv` and are in `config/functions.tsv`.
`tools/ghidra/DecompileAt.java` creates a function at given addresses and prints its decompilation and body extent.
