#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

void Game::M_004012a0()
{
    char path[0x108];
    char c;
    int i;

    i = 0;
    DAT_00901700 = &DAT_004306e8[i];
    c = *DAT_00901700->name;
    while (c != '\0' && c != '*') {
        strcpy(path, DAT_011215bc);
        strcat(path, "SE\\");
        strcat(path, DAT_00901700->name);
        strcat(path, ".WAV");
        FUN_0041e260(path, DAT_00901700->id);
        i++;
        DAT_00901700 = &DAT_004306e8[i];
        c = *DAT_00901700->name;
    }
}
