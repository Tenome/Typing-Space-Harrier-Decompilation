#include <stdlib.h>
#include "game.h"

void Game::M_00407040(int cmd)
{
    int n;

    M_00408650(0x20);
    n = 0;
    switch (cmd) {
    case 1:
        n = 0x21;
        break;
    default:
        return;
    case 3:
        m_3c0++;
        n = 8;
        break;
    case 4:
        if (m_3c0 > 1) {
            m_3c0--;
            n = 8;
        }
        break;
    case 5:
        DAT_008f1000->m_180 = DAT_00a460bc * 0x14;
        n = 8;
        break;
    case 6:
        if (m_378 + 1 < DAT_004bd608) {
            M_00410460(m_378 + 1);
            n = 8;
        }
        break;
    case 7:
        if (m_378 - 1 >= 0) {
            M_00410460(m_378 - 1);
            n = 8;
        }
        break;
    case 8: case 9: case 10: case 11: case 12:
        M_004066d0(DAT_004e2110[cmd]);
        n = 0x20;
        break;
    case 13: case 14: case 15: case 16: case 17:
        M_004066d0(-DAT_004e20fc[cmd]);
        n = 0x29;
        break;
    }
    M_00408650(n);
}

void Game::M_00408650(int n)
{
    M_00408660(n, 0);
}

void Game::M_00407230()
{
    int chosen;
    int i;

    if (m_a795c != 3) {
        chosen = DAT_00430658;
        for (i = 0; i < 4; i++) {
            M_00406c90(i);
            if (DAT_00a232a0->m_0 == 1 && DAT_00a46264[i] != 0) {
                chosen = i;
                break;
            }
        }
        if (chosen != DAT_00430658) {
            for (i = 0; i < 4; i++) {
                if (i == chosen) {
                    M_00406c90(i);
                    DAT_00a232a0->m_0 = 2;
                    DAT_00a232a0->m_8[0] = DAT_00a232a0->m_4;
                    DAT_00a232a0->m_48 = 1;
                    DAT_00a232a0->m_4c = 0;
                    DAT_00a232a0->m_50 = 1;
                    DAT_00a232a0->m_58 = 0;
                    DAT_00a232a0->m_5c = 0x5a;
                    M_00407040(DAT_00a232a0->m_4);
                } else {
                    M_00406cb0(i);
                }
            }
        }
    }
}
