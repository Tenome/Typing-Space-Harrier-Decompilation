#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_00409db0()
{
    int i;

    i = 0;
    do {
        M_00409ca0(i);
        if (DAT_004efa18->m_0 != 0) {
            DAT_004efa18->m_10--;
            if (DAT_004efa18->m_10 <= 0) {
                M_00409700(DAT_004efa18->m_4, DAT_004efa18->m_8, DAT_004efa18->m_c);
                M_00409ce0(i);
            }
        }
        i++;
    } while (i < 0x40);
}
