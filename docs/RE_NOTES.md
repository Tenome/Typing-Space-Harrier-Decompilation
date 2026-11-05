# Typing Space Harrier (2002, SEGA) — tsh.exe reverse engineering notes

Target: `orig/tsh.exe` (SHA-1 185fec0816def349de984d6bdb402753004dfc1e, 987,136 bytes). A Ghidra export can be produced with
`tools/ghidra/run_ghidra_export.bat` (`ExportAll.java`); `tools/fn.sh` and `tools/asm.py` read from it. Early notes from the first
analysis session; the toolchain blocker described below was resolved (VC6 is the exact compiler, see `DECOMP_WORKFLOW.md`).

## Toolchain
- PE: linker **6.00**, no relocations (`/FIXED`, base 0x400000), 4 sections (.text .rdata .data .rsrc), subsystem GUI,
  timestamp field is the ASCII string "1969" (0x39363931), not a real date. Entry RVA 0x20132.
- Rich header: **cl 12.00.8168 (Visual C++ 6.0)**: 84 C objects (prodid 0x0A) + 24 C++ objects (0x0B), 15 + 12 + 7
  MASM/import-library objects, linker 6.00. Statically linked **VC6 libc** (strings "Microsoft Visual C++ Runtime
  Library", `GAIsProcessorFeaturePresent`, heap/lowio/stdio init), so byte-matching also needs VC6's `libc.lib`.
- **VC6 is not installed.** Only VC .NET 2003 (cl 13.10) and the Toolkit 2003 exist; both generate different code from
  VC6, so no function will byte-match with them. Needed: VC6 (`CL.EXE`/`C1.DLL`/`C1XX.DLL`/`C2.DLL`/`LINK.EXE`/`ML.EXE`
  + `LIB\LIBC.LIB` and the 1998–2001 DirectX/Platform SDK headers) — ideally the same service-pack level (12.00.8168).
- Until then: a *functional* decompile (this notes' findings, then C source) can be built with 13.10 for behaviour;
  `COMPILER_PLAN.md`-style per-function matching must wait for VC6.

## Layout (.text 0x401000–0x426B2D)
- Game code ≈ 0x401000–0x41EEB0 (WinMain = `FUN_0041e0d0`, called from CRT `entry` 0x420132). CRT ≈ 0x41EEE0–0x426B2D.
- 677 functions in Ghidra's list. Imports: DDRAW (only `DirectDrawCreate`), DSOUND (`DirectSoundCreate`), WINMM (mci/mmio,
  timeGetTime — BGM via MCI window class `HJMCIWindowClassName`), GDI text output, KERNEL32/USER32. **No Direct3D**: the
  renderer is pure DirectDraw blitting. `.data` is 13 MB virtual (large BSS-style buffers).
- DirectDraw init: `FUN_00412400` (DirectDrawCreate → SetCooperativeLevel [vtbl+0x50] → SetDisplayMode [vtbl+0x54] →
  primary surface [+0x18] → back buffer attach). Resolution is therefore a fixed DDraw mode; widescreen/modern
  resolutions would be done by hooking DDraw (scale the 640x480 back buffer to a window/desktop-sized primary) rather
  than changing the game's logic.
- Protection files next to the exe (`secdrv.sys`, `drvmgt.dll`) are SafeDisc leftovers; tsh.exe itself has no reference
  to them (0 hits for drvmgt) — it is already unprotected.
- `tsh.bat` kills explorer.exe before launching (colour-fix workaround) — do not use it for tests.

## Word banks (`WORD\word_NN.dat`, 68 files) — DONE
- Loader `FUN_0040c780(n)` builds `<dir>\word\word_%02d.dat`, reads the whole file and **only de-obfuscates when the file
  does not start with CR LF** (`FUN_004051b0`: `byte ^= KEY[i % len] ^ 0xA7`, KEY = `God's in His Heaven, all's right with
  the world.` at 0x4EBA0C). Then control characters are zeroed and tokens are taken in pairs
  (`FUN_0040c9f0`): **reading (kana/romaji source), display text**. Terminator byte 0x1A is appended.
- So a *plain-text* bank starting with `\r\n` works as is, no hook or patch needed. Decoded format: `\r\n` then lines
  `reading<TAB>display` in cp932, ending with a blank line.
- `tools\tsh_words.py decode|encode` (XOR round trip verified byte-exact on all 68 originals); decoded copies are in
  `wordbank\` (cp932 text, not to be published). Bank sizes: word_01 2444 lines … word_06 4040, word_41 2000; 23–40 and
  42–67 have one entry (placeholder). word_00/05 are the A–Z letters.
- NOT yet verified in game (needs a run with a custom plain bank); the reading column probably must be hiragana
  (romaji generation is done by the game, see `DAT_004e26a8` scoring table in `FUN_0040c970`).

## Next steps
1. **Obtain VC6** (user) → set up `decomp\` like TOD (`COMPILER_PLAN.md`, inventory, per-function compare).
2. Build `decomp\functions.tsv` (game vs CRT vs DX-glue split) with a copy of `re/decomp/tools/build_inventory.py`.
3. In-game check of a plain custom bank (launch tsh.exe directly, not tsh.bat).
4. Resolution: hook/replace DDraw (`ddraw.dll` proxy next to the exe, or detour `DirectDrawCreate` 0x41EEA6) to present
   the 640x480 back buffer scaled into an arbitrary window/desktop size; check whether the game uses `Lock` on the
   primary (would need a shadow surface).

## Update 2026-10-28 — VC6 supplied and verified
`E:\Microsoft Visual Studio\VC98` (user-granted access) works: cl 12.00 / link 6.00.8168 = the Rich-header build. See
`decomp\README.md` for the workflow, flags and status (10 of 484 game functions byte-matching, pipeline proven).

## Update 2026-10-28 — in-game tests
- **Interim modern display works:** dgVoodoo's `DDraw.dll` (copied from the TOD install, `tools\dgvoodoo\`) wraps the game's
  640x480 **8-bit palettized** DirectDraw mode; windowed/fullscreen aspect-correct scaling to 1920x1080 verified (title screen,
  Arcade stage 1). Toggle: `tools\tsh_dgvoodoo_enable.bat` / `..._disable.bat` (they copy/delete DDraw.dll + dgVoodoo.conf in the
  game folder). A dgVoodoo-free path would be an own `ddraw.dll` proxy (not started). Launch with the game folder as the working
  directory; do NOT use tsh.bat (kills explorer).
- **Plain word banks verified in game:** all 68 `WORD\word_NN.dat` were replaced by their decoded plain-text versions (originals in
  `WORD_orig\`); stage 1 loads and shows romaji from the plain files. So custom banks need no encryption.
- **Open display issue:** the Japanese display text of words (kanji/kana boxes above the romaji) renders as tofu on this
  non-Japanese system locale (game draws with GDI `TextOutA` + cp932 bytes; readme's "色化けする場合" note). Fix idea: hook
  `TextOutA` → `MultiByteToWideChar(932)` + `TextOutW`, or run under a Japanese locale.
- **Bank usage:** stage 1's first enemies take words from the one-entry banks (word_23 = むかでんす "MUKADENSU", other 23–40/42–67 similar):
  the game derives the romaji (typed text) from the kana reading itself. Multi-word banks (01–22, 41) serve other enemies/modes.
  An English-reading test on banks 01–22/41 was inconclusive (stage 1 start never used them) — whether ASCII readings are
  accepted by the kana→romaji step is still **unverified**; check the kana-to-romaji converter before writing English packs.
- **Kana→romaji table** (.data 0x429040–0x42CA78, 414 records of 36 bytes: ≤7-byte cp932 kana key, then NUL-separated typing
  variants, e.g. `しゃ → sya, sha, sila, sixa`, `っ`+kana doubled forms, `ー → -`, `ヴ → vu`): **hiragana only** (plus ヴ and ー).
  So bank readings must be hiragana; English/ASCII readings are NOT accepted by this table (word_00/word_05 letter banks use
  full-width single letters through another path). To get real English word banks the typing judge must be extended with a
  hook (accept ASCII readings as literal key sequences) — or an English pack can be written in kana respelling (ugly).
  Table access function not located yet (find xrefs to 0x429040).
- **Typing judge ids:** the judge `FUN_00402f10` works on character ids: 0..0x19d index the kana table above (id*0x24 + 0x429040),
  0x400–0x409 and 0x40a–0x423 (digits, letters) go to `FUN_004034c0` (letter/digit path, as used by the A–Z banks). The
  text→id conversion (reading bytes → ids) is still to be located (lane A/B functions near 0x402a80/0x4020a0); once found, an
  English-word hook can map Latin letters in a bank's reading column to the 0x40a+ ids.
