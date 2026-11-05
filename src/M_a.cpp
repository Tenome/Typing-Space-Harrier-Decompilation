#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_00402200()
{
    int i;
    int w;
    int color;
    int x;
    Slot *s = m_8c4;

    if (s->m_18 != 0 && s->m_60 == 0) {
        int remaining = s->m_84 - s->m_80;
        int y = s->m_30 + s->m_20;
        for (i = 0; i < 5; i++) {
            if (remaining <= 0)
                break;
            w = DAT_00a460bc * 5;
            if (remaining < DAT_00a460bc * 10)
                w = (DAT_00a460bc * remaining * 5) / (DAT_00a460bc * 10);
            color = DAT_004e1d94[i];
            remaining -= DAT_00a460bc * 10;
            x = (m_8c4->m_2c + m_8c4->m_1c) - w / 2;
            if (i == 0)
                M_004022e0(x + 2, y + 2, w, 4, 0);
            M_004022e0(x, y, w, 4, color);
        }
    }
}
