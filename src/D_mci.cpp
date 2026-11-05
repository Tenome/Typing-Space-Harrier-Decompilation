#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include "game.h"

extern WNDCLASSA DAT_00a4b0b8;
LRESULT CALLBACK FUN_004180f0(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/* Creates the hidden window that receives the MCI notifications. */
int FUN_00417ef0(HINSTANCE instance, HWND parent)
{
    int i;

    if (DAT_00a4b0b0 == NULL) {
        DAT_00a4b0b8.style = CS_HREDRAW | CS_VREDRAW;
        DAT_00a4b0b8.lpfnWndProc = FUN_004180f0;
        DAT_00a4b0b8.cbClsExtra = 0;
        DAT_00a4b0b8.cbWndExtra = 0;
        DAT_00a4b0b8.hInstance = instance;
        DAT_00a4b0b8.hIcon = LoadIcon(instance, IDI_APPLICATION);
        DAT_00a4b0b8.hCursor = LoadCursor(NULL, IDC_ARROW);
        DAT_00a4b0b8.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        DAT_00a4b0b8.lpszMenuName = "HJMCIWindowClassName";
        DAT_00a4b0b8.lpszClassName = "HJMCIWindowClassName";
        RegisterClass(&DAT_00a4b0b8);
        DAT_00a4b0b0 = CreateWindowEx(WS_EX_TRANSPARENT, "HJMCIWindowClassName", "HJMCIWindowClassName", WS_POPUP,
                                      0, 0, CW_USEDEFAULT, CW_USEDEFAULT, parent, NULL, instance, NULL);
        if (DAT_00a4b0b0 == NULL)
            return 0;
        mciCdOpen = 0;
        for (i = 0; i < 10; i++)
            mciChannelOpen[i] = 0;
    }
    return 1;
}

/* Stops and closes every open MCI device and destroys the notification window. */
void FUN_00417fd0()
{
    char command[128];
    int channel;
    int tries;

    if (DAT_00a4b0b0 != NULL) {
        if (mciCdOpen & 1) {
            for (tries = 0; tries < 3; tries++) {
                if (mciSendString("Stop CDAudio", NULL, 0, DAT_00a4b0b0) == 0)
                    break;
            }
            for (tries = 0; tries < 3; tries++) {
                if (mciSendString("Close CDAudio", NULL, 0, DAT_00a4b0b0) == 0)
                    break;
            }
            mciCdOpen = 0;
        }
        for (channel = 0; channel < 10; channel++) {
            if (mciChannelOpen[channel] & 1) {
                sprintf(command, "Stop %s%d", "awf", channel);
                for (tries = 0; tries < 3; tries++) {
                    if (mciSendString(command, NULL, 0, DAT_00a4b0b0) == 0)
                        break;
                }
                sprintf(command, "Close %s%d", "awf", channel);
                for (tries = 0; tries < 3; tries++) {
                    if (mciSendString(command, NULL, 0, DAT_00a4b0b0) == 0)
                        break;
                }
                mciChannelOpen[channel] = 0;
            }
        }
        DestroyWindow(DAT_00a4b0b0);
        DAT_00a4b0b0 = NULL;
    }
}

/* Stops and closes the MCI device of one BGM channel (0-9). */
int FUN_004188f0(int channel)
{
    char command[128];
    int tries;

    if (channel < 0)
        channel = 0;
    else if (channel >= 10)
        channel = 9;
    sprintf(command, "Stop %s%d", "awf", channel);
    for (tries = 0; tries < 3; tries++) {
        if (mciSendString(command, NULL, 0, DAT_00a4b0b0) == 0)
            break;
    }
    sprintf(command, "Close %s%d", "awf", channel);
    for (tries = 0; tries < 3; tries++) {
        if (mciSendString(command, NULL, 0, DAT_00a4b0b0) == 0)
            break;
    }
    mciChannelOpen[channel] = 0;
    return 1;
}
