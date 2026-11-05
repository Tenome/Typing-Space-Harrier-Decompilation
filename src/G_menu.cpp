#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_004100e0()
{
    char text[64];
    int i;

    M_00410260();
    switch (m_a7964) {
    case 2:
        if (m_a7968 > 0) {
            wsprintfA(text, "@bX %d POINT", 50000);
            M_0040bfa0(0x70, 0x89, text);
            if ((m_8a8 & 1) == 0) {
                M_004066d0(5000);
                m_a7968--;
                M_00408650(m_a797c + 0xe);
                m_a797c = (m_a797c == 0);
            }
        } else {
            m_a7968 = 0;
            m_a7964 = 0;
        }
        M_0040ff90(m_a7968);
        break;
    case 1:
        m_a7968 = DAT_0099d6bc->m_00[0xb];
        M_0040ff90(m_a7968);
        if (M_004084d0() != 0) {
            m_a7964 = 2;
            for (i = 0; i < 0x20; i++)
                M_00401630(i);
        }
        break;
    }
}

void Game::M_00410600()
{
    int result;
    int choice;
    int unused;
    int i;

    if (m_8ac < 0) {
        M_004061c0(2);
        M_00406210(0, 0, 0);
        M_00406240(0, 2);
        M_00406270(0, 1);
        M_004062e0(0x140, 0xe0, 0, 0x20);
        M_004061f0(7, 1);
        for (i = 0; *DAT_004bdcac[i] != '\0'; i++)
            M_004062b0(i, DAT_004bdcac[i]);
        M_0040e900(1);
    } else if (m_8ac == 0) {
        M_0040e900(0);
    }
    M_00406600();
    result = M_00406660();
    M_00406290(&unused, &choice);
    M_00410730();
    if (result != 0) {
        if (result < 0)
            choice = 1;
        switch (choice) {
        case 0:
            m_a795c = 2;
            break;
        case 1:
            m_a795c = 4;
            break;
        }
        M_004010d0();
    } else {
        m_8ac++;
        if (m_8ac >= 1000000)
            m_8ac = 1000000;
    }
}

void Game::M_00410800()
{
    int i;

    if (DAT_008f544c->m_00 != *m_3d8) {
        DAT_008f544c->m_00 = *m_3d8;
        strcpy(DAT_008f544c->m_10, DAT_004e28f8[DAT_008f544c->m_00]);
        strcpy(DAT_008f544c->m_50, DAT_008f544c->m_10);
        DAT_008f544c->m_0c = strlen(DAT_008f544c->m_50);
        DAT_008f544c->m_08 = 0;
        DAT_008f544c->m_04 = 0;
        DAT_008f544c->m_90 = 0x1c0;
        DAT_008f544c->m_94 = 0x1d0;
        for (i = 0; i < DAT_008f544c->m_0c; i++) {
            if (DAT_008f544c->m_50[i] == ' ')
                DAT_008f544c->m_50[i] = '@';
        }
    }
    if (DAT_008f544c->m_00 >= 0 && DAT_008f544c->m_10[0] != '\0') {
        M_00410900();
        M_00410b00();
    }
}
