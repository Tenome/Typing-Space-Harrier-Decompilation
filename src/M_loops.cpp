#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_00403650()
{
    int i = 0;
    char *p = DAT_00a461d0;

    while (1) {
        if (*p == 0)
            break;
        *p = p[1];
        i++;
        p++;
        if (i >= 0x1f)
            break;
    }
}

int Game::M_00408130()
{
    int i = 0;
    int distance;
    int stageNo;
    int active;

    while (1) {
        stageNo = DAT_004bddf8[i].m_0;
        active = DAT_004bddf8[i].m_4;
        if (DAT_004bddf8[i].m_10 < 0 && DAT_004bddf8[i].m_1c < 0)
            break;
        if ((stageNo <= 0 || stageNo == m_3d8[0] + 1) && active == 1 && m_87fcc8[i] == 1) {
            M_0040af40(0, m_880cc8[i]);
            distance = m_87cbc8->m_0;
            if (distance < 0)
                distance = -distance;
            if (distance == m_881cc8[i])
                return 1;
        }
        i++;
    }
    return 0;
}

void Game::M_0040c650()
{
    int i = 0;

    while (1) {
        DAT_00a45e34 = &DAT_00901708[i];
        memset(&DAT_00901708[i], 0, sizeof(BankRec));
        i++;
        if (i >= 0x50)
            break;
    }
    M_0040c760();
    M_0040c6a0();
}

void Game::M_0040c6a0()
{
    int i = 0;

    while (1) {
        DAT_004f052c = &DAT_008f5450[i];
        memset(&DAT_008f5450[i], 0, sizeof(GroupRec));
        i++;
        if (i >= 72)
            break;
    }
    m_87fcb8 = 0;
    m_87fcc0 = 0;
    m_87fcc4 = 0;
}

void Game::M_0040e2a0(char *str)
{
    char *p = str;
    char c;

    while (*p != 0) {
        c = *p;
        if (c >= 'a' && c <= 'z')
            *p = c - 0x20;
        p++;
    }
}
