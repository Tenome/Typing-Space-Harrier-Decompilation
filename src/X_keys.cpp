#include "X_window.h"

extern int DAT_011216cc;
extern Game *DAT_011216c4;
extern int DAT_00a4625c;
extern int DAT_00a46260;
extern int DAT_00a46174;
extern int DAT_00a46178;
extern int DAT_00a4617c;
extern int DAT_00a46180;
extern int DAT_00a46184;
extern int DAT_00a46188;
extern int DAT_00a4618c;
extern int DAT_00a46190;

int TypingWindow::OnKeyDown(WPARAM key, LPARAM info)
{
    if ((info & 0x40000000) != 0) {
        switch (key) {
        case VK_UP:
            DAT_00a46184 = 1;
            break;
        case VK_DOWN:
            DAT_00a46188 = 1;
            break;
        case VK_LEFT:
            DAT_00a4618c = 1;
            break;
        case VK_RIGHT:
            DAT_00a46190 = 1;
            break;
        }
        return 1;
    }
    switch (key) {
    case VK_ESCAPE:
        DAT_00a46260 = 1;
        break;
    case VK_RETURN:
        DAT_00a4625c = 1;
        break;
    case VK_UP:
        DAT_00a46174 = 1;
        break;
    case VK_DOWN:
        DAT_00a46178 = 1;
        break;
    case VK_LEFT:
        DAT_00a4617c = 1;
        break;
    case VK_RIGHT:
        DAT_00a46180 = 1;
        break;
    case VK_F1:
        DAT_00a46264[0] = 1;
        break;
    case VK_F2:
        DAT_00a46264[1] = 1;
        break;
    case VK_F3:
        DAT_00a46264[2] = 1;
        break;
    case VK_F4:
        DAT_00a46264[3] = 1;
        break;
    }
    return 1;
}

int TypingWindow::OnSysKeyDown(WPARAM key, LPARAM info)
{
    switch (key) {
    case VK_F4:
        if (DAT_011216cc) {
            DAT_011216c4->M_004049f0();
            DAT_011216c4->M_00404d80();
        }
        DestroyWindow(m_160);
        return 1;
    case VK_F2:
        if (DAT_011216cc) {
            DAT_011216c4->M_004049f0();
            DAT_011216c4->M_00401100();
        }
        return 1;
    }
    return 1;
}
