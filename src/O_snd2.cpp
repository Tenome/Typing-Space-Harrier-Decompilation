#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"

void Game::M_00410460(int mode)
{
    int n;
    int i;

    DAT_00a460bc = 0x1e;
    if (mode > -100) {
        if (mode < -1)
            mode = 3;
        n = 0;
        for (i = 0; DAT_004e28dc[i] > 0; i++)
            n = i + 1;
        if (mode < 0 || n <= mode)
            mode = 2;
        DAT_00a461cc = 1000 / DAT_004e28dc[mode];
        if (DAT_00a461cc > 0)
            return;
    }
    DAT_00a461cc = 1;
}
