#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"

void Game::M_00410340()
{
    DAT_004f0530 = 0;
    memset(DAT_0099d5bc, 0, sizeof(DAT_0099d5bc));
}
