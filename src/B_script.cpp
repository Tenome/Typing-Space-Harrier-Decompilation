#include <string.h>
#include "game.h"

void Game::M_00407c90(int count)
{
    memset(DAT_004f065c, 0, sizeof(DAT_004f065c));
    if (count > 0) {
        memcpy(DAT_004f065c, &DAT_00430840[DAT_004efbd4], count * sizeof(int));
        DAT_004efbd4 += count;
    }
}

void Game::M_00407e40(int a, int b, int c, int d)
{
    if (DAT_00a3d704 < 0x10000) {
        M_0040aff0(0, DAT_00a3d704);
        m_87cbd8->m_0 = 0;
        m_87cbd8->m_4 = a;
        m_87cbd8->m_8 = b;
        m_87cbd8->m_c = c;
        m_87cbd8->m_10 = d;
        DAT_00a3d704++;
        m_87cbc4->m_7d5220 = DAT_00a3d704;
        m_87cbc4->m_7d5210 = DAT_00a3d704;
    }
}

int Game::M_00407f70()
{
    M_00407fd0();
    if (m_390 == 0) {
        if (M_00408130() == 0)
            m_3d8[0xc] += m_3d8[0xd];
    }
    M_00408d00();
    M_004081b0(1);
    M_004081b0(0);
    if (m_87fcac > 0)
        m_87fcac--;
    return 0;
}
