#include <windows.h>
#include <shellapi.h>
#include "game.h"

struct EzWndClass {
    WNDCLASSEXA wc;
    const char *className;
    char title[0x100];
    DWORD style;
    int x;
    int y;
    int width;
    int height;
    HWND parent;
    HMENU menu;
    HINSTANCE instance;
    DWORD exStyle;
};

class EzWindow {
public:
    EzWndClass wc;
    HWND m_15c;
    HWND m_160;
    HANDLE m_164;
    int m_168;
    DWORD m_16c;
    int m_170;

    EzWindow();
    virtual LRESULT WindowProc(UINT msg, WPARAM wParam, LPARAM lParam);
    virtual int OnCreate();
    virtual void OnDestroy();
    virtual LRESULT OnCommand(WORD notify, WORD id, LPARAM control)
    {
        return DefWindowProc(m_160, WM_COMMAND, MAKELONG(id, notify), control);
    }
    virtual int OnActivateApp(WPARAM active) { return 0; }
    virtual int OnEnable(WPARAM enabled) { return 0; }
    virtual void OnPaint(HDC dc) {}
    virtual int OnKeyDown(WPARAM key, LPARAM info);
    virtual int OnKeyUp(WPARAM key, LPARAM info) { return 0; }
    virtual int OnChar(WPARAM ch, LPARAM info);
    virtual int OnLButtonDown(WPARAM keys, WORD x, WORD y) { return 0; }
    virtual int OnLButtonUp(WPARAM keys, WORD x, WORD y) { return 0; }
    virtual int OnRButtonDown(WPARAM keys, WORD x, WORD y) { return 0; }
    virtual int OnRButtonUp(WPARAM keys, WORD x, WORD y) { return 0; }
    virtual int OnMButtonDown(WPARAM keys, WORD x, WORD y) { return 0; }
    virtual int OnMButtonUp(WPARAM keys, WORD x, WORD y) { return 0; }
    virtual int OnSetFocus(int unused);
    virtual void OnDropFiles(HDROP drop) { DragFinish(drop); }
    virtual int OnSysCommand(UINT command, LPARAM pos, WORD y)
    {
        return command == SC_SCREENSAVE ? 1 : -1;
    }
    virtual int OnPower(int state) { return -1; }
    virtual void OnIdle();
    virtual int OnSysKeyDown(WPARAM key, LPARAM info);
    virtual int OnSysKeyUp(WPARAM key, LPARAM info);
    virtual int OnSysChar(WPARAM ch, LPARAM info);
};

class TypingWindow : public EzWindow {
public:
    TypingWindow();
    virtual int OnChar(WPARAM ch, LPARAM info);
    virtual int OnSysKeyUp(WPARAM key, LPARAM info) { return 1; }
    virtual int OnSysChar(WPARAM ch, LPARAM info) { return 1; }
    virtual int OnCreate();
    virtual void OnDestroy();
};
