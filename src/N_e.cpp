#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_0040ef00()
{
    PaletteTable palette;
    int level;
    int phase;

    FUN_004139f0(&palette);
    palette.entry[255].b = 0xff;
    palette.entry[255].g = 0xff;
    palette.entry[255].r = 0xff;
    phase = DAT_004e2774[m_8a8 % 32u];
    if (m_a795c == 7) {
        level = phase / 4 + 0xbf;
        if (level < 0)
            level = 0;
        else if (level > 0xff)
            level = 0xff;
        palette.entry[8].r = level;
        palette.entry[8].g = 0x20;
        palette.entry[8].b = level;
    } else {
        level = phase / 2 + 0xbf;
        if (level < 0)
            level = 0;
        else if (level > 0xff)
            level = 0xff;
        palette.entry[8].r = level;
        palette.entry[8].b = 0;
        palette.entry[8].g = 0;
    }
    level = phase + 0xe7;
    if (level < 0)
        level = 0;
    else if (level > 0xff)
        level = 0xff;
    palette.entry[54].r = level;
    if (m_a795c == 2) {
        level = phase + 0xf7;
        if (level < 0)
            level = 0;
        else if (level > 0xff)
            level = 0xff;
        palette.entry[201].r = level;
        level = phase + 0xc6;
        if (level < 0)
            level = 0;
        else if (level > 0xff)
            level = 0xff;
        palette.entry[202].r = level;
    }
    FUN_00413a20(&palette);
    FUN_00413a50();
}





