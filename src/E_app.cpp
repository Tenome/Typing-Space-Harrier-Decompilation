#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"

extern int DAT_00a460c4;
extern char DAT_00a461d0[];
extern int DAT_00a461cc;
extern int DAT_011216c8;
extern Game *DAT_011216c4;
extern int DAT_011216cc;
extern int DAT_011216d0;
extern int DAT_00a460d4;
extern int DAT_00a460c0;
extern int DAT_00a461f4;
extern HWND DAT_00a46098;
extern HINSTANCE DAT_011215b8;
extern char *DAT_004ecfa0;
extern int FUN_00412400(HWND wnd, int w, int h, int bpp, int a, int b, int c, int d, int e);
extern int FUN_00417ef0(HINSTANCE inst, HWND wnd);
void FUN_0041e1d0(char ch);
void FUN_0041f018(unsigned int seed);
// OPEN: 0x41e190 original materializes xor eax,eax before the two rep stosd loops
void FUN_0041e190();
LRESULT CALLBACK FUN_0041ee30(HWND wnd, UINT msg, WPARAM w, LPARAM l);

/* Two further methods of the sound object (0xa460a0); lane C's SoundMgr does not list them yet. */
struct SoundMgrExt : SoundMgr {
    void M_00411340(int a);
    int M_004114d0(const char *path, int id);
};
#define SOUND (*(SoundMgrExt *)&DAT_00a460a0)

/* Thin window-class wrapper: a WNDCLASSEX followed by the creation parameters. */
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

    EzWndClass(const char *cls, HINSTANCE inst);
    int M_0041ed00(const char *name);
    void M_0041ed50();
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

    EzWindow(const char *cls, HINSTANCE inst) : wc(cls, inst)
    {
        m_160 = 0;
        m_164 = 0;
        m_168 = 0;
        m_170 = 0;
    }
    virtual LRESULT M_0041e530(UINT msg, WPARAM w, LPARAM l);
    virtual void V01();
    virtual void V02();
    virtual void V03();
    virtual void V04();
    virtual void V05();
    virtual void V06();
    virtual void V07();
    virtual void V08();
    virtual void V09();
    virtual void V10();
    virtual void V11();
    virtual void V12();
    virtual void V13();
    virtual void V14();
    virtual void V15();
    virtual int V16(int unused);
    virtual void V17();
    virtual void V18();
    virtual void V19();
    virtual void V20();

    int M_0041e300();
    WPARAM M_0041e480(int poll);
};

class TypingWindow : public EzWindow {
public:
    TypingWindow(const char *cls, HINSTANCE inst) : EzWindow(cls, inst) {}
    virtual int V16(int unused);
    virtual void V20();
    void M_0041e060(int mode);
};

/* ---- application window (0x41d9e0-0x41e0d0) ---- */

// OPEN: 0x41d9e0 register choices (ecx/eax/edx) in cases 2 and 3; 4 bytes larger than original
void TypingWindow::V20()
{
    DWORD now;

    switch (m_170) {
    case 0:
        DAT_00a460c8 = 0;
        DAT_011216c8 = 0;
        DAT_00a460bc = 0x18;
        DAT_00a461cc = 0x29;
        SOUND.M_00411340(0x40);
        DAT_00a460a0.FUN_00411100(m_160, 1);
        DAT_011216c4 = new Game;
        DAT_011216cc = 1;
        m_170++;
        break;
    case 1:
        m_16c = timeGetTime() + DAT_00a461cc;
        m_170++;
        break;
    case 2:
        if (DAT_00a460c8 != 0) {
            DAT_00a460c4 = 0;
            DAT_00a461d0[0] = 0;
            if (DAT_00a460c8 != -20000) {
                if (DAT_00a460c8 != -10000) {
                    if (DAT_00a460c8 >= 0x20)
                        FUN_0041e1d0(DAT_00a460c8);
                    goto frame;
                }
                DAT_011216c8 = 0;
                exit(1);
            }
            DAT_011216c8 = 1;
            DestroyWindow(m_160);
        }
frame:
        DAT_011216c4->M_00401120();
        FUN_0041e190();
        m_170++;
        break;
    case 3:
        now = timeGetTime();
        if (m_16c < now || now + DAT_00a461cc < m_16c) {
            DAT_011216d0 = DAT_011216d0 == 0;
            if (!FUN_00412800())
                FUN_004127a0();
            m_16c += DAT_00a461cc;
            if (now > m_16c)
                m_16c = timeGetTime() + DAT_00a461cc;
            m_170 = 2;
        }
        break;
    default:
        m_170++;
        if (m_170 > 99999)
            m_170 = 10000;
    }
}

