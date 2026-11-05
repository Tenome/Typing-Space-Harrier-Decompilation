#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"

void Game::M_0040f8a0()
{
    switch (DAT_00a3d6f0) {
    default:
        if (0 < DAT_008f1004)
            DAT_008f1004--;
        if (0 < DAT_00a3d6f0 && --DAT_00a3d6f0 == 0) {
            switch (DAT_00a45e28) {
            case 0:
                M_00410580(0, 0);
                m_390 = 0;
                M_00409330(1, 5);
                M_00408650(0x19);
                DAT_008f1004 = 0;
                if (0 < m_3c0)
                    DAT_008f1004 = DAT_00a460bc * 2;
                DAT_00a45e28 = 2;
                M_0040fa70(DAT_00a460bc * 3);
                return;
            case 1:
                M_004010d0();
                m_a795c = 0xb;
                return;
            case 2:
                M_0040f840();
                return;
            }
        }
        break;
    case -1:
        M_0040fa00();
        M_0040fa70(0x50);
        M_0040fab0();
        M_0040fa80();
        M_00410580(2, 0);
        m_390 = 1;
        return;
    case -2:
        DAT_00a45e28 = 1;
        M_00410460(-1);
        M_0040fa70(DAT_00a460bc * 0x1e);
        M_00408660(0x4f, 0);
        M_00410580(0, 0);
        m_390 = 0;
        M_00409330(1, 7);
        break;
    }
}
