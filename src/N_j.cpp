#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

int Game::M_0040d2c0(int index, int bank, int n, int a4)
{
    int per;
    int start;
    int pick;
    int i;
    int id;
    char *reading;
    char *text;

    FUN_0040d130(bank);
    if (n != 0 && DAT_00a45e34->m_0c != 0) {
        per = DAT_00a45e34->m_0c / n;
        if (DAT_004f052c->m_b0 == DAT_00430658) {
            start = rand() % per * n;
            for (i = 0; i < n; i++)
                DAT_004f052c->m_b4[i] = i;
            DAT_004f052c->m_b0 = start;
        }
        pick = DAT_004f052c->m_b4[n - DAT_004f052c->m_08] + DAT_004f052c->m_b0;
        reading = DAT_00a45e34->m_10[pick];
        text = DAT_00a45e34->m_14[pick];
        id = M_0040dd60(index, reading, text, M_0040c970(text), a4);
        m_8a3cc8[index][m_8a2cc8[index]] = (short)id;
        m_8a2cc8[index]++;
        m_8c4->m_c = m_8a2cc8[index] - 1;
        m_8c4->m_10 = m_882cc8[index][m_8c4->m_c];
        m_8c4->m_64 = 0;
        return id;
    }
    return M_0040d160(index, bank, a4);
}