// OPEN: 0x41df40 original reuses eax=0 as MessageBoxA uType and hoists push 0; mine pushes constants
int TypingWindow::V16(int)
{
    if (m_168 != 0) {
        m_168 = 0;
        if (FUN_00412400(m_160, 0x280, 0x1e0, 8, 0x11, 0x280, 0x960, -1, -1) == 0) {
            MessageBoxA(m_160, "DirectDraw Init FAILED", DAT_004ecfa0, 0);
            DestroyWindow(m_160);
            return 0;
        }
        if (FUN_00417ef0(DAT_011215b8, m_160) == 0) {
            MessageBoxA(m_160, "MCI Init FAILED", DAT_004ecfa0, 0);
            DestroyWindow(m_160);
            return 0;
        }
        DAT_00a460d4 = SystemParametersInfoA(0x5c, 1, 0, 0) != 0;
        FUN_0041f018(GetTickCount());
        DAT_00a460c0 = 1;
        DAT_00a461f4 = 0x32;
        m_170 = 0;
        M_0041e060(0);
        DAT_00a46098 = m_160;
    }
    return 1;
}

void FUN_0041e190()
{
    int *p;

    for (p = &DAT_00a46174; p < &DAT_00a46174 + 8; p++)
        *p = 0;
    for (p = DAT_00a46264; p < DAT_00a46264 + 8; p++)
        *p = 0;
    DAT_00a4625c = 0;
    DAT_00a46260 = 0;
    DAT_00a460c4 = 0;
    DAT_00a461d0[0] = 0;
}

