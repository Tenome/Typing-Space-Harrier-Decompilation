#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_00404040()
{
    int tick;
    int expired;

    DAT_00a460c8 = 0;
    expired = 0;
    tick = m_8ac;
    if (tick < 0)
        M_004014d0(DAT_009016fc);
    if (tick >= DAT_00a460bc * 30)
        expired = 1;
    if (M_00406660() != 0)
        expired = 1;
    if (expired) {
        DAT_009016fc++;
        if (DAT_009016fc >= 0x10)
            DAT_009016fc = 0;
        M_004014d0(DAT_00a23308);
        M_004010d0();
        M_00403730();
        m_a795c = 2;
        DAT_00a460c8 = 0;
    } else {
        m_8ac++;
        if (m_8ac >= 1000000)
            m_8ac = 1000000;
        M_00404110(0x14);
    }
}
