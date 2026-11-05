#include "X_window.h"

// OPEN: 0x41e530 window procedure. Switch shape (byte-table jump tables for 1..0xf, 0x48..0x104, 0x106..0x202,
// 0x205..0x233) and the case set are right, but the original keeps wParam/lParam in memory and caches them per case
// in ebx/ebp (996 bytes, tails cross-jumped into one DefWindowProc call); mine enregisters both for the whole
// function (1523 bytes). Levers tried: break vs return forms, if(!x) return -1 forms, /GX (no effect).
LRESULT EzWindow::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
    RECT updateRect;
    PAINTSTRUCT paint;
    HDC dc;
    LRESULT result;

    switch (msg) {
    case WM_CREATE:
        if (!OnCreate())
            return -1;
        return 0;
    case WM_DESTROY:
        OnDestroy();
        return 0;
    case WM_ENABLE:
        if (OnEnable(wParam))
            return 0;
        break;
    case WM_PAINT:
        if (GetUpdateRect(m_160, &updateRect, FALSE)) {
            dc = BeginPaint(m_160, &paint);
            OnPaint(dc);
            EndPaint(m_160, &paint);
        }
        return 0;
    case WM_SETFOCUS:
        if (!OnSetFocus(wParam))
            return -1;
        return 0;
    case WM_ACTIVATEAPP:
        if (OnActivateApp(wParam))
            return 0;
        break;
    case WM_KEYDOWN:
        if (OnKeyDown(wParam, lParam))
            return 0;
        break;
    case WM_KEYUP:
        if (OnKeyUp(wParam, lParam))
            return 0;
        break;
    case WM_CHAR:
        if (OnChar(wParam, lParam))
            return 0;
        break;
    case WM_SYSKEYDOWN:
        if (OnSysKeyDown(wParam, lParam))
            return 0;
        break;
    case WM_POWER:
        result = OnPower(wParam);
        if (result == -1)
            break;
        return result;
    case WM_SYSKEYUP:
        if (OnSysKeyUp(wParam, lParam))
            return 0;
        break;
    case WM_COMMAND:
        return OnCommand(HIWORD(wParam), wParam, lParam);
    case WM_SYSCHAR:
        if (OnSysChar(wParam, lParam))
            return 0;
        break;
    case WM_LBUTTONDOWN:
        if (OnLButtonDown(wParam, lParam, HIWORD(lParam)))
            return 0;
        break;
    case WM_LBUTTONUP:
        if (OnLButtonUp(wParam, lParam, HIWORD(lParam)))
            return 0;
        break;
    case WM_SYSCOMMAND:
        result = OnSysCommand(wParam, lParam, HIWORD(lParam));
        if (result == -1)
            break;
        return result;
    case WM_RBUTTONDOWN:
        if (OnRButtonDown(wParam, lParam, HIWORD(lParam)))
            return 0;
        break;
    case WM_RBUTTONUP:
        if (OnRButtonUp(wParam, lParam, HIWORD(lParam)))
            return 0;
        break;
    case WM_MBUTTONDOWN:
        if (OnMButtonDown(wParam, lParam, HIWORD(lParam)))
            return 0;
        break;
    case WM_MBUTTONUP:
        if (OnMButtonUp(wParam, lParam, HIWORD(lParam)))
            return 0;
        break;
    case WM_DROPFILES:
        OnDropFiles((HDROP)wParam);
        return 0;
    }
    return DefWindowProc(m_160, msg, wParam, lParam);
}
