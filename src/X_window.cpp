#include <windows.h>
#include <shellapi.h>
#include <string.h>
#include "game.h"

extern int DAT_00a46170;
extern int DAT_00a460d8;
extern int DAT_00a46258;
extern int DAT_00a460cc;
extern int DAT_00a460b8;
extern HWND DAT_00a46098;
extern char DAT_011215bc[];
extern Game *DAT_011216c4;
void FUN_0041e1d0(char ch);
void FUN_0041e2d0();
extern "C" void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);
void FUN_00417fd0();
void FUN_00412730();

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

int TypingWindow::OnChar(WPARAM ch, LPARAM info)
{
    if ((info & 0x40000000) == 0)
        FUN_0041e1d0(ch);
    return 1;
}

int TypingWindow::OnCreate()
{
    char drive[4];
    char path[0x105];
    char dir[260];

    m_170 = 0;
    DAT_00a46170 = 0;
    DAT_00a460d8 = 0;
    DAT_00a46258 = 0;
    DAT_00a460cc = 0;
    DAT_00a460b8 = 0;
    DAT_00a46098 = 0;
    DAT_011215bc[0] = 0;
    GetModuleFileNameA(0, DAT_011215bc, 0x105);
    FUN_0041e2d0();
    if (GetModuleFileNameA(0, path, sizeof path) != 0) {
        _splitpath(path, drive, dir, 0, 0);
        strcpy(path, drive);
        strcat(path, dir);
        SetCurrentDirectoryA(path);
    }
    m_168 = 1;
    return 1;
}

void TypingWindow::OnDestroy()
{
    Game *game;

    FUN_00417fd0();
    game = DAT_011216c4;
    if (game) {
        game->M_004010c0();
        delete game;
    }
    FUN_00412730();
    PostQuitMessage(0);
}

// In the original both constructors are inline in the translation unit of WinMain (E_app.cpp); here they only make
// the compiler emit the vtable and the inline virtual methods.
EzWindow::EzWindow()
{
    m_160 = 0;
}

TypingWindow::TypingWindow()
{
}
