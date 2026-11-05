#include <string.h>
#include "X_window.h"

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
