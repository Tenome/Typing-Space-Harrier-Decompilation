#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"
#include "game.h"
void Game::M_00410460(int mode)
{
    int n;
    int i;
    int *p;

    DAT_00a460bc = 0x1e;
    if (mode > -100) {
        if (mode < -1)
            mode = 3;
        n = 0;
        if (DAT_004e28dc[0] > 0) {
            i = 1;
            p = DAT_004e28dc;
            do {
                n = i;
                p++;
                i = n + 1;
            } while (*p > 0);
        }
        if (mode < 0 || n <= mode)
            mode = 2;
        DAT_00a461cc = 1000 / DAT_004e28dc[mode];
        if (DAT_00a461cc > 0)
            return;
    }
    DAT_00a461cc = 1;
}
