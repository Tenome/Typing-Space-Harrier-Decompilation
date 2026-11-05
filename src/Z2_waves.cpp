#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_00407310(int group)
{
    int order[4];
    int i;
    int a;
    int b;
    int swap;
    int row;
    int mode;
    int kind;
    int k;
    int count;
    int value;
    int roll;
    int *waves = (int *)DAT_004e22e0;
    int base;

    for (i = 0; i < 4; i++)
        order[i] = i;
    for (i = 0x28; i != 0; i--) {
        a = rand() % 4;
        b = rand() % 4;
        swap = order[a];
        order[a] = order[b];
        order[b] = swap;
    }
    switch (group) {
    case 1:
        row = 0;
        break;
    case 2:
        row = 1;
        break;
    case 3:
        row = 2;
        break;
    case 4:
        row = 3;
        break;
    default:
        return;
    }
    base = row * 3;
    roll = rand() % 100;
    count = waves[base];
    value = waves[base + 2];
    mode = (roll < waves[base + 1]) ? 2 : 0;
    for (i = 0; i < count; i++) {
        kind = DAT_004e21a0[row * 16 + rand() % 16];
        M_00406c90(order[i]);
        DAT_00a232a0->m_0 = 1;
        DAT_00a232a0->m_4 = kind;
        DAT_00a232a0->m_58 = 0;
        DAT_00a232a0->m_4c = 0;
        DAT_00a232a0->m_54 = 0;
        DAT_00a232a0->m_5c = value;
        switch (mode) {
        case 2:
            DAT_00a232a0->m_50 = 1;
            for (k = 0; DAT_004e22d8[k] >= 0; k++) {
                DAT_00a232a0->m_8[k] = DAT_004e22d8[k];
                DAT_00a232a0->m_48 = k + 1;
            }
            break;
        case 1:
            DAT_00a232a0->m_50 = 2;
            for (k = 0; DAT_004e22a4[k] >= 0; k++) {
                DAT_00a232a0->m_8[k] = DAT_004e22a4[k];
                DAT_00a232a0->m_48 = k + 1;
            }
            break;
        case 0:
            if (kind == 1) {
                DAT_00a232a0->m_50 = 4;
                DAT_00a232a0->m_8[0] = 1;
                DAT_00a232a0->m_8[1] = 2;
                DAT_00a232a0->m_48 = 2;
            } else {
                DAT_00a232a0->m_50 = 4;
                DAT_00a232a0->m_8[0] = kind;
                DAT_00a232a0->m_8[1] = kind;
                DAT_00a232a0->m_48 = 2;
            }
            break;
        }
    }
}



