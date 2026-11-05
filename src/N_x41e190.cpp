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
