#include "game.h"

void Game::M_0040bce0()
{
    int i;
    int color;
    int tile;

    m_87cbf8 = 0;
    m_87cbf4 = 0;
    m_87cbf0 = 0;
    for (i = 0; i < 0x100; i++) {
        m_87cffc[i] = 0;
        m_87cbfc[i] = 0;
    }
    for (i = 0; i < 0xa00; i++) {
        m_87e7fc[i] = 0;
        m_87d3fc[i] = 0;
    }
    for (i = 0; i <= 8; i++) {
        color = (i & 1) > 0;
        color = (color << 8) + ((i & 4) > 0);
        color = (color << 8) + ((i & 2) > 0);
        m_87fbfc[i] = color * 255;
    }
    m_87fbfc[4] = 0x1f101;
    m_87fbfc[8] = 0xff8020;
    m_87fbfc[9] = 0xffe080;
    m_87cbf4 = 1;
    DAT_00a45e38 = 0;
    M_0040e2d0();
    M_0040f030();
    tile = m_87cbfc[11];
    for (i = 0; i < 6; i++, tile++)
        M_0040be30(tile, &m_87fc24[i], &m_87fc64[i]);
    M_0040be30(m_87cbfc[13], &m_87fca4, &m_87fca8);
}