void FUN_0041e1d0(char ch)
{
    if (DAT_00a460c4 < 0x1f) {
        if ((ch >= '0' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '\'' || ch == '-' || ch == ',' || ch == '.' || ch == '/') {
            DAT_00a461d0[DAT_00a460c4] = ch;
            DAT_00a460c4++;
            DAT_00a461d0[DAT_00a460c4] = 0;
        }
    }
}

void TypingWindow::M_0041e060(int mode)
{
    HMODULE lib;
    FARPROC fn;

    if (mode == 0) {
        lib = LoadLibraryA("USER32");
        if (lib != 0) {
            fn = GetProcAddress(lib, "WINNLSEnableIME");
            if (fn != 0)
                ((BOOL (WINAPI *)(HWND, BOOL))fn)(0, 0);
            FreeLibrary(lib);
        }
    }
}

int WINAPI FUN_0041e0d0(HINSTANCE inst, HINSTANCE prev, LPSTR cmdLine, int show)
{
    DAT_011215b8 = inst;
    TypingWindow wnd("typing_class", inst);
    wnd.wc.M_0041ed00(DAT_004ecfa0);
    ShowCursor(0);
    wnd.wc.width = 640;
    wnd.wc.height = 480;
    wnd.wc.M_0041ed50();
    if (!wnd.M_0041e300())
        return 0;
    ShowWindow(wnd.m_160, show);
    return wnd.M_0041e480(1);
}

void FUN_0041e220(int id)
{
    DAT_00a460a0.FUN_00411120(id, 0);
}

void FUN_0041e240(int id)
{
    DAT_00a460a0.FUN_00411120(id, 1);
}

int FUN_0041e260(const char *path, int id)
{
    int result;

    result = SOUND.M_004114d0(path, id);
    DAT_00a460a0.FUN_004111f0(id, 0);
    return result;
}

void FUN_0041e290(int id)
{
    DAT_00a460a0.FUN_00411190(id);
}

void FUN_0041e2a0(int a, int b)
{
    DAT_00a460a0.FUN_00411260(a, b);
}

int FUN_0041e2c0()
{
    return -10000;
}

void FUN_0041e2d0()
{
    char *lastSlash = DAT_011215bc;
    char *dst = DAT_011215bc;
    char *src = DAT_011215bc;

    while (*src != 0) {
        if (*src != '"') {
            if (*src == 0x5c)
                lastSlash = dst + 1;
            *dst = *src;
            dst++;
        }
        src++;
    }
    *lastSlash = 0;
}

// OPEN: 0x41e300 original computes the AdjustWindowRect menu flag with jne/xor before storing the rect
int EzWindow::M_0041e300()
{
    RECT rc;

    if (m_160 != 0)
        return 0;
    m_164 = CreateMutexA(0, 1, wc.className);
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;
    wc.wc.cbWndExtra = 4;
    if (RegisterClassExA(&wc.wc) == 0)
        return 0;
    rc.bottom = wc.height;
    rc.right = wc.width;
    rc.left = 0;
    rc.top = 0;
    BOOL hasMenu = TRUE;
    if (wc.menu == 0)
        hasMenu = FALSE;
    if (AdjustWindowRect(&rc, wc.style, hasMenu) == 0)
        return 0;
    if (rc.top < 0) {
        rc.bottom -= rc.top;
        rc.top = 0;
    }
    if (rc.left < 0) {
        rc.right -= rc.left;
        rc.left = 0;
    }
    if (wc.exStyle == 0)
        m_160 = CreateWindowExA(0, wc.className, wc.title, wc.style, wc.x, wc.y, rc.right - rc.left, rc.bottom - rc.top, wc.parent, wc.menu, wc.instance, this);
    else
        m_160 = CreateWindowExA(wc.exStyle, wc.className, wc.title, wc.style, wc.x, wc.y, rc.right - rc.left, rc.bottom - rc.top, wc.parent, wc.menu, wc.instance, this);
    return m_160 != 0;
}

WPARAM EzWindow::M_0041e480(int poll)
{
    MSG msg;

    if (poll == 0) {
        while (GetMessageA(&msg, 0, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    } else {
        for (;;) {
            if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT)
                    break;
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            } else {
                V20();
            }
        }
    }
    return msg.wParam;
}

EzWndClass::EzWndClass(const char *cls, HINSTANCE inst)
{
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = FUN_0041ee30;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = inst;
    wc.hIcon = LoadIconA(0, IDI_APPLICATION);
    wc.hCursor = LoadCursorA(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = cls;
    wc.hIconSm = LoadIconA(0, IDI_APPLICATION);
    className = cls;
    wsprintfA(title, "EzWindowSystem");
    x = 0;
    y = 0;
    parent = 0;
    menu = 0;
    exStyle = 0;
    style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    width = 320;
    height = 240;
    instance = inst;
}

int EzWndClass::M_0041ed00(const char *name)
{
    int ok = 0;

    if (strlen(name) < 250) {
        wsprintfA(title, "%s", name);
        ok = 1;
    }
    return ok;
}

// OPEN: 0x41ed50 same menu flag shape as 0x41e300
void EzWndClass::M_0041ed50()
{
    RECT rc;
    RECT desk;
    int w;
    int h;

    rc.bottom = height;
    rc.right = width;
    rc.left = 0;
    rc.top = 0;
    BOOL hasMenu = TRUE;
    if (menu == 0)
        hasMenu = FALSE;
    if (AdjustWindowRect(&rc, style, hasMenu)) {
        if (rc.top < 0) {
            rc.bottom -= rc.top;
            x = 0;
            rc.top = 0;
        }
        if (rc.left < 0) {
            rc.right -= rc.left;
            y = 0;
            rc.left = 0;
        }
        h = rc.bottom - rc.top;
        w = rc.right - rc.left;
        GetWindowRect(GetDesktopWindow(), &desk);
        x = ((desk.right - desk.left) - w) / 2;
        y = ((desk.bottom - desk.top) - h) / 2;
    }
}

LRESULT CALLBACK FUN_0041ee30(HWND wnd, UINT msg, WPARAM w, LPARAM l)
{
    EzWindow *self = (EzWindow *)GetWindowLongA(wnd, 0);

    if (self == 0) {
        if (msg == WM_CREATE) {
            self = (EzWindow *)((CREATESTRUCTA *)l)->lpCreateParams;
            SetWindowLongA(wnd, 0, (LONG)self);
            self->m_15c = wnd;
        } else {
            return DefWindowProcA(wnd, msg, w, l);
        }
    }
    return self->M_0041e530(msg, w, l);
}
