#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>
#include <string.h>
#include "game.h"

LRESULT CALLBACK FUN_004180f0(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    char command[128];
    char status[128];
    int channel;
    int tries;
    int *state;
    long length;
    long position;

    switch (msg) {
    case MM_MCINOTIFY:
        if (mciCdOpen & 1) {
            for (tries = 0; tries < 3; tries++) {
                if (mciSendString("Status CDAudio Mode", status, 128, hwnd) == 0)
                    break;
            }
            if (strcmp(status, "stopped") == 0) {
                if (mciCdOpen & 2) {
                    if (DAT_00a4b160 == 0) {
                        sprintf(command, "Play CDAudio From %d Notify", DAT_00a4b15c);
                        for (tries = 0; tries < 3; tries++) {
                            if (mciSendString(command, NULL, 0, hwnd) == 0)
                                break;
                        }
                    } else {
                        sprintf(command, "Play CDAudio From %d To %d Notify", DAT_00a4b15c, DAT_00a4b160);
                        for (tries = 0; tries < 3; tries++) {
                            if (mciSendString(command, NULL, 0, hwnd) == 0)
                                break;
                        }
                    }
                } else {
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString("Close CDAudio", NULL, 0, hwnd) == 0)
                            break;
                    }
                    mciCdOpen = 0;
                }
            }
        }
        channel = 0;
        for (state = mciChannelOpen; state < mciChannelOpen + 10; channel++, state++) {
            if (*state & 1) {
                sprintf(command, "Status %s%d Mode", "awf", channel);
                for (tries = 0; tries < 3; tries++) {
                    if (mciSendString(command, status, 128, hwnd) == 0)
                        break;
                }
                if (strcmp(status, "stopped") != 0) {
                    sprintf(command, "Status %s%d Length", "awf", channel);
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString(command, status, 128, hwnd) == 0)
                            break;
                    }
                    length = strtol(status, NULL, 10);
                    sprintf(command, "Status %s%d Position", "awf", channel);
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString(command, status, 128, hwnd) == 0)
                            break;
                    }
                    position = strtol(status, NULL, 10);
                    if ((unsigned long)length > (unsigned long)position)
                        continue;
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
                }
                if (*state & 2) {
                    sprintf(command, "Play %s%d From 1 Notify", "awf", channel);
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString(command, NULL, 0, hwnd) == 0)
                            break;
                    }
                } else {
                    sprintf(command, "Close %s%d", "awf", channel);
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString(command, NULL, 0, hwnd) == 0)
                            break;
                    }
                    *state = 0;
                }
            }
        }
        break;
    case WM_DESTROY:
        if (mciCdOpen & 1) {
            for (tries = 0; tries < 3; tries++) {
                if (mciSendString("Stop CDAudio", NULL, 0, hwnd) == 0)
                    break;
            }
            for (tries = 0; tries < 3; tries++) {
                if (mciSendString("Close CDAudio", NULL, 0, hwnd) == 0)
                    break;
            }
            mciCdOpen = 0;
        }
        channel = 0;
        for (state = mciChannelOpen; state < mciChannelOpen + 10; channel++, state++) {
            if (*state & 1) {
                sprintf(command, "Stop %s%d", "awf", channel);
                for (tries = 0; tries < 3; tries++) {
                    if (mciSendString(command, NULL, 0, hwnd) == 0)
                        break;
                }
                sprintf(command, "Close %s%d", "awf", channel);
                for (tries = 0; tries < 3; tries++) {
                    if (mciSendString(command, NULL, 0, hwnd) == 0)
                        break;
                }
                *state = 0;
            }
        }
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}
