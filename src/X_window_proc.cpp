#include "X_window.h"

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
        break;
    case WM_DESTROY:
        OnDestroy();
        return 0;
    case WM_ENABLE:
        if (!OnEnable(wParam))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_PAINT:
        if (!GetUpdateRect(m_160, &updateRect, FALSE))
            break;
        dc = BeginPaint(m_160, &paint);
        OnPaint(dc);
        EndPaint(m_160, &paint);
        return 0;
    case WM_SETFOCUS:
        if (!OnSetFocus(wParam))
            return -1;
        break;
    case WM_ACTIVATEAPP:
        if (!OnActivateApp(wParam))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_KEYDOWN:
        if (!OnKeyDown(wParam, lParam))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_KEYUP:
        if (!OnKeyUp(wParam, lParam))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_CHAR:
        if (!OnChar(wParam, lParam))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_SYSKEYDOWN:
        if (!OnSysKeyDown(wParam, lParam))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_POWER:
        result = OnPower(wParam);
        if (result == -1)
            return DefaultProc(msg, wParam, lParam);
        return result;
    case WM_SYSKEYUP:
        if (!OnSysKeyUp(wParam, lParam))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_COMMAND:
        return OnCommand(HIWORD(wParam), wParam, lParam);
    case WM_SYSCHAR:
        if (!OnSysChar(wParam, lParam))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_LBUTTONDOWN:
        if (!OnLButtonDown(wParam, lParam, HIWORD(lParam)))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_LBUTTONUP:
        if (!OnLButtonUp(wParam, lParam, HIWORD(lParam)))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_SYSCOMMAND:
        result = OnSysCommand(wParam, lParam, HIWORD(lParam));
        if (result == -1)
            return DefaultProc(msg, wParam, lParam);
        return result;
    case WM_RBUTTONDOWN:
        if (!OnRButtonDown(wParam, lParam, HIWORD(lParam)))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_RBUTTONUP:
        if (!OnRButtonUp(wParam, lParam, HIWORD(lParam)))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_MBUTTONDOWN:
        if (!OnMButtonDown(wParam, lParam, HIWORD(lParam)))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_MBUTTONUP:
        if (!OnMButtonUp(wParam, lParam, HIWORD(lParam)))
            return DefaultProc(msg, wParam, lParam);
        break;
    case WM_DROPFILES:
        OnDropFiles((HDROP)wParam);
        break;
    default:
        return DefaultProc(msg, wParam, lParam);
    }
    return 0;
}
