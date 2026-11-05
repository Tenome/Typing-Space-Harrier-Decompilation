#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_00405fe0(int x)
{
    int col = x / 5;
    int last;
    int ignored;

    last = DAT_00a23304 / 5;
    if (col < 2)
        col = 0;
    if (last - 2 <= col)
        col = last - 2;
    M_004060f0(&x, &ignored, 0, col * 10);
    x = x + (-0x50 - DAT_009016f4);
    M_00406070(-x, 0, 4);
}
