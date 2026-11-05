#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"

int Game::M_0040d160(int index, int bank, int a3)
{
    int tries;
    int pick;
    int id;
    int base;
    int total;
    char *reading;
    char *text;
    int span;
    int weight;

    FUN_0040d130(bank);
    if (DAT_00a45e34->m_0c < 1)
        return DAT_00430658;
    for (tries = 0; tries < 10; tries++) {
        pick = rand() % DAT_00a45e34->m_0c;
        if (M_0040d850(DAT_00a45e34->m_14[pick], DAT_00a45e34->m_10[pick]) == 0)
            break;
    }
    m_87fcb8++;
    reading = DAT_00a45e34->m_10[pick];
    text = DAT_00a45e34->m_14[pick];
    weight = M_0040c970(text);
    id = M_0040dd60(index, reading, text, weight, a3);
    if (id >= 0) {
        base = DAT_004bde14[index * 0x16];
        total = DAT_004bde14[index * 0x16 + 3];
        if (total < 1)
            total = 1;
        m_8a3cc8[index][m_8a2cc8[index]] = (short)id;
        m_8a2cc8[index]++;
        span = DAT_008f069c[base];
        m_8c4->m_c = span - (m_8a2cc8[index] - 1) * span / total - 1;
        if (span <= m_8c4->m_c)
            m_8c4->m_c = span - 1;
        if (m_8c4->m_c < 0)
            m_8c4->m_c = 0;
        m_8c4->m_10 = m_882cc8[index][m_8c4->m_c];
        m_8c4->m_64 = 0;
    }
    return id;
}
