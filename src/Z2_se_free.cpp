#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

void Game::M_004013b0()
{
    int i;

    i = 0;
    DAT_00901700 = &DAT_004306e8[i];
    if (*DAT_00901700->name != '\0') {
        do {
            if (DAT_00901700->id != 3)
                FUN_0041e290(DAT_00901700->id);
            i++;
            DAT_00901700 = &DAT_004306e8[i];
        } while (*DAT_00901700->name != '\0');
    }
    M_004089a0(-1);
}
