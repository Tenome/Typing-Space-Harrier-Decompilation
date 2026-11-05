#include "game.h"

void Game::M_0040a6d0()
{
    int x;
    int y;
    int shift;
    int * stage;
    int next;
    if (m_39c <= 0) {
        y = (m_87cbc4->m_7d5230 - 0x1e0) * 0xcc / 0x141 + 0x1ac;
        x = m_87cbc4->m_7d522c;
        shift = (x - 0x140) / 16;
        m_87cbc4->m_7d523c = y;
        stage = m_3d8;
        if (stage[4] != stage[0] && stage[0xc] >= 0) {
            stage[4] = stage[0];
            next = m_3d8[4] + 1;
            if (next > 0x12)
                next = 0x12;
            FUN_00418f70(next);
            m_3d8[8] = 0;
            m_3d8[7] = 0;
            m_3d8[6] = 0;
            m_3d8[5] = 0;
        }
        M_0040f3a0(-shift, y);
    }
}
