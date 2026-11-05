#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_00402960()
{
    char text[0x1400];
    char line[0x1402];
    int x, y;

    if (m_8c4->m_60 == 0) {
        M_00402a80(text);
        x = m_8c4->m_34 + m_8c4->m_1c;
        y = m_8c4->m_38 + m_8c4->m_20;
        M_0040e2a0(text);
        strcpy(line, "@R");
        strcat(line, text);
        M_0040bfa0(x, y, line);
    }
}

