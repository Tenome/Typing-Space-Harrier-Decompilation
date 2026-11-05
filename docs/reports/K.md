# Lane K report: initialised data (.data / .rdata / string literals)

## Result
- `.data` has 345,781 non-zero bytes. **312,943 (90.5 %) are produced byte-identically by typed definitions in `src\K_*.cpp`**
  (771,252 bytes of data symbols including zero bytes inside the arrays; 133 symbols, all MATCH, `progress_K.tsv`).
- Not covered by K (all non-zero bytes): 31,570 B string literals used by code (pool 0x4e2978-0x4ec3c8, 0x4ecb28-0x4ecfa0,
  see "String pool"), 1,154 B libc data (0x429004-0x42903b init table, 0x4eed90-0x4ef900), 114 B window/class literals at the
  tail of the game pool. `.rdata` (3 KB: floats, vtables, import names, CRT) and `.rsrc` (616 B resource directory) are not data
  of lane K: compiler/linker/resource-compiler output.
- Measured with `build\K\coverage.py` (coverage) and `build\K\literals.py` (which pool literals a lane object already produces:
  34,211 of 40,203 pool string bytes already exist as string constants in some lane's .obj, content-level).

## Tools
- `tools\compare_data.py <obj...> [--progress progress_K.tsv] [--verbose] [--quiet]`: for every data symbol (DAT_xxxxxxxx /
  `?DAT_xxxxxxxx@...` / names in symbols*.tsv) of the .data/.rdata sections of a VC6 object: byte compare with tsh.exe at the
  address; DIR32 relocations to resolvable names must equal the original pointer; relocations to the object's own string literals
  are checked by content (the original pointer must lead to the same bytes and NUL; one literal must not be used at two
  addresses); other relocations are masked and counted. Up to 7 zero bytes of alignment gap at the end of a symbol are ignored.
  VC6 puts each string literal in its own `.data` COMDAT section, so symbol extents are taken per section.
- `data_build_K.bat [K_x.cpp ...]` (set `DATA_FLAGS=--quiet`): gen.bat + cl /O2 + compare_data. `data_gen_K.py` + `data_spec_K.py`
  generate `src\K_*.cpp` from the image (types/extents are the decision, values come from tsh.exe); regenerate with Python313.
- Known tool limit: `gen_game_h.py` reads `src\*.cpp` as UTF-8, so K sources contain only ASCII + UTF-8 comments; Shift-JIS strings
  are octal-escaped with the decoded text in a trailing comment.

## What the data is (all typed, nothing opaque)
| address | symbol / type | evidence |
|---|---|---|
| 0x429040 (14,904 B) | `RomajiEntry[0x19e]` | kana key, 4 typing variants, 1 flag byte (lane A's `pad23` is read as flags, values 0/16/87/216/223/248/255) |
| 0x4ecfa8 (7,488 B) | `RomajiEntry[208]` (new) | second kana table (compound kana: きゃ kya kilya kixya ... ヴょ), last record all zero. No code reference; indexed through a computed base. Preceded by `char *DAT_004ecfa0 = "TYPING SPACE HARRIER"` (window title) |
| 0x42ca9c, 0x42d5dc | `int[720]` x2 | 16.16 cos / sin at 0.5 degree steps (F_misc.cpp `DAT_0042ca9c[angle*2]`) |
| 0x42e11c-0x430840 | `char *` x4, `char *[]` group names (129, empty-string terminated), `int[128]` per-group counts, `char *[]` BMP file names (terminated by DAT_00a46054 = ""), `int` no-index -1, character / boss name tables, `SeEntry[44]` | decompiled loader 0x40e380/0x40e570 (loop ends at `**p == 0`) |
| **0x430840-0x4bd548 (576,780 B)** | `int DAT_00430840[]` stage timeline script | NOT a binary blob: an op + fixed-length-argument stream read by the interpreter FUN_004078f0 through FUN_00407c90(count) (DAT_004efbd4 = read position). Ops -100000 (reset, 0 args), -99999 (group, 1), -99998 (add object, 8), -99997 (end, 0), -99996..-99990 (seven event lanes, 4 args). The stream parses cleanly from the first to the last command (about 27,000 commands, 25 trailing END ops, then a -1). Emitted as one command per line with an `enum StageOp` (names invented from the interpreter; argument meaning not recovered). The original source was almost certainly generated. |
| 0x4bd54c-0x4bdd70 | option / menu tables: `int[]`, `char *[]` (labels "@2DIFFICULTY" ... "@kローマ字の設定", kana rows for the romaji editor), `short[]` x2 (0x4bd924: 202, 0x4bdab8: 250 entries), scalars declared by lanes A/B | split automatically at declared objects and where pointer runs change to integer runs |
| 0x4bdd70 | `BgmFile[17]` (size, "NN.WAV"), terminator {0, ""} | lane C |
| **0x4bddf8-0x4e0ef8 (143,616 B)** | `StageRow[1632]` (22 ints, 0x58 stride) | per-stage event rows; two all -1 rows terminate (A_field.cpp loops until m_10 < 0 && m_1c < 0). This is the other big "binary region": it is an array of the existing struct. |
| 0x4e0ef8 | `Rec14[187]` (5 ints), last row zero | lane C |
| 0x4e1d94-0x4e2978 | menu / result / item tables, `WaveRow[4]` (0x4e22e0), tables of `char *` incl. boss names, `char DAT_004e2970[8]` | lanes A/B/F/G declarations |
| 0x4ebb14-0x4ebbc4 | int flags = 1, `unsigned char[4] {0x20,0x7f,0xa1,0xe0}`, `unsigned short[72]` Shift-JIS lead-byte ranges | lane D (FUN_004136f0/413870/413900); objects sit inside the literal pool |
| 0x4ebd60-0x4ec3bc | `char *` ("bg.bin"), `int[254]` key record tables (4-int and 9-int records read by FUN_004189d0), `Rgb[144]` colour ramps (3 bytes each, 12..24 entries per key outline), `int[]` | lane E; the int tables are typed flat (structure not recovered) |
| 0x4ec3c8-0x4ecb28 | `KeyDef[48]`, `IconPos[12]`, `NameTiming[15]`, `LabelDef[48]`, tutorial title tables | lane E/J |

## BLOB regions
None is an opaque byte blob. Borderline items, typed but weakly:
- **Stage script (576,780 B at 0x430840)**: typed as `int` stream with opcode enum; the arguments are unnamed ints. If the
  user wants it as readable source it needs a script macro layer (the interpreter's argument meanings: x, y, kind, timing, ...).
- `int[254]` at 0x4ebd68 and `int[]` at 0x4ec310 (lane E key tables): typed `int`, record layout unknown (4-int and 9-int strides seen).
- cos/sin tables are generated numbers (fine as `int[720]`).

## String pool (not mine, listed for the orchestrator)
Literals live in 0x4e2978-0x4eed90 in **reverse order of first use** (VC6 emits each literal in its own COMDAT; the linker
orders them last-in-first), each padded to 4 bytes, interleaved with a few data objects (listed above). They are produced by
compiling every translation unit; layout/order can only be verified when the whole exe is linked in original object order.
Table pointers into the pool are verified by content by `compare_data.py` (322 B referenced only from tables).

## Requests for the orchestrator / other lanes (nothing was edited outside lane K files)
1. Lane E: `DAT_004ec3cc[0x31]` has 3 entries (KeyDef table starts at 0x4ec3d8); K defines it in `K_open.cpp` with 3 entries.
   `DAT_004ec7a8`/`DAT_004ec7b8` are `const unsigned char *` in E but initialised from string literals (only valid in C): K defines them
   as `const char *` in `K_open.cpp`; change E's declarations to `const char *` and K_open's two definitions move into K_keyboard.
   `DAT_004ebb2b`, `DAT_004ebb2d`, `DAT_004ebb32` are views of the same objects as `DAT_004ebb2c` (uchar[4] {0x20,0x7f,0xa1,0xe0})
   and the int at 0x4ebb30; only `DAT_004ebb2c`, `DAT_004ebb30`, `DAT_004ebb34` are defined. `DAT_004e2d68` is the address of the
   literal "０" (a string in the pool), not a data object, so no definition exists.
2. Lane C: `DAT_004bde00` and `DAT_004bde14` lie inside `StageRow DAT_004bddf8[]` (row 0: m_8 / m_1c ...); K defines only the StageRow
   array, so these names need to become `DAT_004bddf8[i].m_xx` accesses. Same for lane B's `DAT_004bd608` (really `int[8]` option counts;
   K keeps the three scalars lane A/B declare: 0x4bd608/60c/610, the rest is `int DAT_004bd614[]`).
3. Lane A: `RomajiEntry::pad23` is read as flags in A_open.cpp (values 0,16,87,216,223,248,255), so it is data, not padding; rename.
4. The stage script enum names (`STAGE_OP_*`) are K's; lane B may adopt them in FUN_004078f0 (`case -100000` etc.).
5. `include\game.h` generation breaks on non-UTF-8 sources (K avoids them); if a lane writes raw Shift-JIS into src, gen_game_h.py
   needs `errors="replace"` or latin-1.

## Open
- String-literal pool order / the 5,992 pool bytes (1,314 strings) with no producing object yet (functions not yet decompiled).
- libc data (init table 0x429000, 0x4eed90+) and `.rdata`/`.rsrc`: come from libc.lib, compiler and resource compiler.
- Final link order of the data objects (K objects are one TU per file; the original interleaves them per source file).
