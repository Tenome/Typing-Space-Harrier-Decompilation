#include <stdlib.h>
#include "game.h"

int Game::M_004093b0(int stage, int index, int base, int count)
{
    int failed;
    int got;
    int i;
    int j;
    int item;
    int serial;
    int randomized;
    int cap;
    int limit;
    int serials[128];
    int unit;
    int k;
    int n;
    int *key;

    randomized = 0;
    if (count == -1) {
        randomized = 1;
        count = DAT_004bddf8[stage].m_44;
        cap = DAT_004bddf8[stage].m_48;
        limit = DAT_008f069c[index];
        if (cap <= 0)
            cap = 0;
        if (cap > limit)
            cap = limit;
    }
    unit = DAT_004bddf8[stage].m_20;
    failed = DAT_00430658;
    if (count != 0) {
        if (count >= 0x80)
            count = 0x80;
        for (got = 0; got < count; got++) {
            item = M_0040bc60();
            if (item == 0)
                return 1;
            serials[got] = item;
        }
        for (i = 0; i < count; i++) {
            item = serials[i];
            serial = i + base;
            if (randomized) {
                if (cap > 0)
                    serial = cap - 1 + base;
                else if (limit > 0)
                    serial = rand() % limit + base;
                else
                    serial = base;
                if (serial < 0)
                    serial = 0;
            }
            M_0040a1d0(item, serial);
            M_0040af40(0, item);
            k = m_87cbc8->m_28;
            if (k > 0)
                m_87cbc8->m_28 = serials[k - base];
            k = m_87cbc8->m_24;
            if (k > 0)
                m_87cbc8->m_24 = serials[k - base];
            if (DAT_004e23c4[0] >= 0) {
                key = DAT_004e23c4;
                k = DAT_004e23c4[0];
                do {
                    item = M_0040bbe0();
                    if (item == 0)
                        return failed;
                    (&m_87cbc8->m_2c)[k] = item;
                    M_0040ba00(item);
                    key++;
                    k = *key;
                } while (k >= 0);
            }
        }
        for (j = 0; j < count; j++) {
            item = serials[j];
            m_882cc8[stage][j] = item;
            M_0040af40(0, item);
            m_87cbc8->m_0 = index;
            m_87cbc8->m_10 = 0;
            m_87cbc8->m_c = stage;
            m_87cbc8->m_14 = unit;
            for (n = 0; DAT_004e23c4[n] >= 0; n++) {
                M_0040aff0(0, (&m_87cbc8->m_2c)[DAT_004e23c4[n]]);
                m_87cbd8->m_0 = 1;
            }
            m_87cbc8->m_18 = 0;
            m_87cbc8->m_1c = 0;
            if (m_87cbc8->m_2c != 0) {
                M_0040aff0(0, m_87cbc8->m_2c);
                m_87cbc8->m_1c = m_87cbd8->m_10;
            }
        }
        return serials[0];
    }
    return failed;
}
