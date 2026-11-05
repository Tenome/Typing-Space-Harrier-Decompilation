#include <stdlib.h>
#include "game.h"

void Game::M_0040d980()
{
    DAT_008f1008 = 0;
    DAT_008f0ffc = 0;
}

void Game::M_0040d990(int id)
{
    if (DAT_008f0ffc < 0x10) {
        M_00401400(id);
        DAT_008f1014 = &DAT_00a3d488[DAT_008f0ffc];
        DAT_008f1014->m_00 = 1;
        DAT_008f1014->m_04 = id;
        DAT_008f1014->m_08 = m_8c4->m_1c;
        DAT_008f1014->m_0c = m_8c4->m_20;
        DAT_008f1014->m_10 = m_8c4->m_24;
        DAT_008f1014->m_14 = m_8c4->m_28;
        DAT_008f1014->m_18 = DAT_008f1014->m_1c = -1;
        if (DAT_008f1008 < m_8c4->m_84)
            DAT_008f1008 = m_8c4->m_84;
        DAT_008f0ffc++;
    }
}

void Game::M_0040da60()
{
    int i;
    int j;

    DAT_008f1014 = &DAT_00a3d488[16];
    for (i = 1; i < DAT_008f0ffc; i++) {
        DAT_00a3d358 = &DAT_00a3d488[i];
        for (j = i + 1; j < DAT_008f0ffc; j++) {
            DAT_0099cfd4 = &DAT_00a3d488[j];
            if (DAT_00a3d358->m_10 < DAT_0099cfd4->m_10) {
                *DAT_008f1014 = *DAT_00a3d358;
                *DAT_00a3d358 = *DAT_0099cfd4;
                *DAT_0099cfd4 = *DAT_008f1014;
            }
        }
    }
}
