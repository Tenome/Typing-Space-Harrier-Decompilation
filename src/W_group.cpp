#include <stdlib.h>
#include "game.h"

int Game::M_0040d400(int index, int bank, int n, int flag, int last)
{
    int order[0x21];
    int count;
    int per;
    int pick;
    int base;
    int maxLen;
    int i;
    int k;
    int a;
    int b;
    int t;
    int v;
    int id;
    int listed;
    int len;
    int entry;
    char *word;
    char *reading;
    char *text;

    FUN_0040d130(bank);
    if (n < 1)
        return M_0040d160(index, bank, last);
    count = n + 1;
    per = DAT_00a45e34->m_0c / count;
    if (per == 0)
        return M_0040d160(index, bank, last);
    m_87fcb8++;
    if (n == 1 && flag != 0)
        pick = m_87fcc0;
    else
        pick = rand() % per;
    base = (n + 1) * pick;
    maxLen = 0;
    for (i = 1; i <= n; i++) {
        word = DAT_00a45e34->m_14[base + i];
        len = M_0040c970(word);
        if (maxLen < len)
            maxLen = len;
    }
    for (i = 0; i <= n; i++)
        order[i] = i;
    k = rand() % count + n;
    for (i = 0; i <= k; i++) {
        a = rand() % n + 1;
        b = rand() % n + 1;
        if (a != b) {
            t = order[a];
            order[a] = order[b];
            order[b] = t;
        }
    }
    for (i = 0; i <= n; i++) {
        v = order[i];
        entry = v + base;
        reading = DAT_00a45e34->m_10[entry];
        text = DAT_00a45e34->m_14[entry];
        id = M_0040dd60(index, reading, text, maxLen, last);
        M_00401400(id);
        listed = 0;
        if (flag != 0) {
            if (v > 0)
                listed = 1;
        } else if (v == 1)
            listed = 1;
        m_8c4->m_64 = 0;
        if (i > 0 && flag == 0 && listed == 1)
            m_8c4->m_64 = 1;
        m_8c4->m_50e0 = -1;
        if (v == 0)
            m_8c4->m_50e0 = 6;
        if (listed != 0) {
            m_8a3cc8[index][m_8a2cc8[index]] = (short)id;
            m_8a2cc8[index]++;
        }
        DAT_004f052c->m_2c[v] = id;
        m_8c4->m_6c = m_87fcb8;
        m_8c4->m_68 = 1;
        if (flag == 0 && listed == 0)
            m_8c4->m_68 = 0;
    }
    M_0040d980();
    for (i = 0; i <= n; i++)
        M_0040d990(DAT_004f052c->m_2c[order[i]]);
    M_0040da60();
    M_0040db00();
    for (i = 0; i <= n; i++) {
        v = order[i];
        id = DAT_004f052c->m_2c[v];
        M_00401400(id);
        m_8c4->m_4c = 0;
        if (v == 0) {
            m_8c4->m_44 = 8;
            m_8c4->m_48 = 0;
            m_8c4->m_58 = m_8c4->m_1c;
            m_8c4->m_50 = m_8c4->m_58;
            m_8c4->m_5c = m_8c4->m_20;
            m_8c4->m_54 = m_8c4->m_5c;
            m_8c4->m_54 -= 0x78;
            m_8c4->m_84 += DAT_00a460bc;
        } else {
            m_8c4->m_44 = 8;
            m_8c4->m_48 = DAT_00a460bc;
            m_8c4->m_58 = m_8c4->m_1c;
            m_8c4->m_50 = m_8c4->m_58;
            m_8c4->m_5c = m_8c4->m_20;
            m_8c4->m_54 = m_8c4->m_5c;
            if (rand() % 2)
                m_8c4->m_50 -= 0x280;
            else
                m_8c4->m_50 += 0x280;
        }
    }
    return id;
}
