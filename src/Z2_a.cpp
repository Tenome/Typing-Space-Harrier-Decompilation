#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

int Game::M_0040bc60()
{
    int tried;
    int limit;
    int cur;

    tried = 0;
    limit = 0x3fff - m_87cbc4->m_7d5214;
    cur = m_87cbc4->m_7d5224;
    if (limit <= 0)
        return 0;
    do {
        cur++;
        if (cur >= 0x4000)
            cur = m_87cbc4->m_7d5214;
        M_0040af40(3, cur);
        if (m_87cbd4->m_0 == 0) {
            if (cur != 0)
                m_87cbc4->m_7d5224 = cur;
            return cur;
        }
        tried++;
    } while (tried < limit);
    return 0;
}
