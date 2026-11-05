#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

int FUN_004189a0(int n)
{
    if (n < 0)
        n = 0;
    if (n >= 10)
        n = 9;
    return DAT_00a4b134[n].flags & 1;
}

void FUN_0041a820(char *dir)
{
    char *p;

    strcpy(dir, GetCommandLineA() + 1);
    p = dir + strlen(dir) - 1;
    while (p > dir) {
        if (*p == 0x5c) {
            p[1] = 0;
            return;
        }
        p--;
    }
    *p = 0;
}
