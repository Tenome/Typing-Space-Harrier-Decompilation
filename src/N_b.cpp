#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "game.h"

void Game::M_00407cd0(int a, int b, int c, int d, int e, int f, int g, int h)
{
    int i;

    if (DAT_0099cfd0 < 0x4000) {
        M_0040af40(0, DAT_0099cfd0);
        for (i = 0; i < 25; i++)
            (&m_87cbc8->m_2c)[i] = 0;
        if (a < 0)
            a = 0;
        if (b < 0)
            b = 0;
        if (c < 0)
            c = 0;
        if (d < 0)
            d = 0;
        if (e < 0)
            e = 0;
        if (f < 0)
            f = 0;
        if (g < 0)
            g = 0;
        if (h < 0)
            h = 0;
        m_87cbc8->m_0 = 0;
        m_87cbc8->m_c = 0;
        m_87cbc8->m_28 = h;
        m_87cbc8->m_24 = 0;
        m_87cbc8->m_20 = DAT_008f0ff4;
        m_87cbc8->m_2c = a;
        m_87cbc8->m_64 = b;
        m_87cbc8->m_6c = c;
        m_87cbc8->m_74 = d;
        m_87cbc8->m_7c = e;
        m_87cbc8->m_84 = f;
        m_87cbc8->m_8c = g;
        m_87cbc4->m_7d5224 = DAT_0099cfd0;
        m_87cbc4->m_7d5214 = DAT_0099cfd0;
        if (DAT_004efa14 > 0) {
            M_0040af40(0, DAT_004efa14);
            m_87cbc8->m_24 = DAT_0099cfd0;
        }
        DAT_004efa14 = DAT_0099cfd0;
        DAT_0099cfd0++;
    }
}
















