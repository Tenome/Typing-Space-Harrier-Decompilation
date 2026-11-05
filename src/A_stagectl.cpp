#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "game.h"

void Game::M_00403cf0()
{
    int tick = m_8ac;

    if (tick < 0) {
        M_00410460(-1);
        DAT_00a460c8 = 0;
        m_87fcc4 = 0;
        M_004013b0();
        M_00403730();
    }
    M_00403d80(tick);
    m_8ac++;
    if (m_8ac >= 1000000)
        m_8ac = 1000000;
    if (M_00406660() != 0 || tick >= DAT_00a460bc * 30) {
        M_004010d0();
        m_a795c = 2;
    }
}

void Game::M_00404000()
{
    int *stage = m_3d8;

    if (stage[0] + 1 == 0x12 && stage[0xcf + stage[0] * 2] - stage[0xd0 + stage[0] * 2] == 1) {
        m_a795c = 0xb;
        M_004010d0();
        return;
    }
    M_00410600();
}

void Game::M_00404110(int chance)
{
    char text[0x1400];
    char first;
    int i;

    if (chance <= 0) {
        DAT_00a4603c = 0;
        return;
    }
    DAT_00a460c8 = -1;
    memset(DAT_00a46264, 0, 0x20);
    M_004071c0();
    if (m_8b8 == -1) {
        if (m_3b8 == 0) {
            DAT_00a4603c = 0;
            return;
        }
        m_950 = 0;
        for (i = 0; i < 0x20; i++) {
            M_00401440(i);
            if (m_8c8->m_0 != 0 && m_8c8->m_60 == 0) {
                m_954[m_950] = i;
                m_950++;
            }
        }
        if (m_950 == 0) {
            DAT_00a4603c = 0;
            return;
        }
        M_00401480(m_954[rand() % m_950]);
    }
    text[0] = 0;
    if (rand() % 100 <= chance) {
        M_00402a80(text);
    } else {
        DAT_00a4603c++;
        if (DAT_00a4603c * chance >= 0xfa) {
            M_00402a80(text);
            DAT_00a4603c = 0;
        }
    }
    first = text[0];
    DAT_00a460c4 = 0;
    DAT_00a461d0[1] = 0;
    DAT_00a461d0[0] = 0;
    if (first > ' ') {
        DAT_00a460c8 = first;
        DAT_00a461d0[0] = first;
        DAT_00a460c4 = 1;
    }
    if (DAT_00a460c8 <= 0)
        DAT_00a460c8 = -1;
}

void Game::M_00403b80(int mode)
{
    char line[0x100];
    int ignored, state;
    int down;
    int x, y, len;
    char *name;

    if (mode >= 0 && mode <= 1) {
        M_00406290(&ignored, &state);
        M_00402730(0xa0, 0x48, 0x140, 0x30, 1, 1);
        M_00402730(0xa0, 0xb8, 0x140, 0x70, 2, 1);
        M_0040bfa0(0xe0, 0x58, DAT_004bdcc0[mode]);
        M_00406490();
        down = 0;
        if (state == 0)
            down = 1;
        name = DAT_004bdcdc[m_3cc];
        x = DAT_00951fcc->m_2c + DAT_00951fcc->m_24;
        y = DAT_00951fcc->m_28;
        if (down != 0) {
            len = strlen(name);
            x += len * -0x20;
            y -= 8;
        } else {
            len = strlen(name);
            x += (-4 - len) * 0x10;
        }
        strcpy(line, DAT_004bd5c4[down]);
        strcat(line, name);
        M_0040bfa0(x, y, line);
    }
}
