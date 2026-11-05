#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_00406540(int dx, int dy)
{
    int before, after;
    int i, v, limit;

    before = M_00406350(DAT_00951fcc->m_14[0], DAT_00951fcc->m_14[1]);
    DAT_00951fcc->m_14[0] += dx;
    DAT_00951fcc->m_14[1] += dy;
    for (i = 0; i < 2; i++) {
        v = DAT_00951fcc->m_14[i];
        limit = DAT_00951fcc->m_1c[i];
        if (DAT_00951fcc->m_8[i] == 1) {
            if (v >= limit)
                v = limit - 1;
            if (v < 0)
                v = 0;
        } else {
            if (v >= limit)
                v -= limit;
            if (v < 0)
                v += limit;
        }
        DAT_00951fcc->m_14[i] = v;
    }
    after = M_00406350(DAT_00951fcc->m_14[0], DAT_00951fcc->m_14[1]);
    if (before != after) {
        m_8ac = 0;
        M_00408650(DAT_00951fcc->m_38);
    }
}
