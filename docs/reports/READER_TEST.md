# Reader-test pass (orchestrator, 2026-10-30) — 308 byte-matched game functions

Mechanical scan of every src\*.cpp (open files excluded for the "matched" claims):
- No `__asm`, `#pragma`, `__declspec(naked)`, `volatile`, hex-address casts, or decompiler placeholder names (iVar/uVar/local_/param_/undefined).
- One `RULE-EXCEPTION` (A_menu.cpp 0x406350, interior-pointer null test, bytes require it). Counted as matched under the rules by lane A.
- `unused` appears twice: E_app.cpp (a vtable method parameter, harmless) and G_menu.cpp (an ignored out-parameter of M_00406290, fine).

Hand-read sample: 0x405770 (A_menu), 0x408f60 (F_misc), H_list.cpp (list classes), B_misc 0x40af40, A/C/E spot checks. They read as
ordinary source: named locals, switches where the bytes show jump tables, real struct/class types.

Weak spots (not fakematches, but type recovery still owed — listed for a finishing pass):
1. B_misc.cpp 0x40af40/0x40aff0: `(Rec_87cbd4 *)(m_87cbc4->m_0 + n * 400)` — an int base address cast to a record pointer. `m_0` is
   really a pointer into a record array; once typed, the casts disappear.
2. F_misc.cpp 0x408f60: reads one array element as `short entry` and then `int value = entry` — bytes need both widths; probably an
   original `short` array element used as int.
3. Names are still placeholders (M_xxxxxxxx, m_XXXX, DAT_/FUN_): by project ruling, renamed only when certain. A naming pass is the
   biggest readability gain left.
4. J_open.cpp (not matched) holds 48 casts: expected for a parked draft; must be cleaned if those functions ever match.
Verdict: no fakematch found among the matched functions. Matched-under-rules count = bytes-matched count, except lane A's 0x402e90 etc.
were cleaned (all 88 A functions now pass; 0x406350 carries the RULE-EXCEPTION).
