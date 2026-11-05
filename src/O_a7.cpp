#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_00403940(int mode)
{
    int count, next;
    char **names;
    int i;
    int result;
    int held;
    int cursor, state;
    int stage;

    if (m_8ac < 0) {
        m_3cc = 0;
        if (*DAT_004bdcdc[0] != 0) {
            next = 1;
            names = DAT_004bdcdc;
            do {
                count = next;
                names++;
                next++;
            } while (**names != 0);
        } else {
            count = mode;
        }
        M_004061c0(2);
        M_00406210(0, 0, 0);
        M_00406240(count, 2);
        M_00406270(0, 0);
        M_004062e0(0x140, 0xd8, 0x80, 0x20);
        M_004061f0(7, 1);
        for (i = 0; *DAT_004bdcc8[i] != 0; i++)
            M_004062b0(i, DAT_004bdcc8[i]);
        M_0040e900(1);
    } else if (m_8ac == 0) {
        M_0040e900(0);
    }
    M_00406290(&cursor, &state);
    held = state;
    if (state == 0)
        cursor = m_3cc;
    else
        cursor = 0;
    M_00406270(cursor, state);
    M_00406600();
    result = M_00406660();
    M_00406290(&cursor, &state);
    if (state == 0 && held == 0)
        m_3cc = cursor;
    M_00403b80(mode);
    if (result == 0) {
        m_8ac++;
        if (m_8ac >= 1000000)
            m_8ac = 1000000;
        return;
    }
    if (result < 0)
        state = 1;
    switch (state) {
    case 0:
        m_a795c = 4;
        DAT_00a23308 = DAT_004bdd28[m_3cc];
        m_3c4 = m_380;
        M_00410460(m_378);
        M_004014d0(DAT_00a23308);
        M_004010d0();
        M_00403730();
        if (mode == 1) {
            stage = M_00403890(DAT_00a23308);
            M_004038f0(DAT_00a23308, stage);
            m_3d8[12] = stage;
            m_3d0 = 1;
        }
        break;
    case 1:
        m_a795c = 2;
        M_004010d0();
        return;
    }
    M_004010d0();
}
