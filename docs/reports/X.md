# Lane X report (21 functions the first Ghidra export missed)

Matched: 21 of 21 (all bytes and relocations identical, no tricks).
Files: `X_window.cpp` + `X_window.h` (window classes, virtual handlers), `X_sound_init.cpp`, `X_list.cpp`, `X_misc.cpp`,
`X_window_proc.cpp` (window procedure), `config/symbols_X.tsv`.

## Matched
- Virtual handlers of `EzWindow` (slot = vtable index of the TypingWindow vtable at 0x4271f0, message in brackets):
  3 `OnCommand(WORD notify, WORD id, LPARAM)` 0x41eb70 [WM_COMMAND, default = DefWindowProc with MAKELONG(id, notify)];
  4 `OnActivateApp` and 5 `OnEnable` 0x41ebb0 (`return 0`); 6 `OnPaint(HDC)` 0x41ebc0 (empty); 8 `OnKeyUp` 0x41ebd0;
  10..15 mouse button handlers (w, x, y) 0x41ebe0 (`return 0`, six methods, one body); 17 `OnDropFiles` 0x41ebf0
  (`DragFinish`); 18 `OnSysCommand` 0x41ec00 (`SC_SCREENSAVE ? 1 : -1`: blocks the screen saver); 19 `OnPower`
  0x41ec20 (`return -1`).
  The identical stubs are folded in the exe (one address for several methods), so `symbols_X.tsv` maps several mangled names to
  one address.
- `TypingWindow::OnSysKeyUp`/`OnSysChar` 0x41ddf0 (slots 22/23, `return 1`), `OnChar` 0x41de00 (feeds `FUN_0041e1d0` unless the
  repeat bit 0x40000000 is set), `OnCreate` 0x41de20 (slot 1: resets globals, `GetModuleFileName`, `_splitpath`, `strcpy`/`strcat`,
  `SetCurrentDirectory`; `path[0x105]` is what gives the 0x210-byte frame; returns 1), `OnDestroy` 0x41e0a0 (slot 2: MCI shutdown,
  `delete` of the game object through a local copy of `DAT_011216c4`, `FUN_00412730`, `PostQuitMessage(0)`).
- `NodeList::M_00410ff0` 0x410ff0 (virtual "delete all nodes": walks `head` through `next`, `delete`s nodes whose flag 0x10 is
  set, otherwise calls the virtual `M_00410e90`).
- The thunks 0x411870..0x4118e0 are compiler-generated dynamic initialisers, not hand-written functions: two global objects with
  constructors and destructors, `SoundMgr DAT_00a460a0;` and `NodeList DAT_00a46198;`, in one translation unit.
  `$E4` = `call $E1; jmp $E3`, `$E1` = `mov ecx,obj; jmp ctor`, `$E3` = `atexit($E2)`, `$E2` = `mov ecx,obj; jmp dtor` (0x411870/80/90/a0),
  same for the NodeList (0x4118b0..e0). They are mapped by name (`$E1`..`$E9`) in `symbols_X.tsv`. `X_sound_init.cpp` is
  self-contained (does not include `game.h`) because it needs `SoundMgr` with a constructor and destructor: 0x4118f0
  (`SoundMgr::SoundMgr`, `FUN_00411020(); m_14 = 0;`) and 0x4112d0 (`~SoundMgr`: frees every HGLOBAL in the `m_14` table, then
  `FUN_00411070()`). Cleanup for lane C: give `SoundMgr` the constructor/destructor in the shared prelude, define
  `SoundMgr DAT_00a460a0; NodeList DAT_00a46198;` in C_sound.cpp, and drop `X_sound_init.cpp`.
- 0x419e70 (lane G's `blendHalf`): `if (index >= 0xfb && index <= 0xfe) return index - 4; return index;` on the second argument; it
  is a palette index remap (0xfb..0xfe -> 0xf7..0xfa), the name `blendHalf` is probably wrong.
- Not in this lane but found: 0x41dc20 (TypingWindow::OnKeyDown, slot 7, jump table at 0x41dcd8) and 0x41dd80 (OnSysKeyDown, slot 21)
  are also missing from `functions.tsv`; the Ghidra body of 0x41d9e0 ends before them.

## Window message map (from the jump tables of 0x41e530)
WM_CREATE->OnCreate (0 / -1), WM_DESTROY->OnDestroy, WM_SETFOCUS->slot 16 (0x41df40, `V16`: deferred DirectDraw/MCI init, 0 / -1),
WM_ENABLE->OnEnable, WM_PAINT (GetUpdateRect/BeginPaint/OnPaint/EndPaint), WM_ACTIVATEAPP, WM_POWER (0x48)->OnPower, WM_KEYDOWN,
WM_KEYUP, WM_CHAR, WM_SYSKEYDOWN, WM_SYSKEYUP, WM_SYSCHAR, WM_COMMAND, WM_SYSCOMMAND, L/R/M button down/up (WM_RBUTTONDBLCLK etc. go
to DefWindowProc), WM_DROPFILES. Handlers returning non-zero return 0, otherwise DefWindowProc (OnPower/OnSysCommand: the
result unless it is -1). Slot 20 (0x41d9e0) is the idle step called from the message loop 0x41e480, not from the procedure.

## 0x41e530 `EzWindow::WindowProc` (`src/X_window_proc.cpp`)
Matched. What made the difference: the default-processing call is an inline member, `LRESULT DefaultProc(UINT msg, WPARAM w,
LPARAM l) { return DefWindowProc(m_160, msg, w, l); }`. With a direct `DefWindowProc(m_160, msg, wParam, lParam)` at every site
VC6 enregisters wParam/lParam for the whole function (1523 bytes, no tail sharing); with the inline helper the parameters stay in
memory, are cached per case in ebx/ebp, and the identical `push msg; push hwnd; call` tails are cross-jumped, exactly as in the
original. Second lever: the shape of the handler tests. `if (!OnX(...)) return DefaultProc(...); break;` (a `break` to one
shared `return 0` after the switch, `default: return DefaultProc(...)`) gives `test; jne shared_ret0`, whereas
`if (OnX(...)) return 0; return DefaultProc(...)` gives an inline `xor eax,eax` epilogue and, for the -1/0 handlers, the
`neg/sbb` collapse. WM_PAINT: `if (!GetUpdateRect(...)) break;` then BeginPaint/OnPaint/EndPaint/`return 0` (the far `je` to the
shared epilogue plus an inline epilogue). The size check counts the four byte-indexed jump tables after the code (1600 bytes
with tables vs the 996-byte code-only inventory entry).
