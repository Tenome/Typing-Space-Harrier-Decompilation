#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

int Game::M_00409270(int n)
{
    int found;
    int cur;
    int tries;
    int limit;

    found = 0;
    cur = m_880cc8[n];
    tries = 0;
    limit = DAT_008f069c[m_881cc8[n]];
    for (; tries < limit; tries++) {
        if (cur <= 0)
            break;
        M_0040af40(0, cur);
        if (m_87cbc8->m_0 != 0 && m_87cbc8->m_c == n) {
            m_8eccc8[found] = cur;
            found++;
            cur = m_87cbc8->m_24;
        }
    }
    m_8ecec8 = found;
    return found;
}
