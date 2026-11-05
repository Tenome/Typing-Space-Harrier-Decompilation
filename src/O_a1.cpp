#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

void Game::M_004066d0(int n)
{
    if (m_a795c == 4 && m_390 == 0 && DAT_00a45e28 == 0 && *m_3d8 + 1 <= 0x12 && m_87fcb4 == 0) {
        m_3a0 += n;
        if (m_3a0 < 0)
            m_3a0 = 0;
        if (m_3a0 >= 0x98967f)
            m_3a0 = 0x98967f;
        if (m_3a4 < m_3a0)
            m_3a4 = m_3a0;
        if (m_10[m_378] < m_3a0)
            m_10[m_378] = m_3a0;
        if (m_3a0 >= m_3a8 + m_3ac) {
            m_3a8 += m_3ac;
            m_3c0++;
            M_00408650(8);
        }
    }
}
