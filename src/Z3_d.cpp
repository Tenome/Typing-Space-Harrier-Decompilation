#include "game.h"

int Game::M_0040bbe0()
{
    int found = 0;
    int tried = 0;
    int cur;
    int limit;

    cur = m_87cbc4->m_7d5220;
    limit = 0x10000 - m_87cbc4->m_7d5210;
    if (limit <= 0)
        return found;
    do {
        cur++;
        if (cur >= 0x10000)
            cur = m_87cbc4->m_7d5210;
        M_0040aff0(5, cur);
        if (m_87cbec->m_0 == 0) {
            if (cur != 0)
                m_87cbc4->m_7d5220 = cur;
            return cur;
        }
        tried++;
    } while (tried < limit);
    return 0;
}
