#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "game.h"

void Game::M_004089a0(int n)
{
    char number[0x20];
    char path[0x108];

    if (n < 0) {
        FUN_004188f0(0);
        FUN_004188f0(1);
        return;
    }
    wsprintfA(number, "%02d", n);
    strcpy(path, DAT_011215bc);
    strcat(path, "BGM\\");
    strcat(path, number);
    strcat(path, ".WAV");
    FUN_004185a0(path, DAT_00a4604c, 1, 0, 0);
    DAT_00a4604c = (DAT_00a4604c == 0);
    FUN_004188f0(DAT_00a4604c);
}
