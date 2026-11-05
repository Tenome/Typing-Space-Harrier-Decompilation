#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "game.h"

void Game::M_004042a0()
{    int limit;
    int tick;
    int expired = 0;

    DAT_00a460c8 = 0;
    M_0040a640();
    M_0040bea0(0xc0, 0xb0, m_87cbfc[0]);
    if (m_39c != 0) {
        if (m_87fcc4 == 0) {
            M_0040c250(0x170, "@aYOU GOT THE");
            M_0040c250(0x1a0, "@aTYPING MASTER POWER");
        } else {
            M_0040c250(400, "@aTHANKS FOR PLAYING");
        }
        limit = DAT_00a460bc * 30;
    } else {
        limit = DAT_00a460bc * 5;
    }
    tick = m_8ac;
    if (tick >= limit)
        expired = 1;
    if ((M_00406660() != 0 && (tick >= DAT_00a460bc * 3 || m_39c != 0)) || expired) {
        M_004010d0();
        M_00403730();
        if (m_39c != 0)
            m_a795c = 0xb;
        else
            m_a795c = 2;
        m_39c = 0;
        DAT_00a460c8 = 0;
    } else {
        m_8ac++;
        if (m_8ac >= 1000000)
            m_8ac = 1000000;
    }
}
