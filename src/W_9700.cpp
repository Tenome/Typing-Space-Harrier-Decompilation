#include "game.h"

int Game::M_00409700(int a, int b, int flag)
{
    int buf[0x40];
    int count;
    int x;
    int y;
    int cx;
    int w;
    int h;
    int len;
    int tick;
    int delta;
    int step;
    int sx;
    int sw;
    int sh;
    int rows;
    int saved;
    int i;
    int j;
    int span;
    int t;
    int v;
    int *table;

    M_0040af40(0, a);
    M_0040af40(3, b);
    if (m_87cbc8->m_10 != 0)
        return 1;
    M_0040b4d0(a, 1, &x, &y);
    cx = x;
    len = M_00409300(a);
    M_0040b4d0(a, 2, &w, &h);
    sx = x - 320;
    w *= 2;
    h *= 2;
    if (len <= 0x146)
        len = 0x14a;
    tick = M_004096b0(len);
    if (tick < 0)
        return DAT_00430658;
    delta = 0x14 - tick;
    if (flag != 0) {
        count = delta * 2 + 4;
        if (count > 0x40)
            count = 0x40;
    } else
        count = 4;
    for (i = 0; i < count; i++) {
        buf[i] = M_0040bbe0();
        if (buf[i] == 0)
            return DAT_00430658;
    }
    M_0040af40(0, a);
    M_0040af40(3, b);
    if (flag == 0) {
        m_87cbc8->m_4 = 0x36;
        m_87cbc8->m_8 = 0x36;
        M_0040b0e0(buf[0], 1, x, y, 0, 1);
        M_0040b0e0(buf[1], 1, x, y, 1, 0);
        M_0040b0e0(buf[2], 1, w, h, 0, 1);
        M_0040b0e0(buf[3], 1, w, h, 1, 0);
        for (i = 0; i < 25; i++) {
            if (m_87cbd4->m_2c[i] > 0)
                (&m_87cbc8->m_2c)[i] = m_87cbd4->m_2c[i];
        }
        m_87cbc8->m_64 = buf[0];
        m_87cbc8->m_6c = buf[2];
        m_87cbc8->m_18 = 0;
    } else {
        span = len - 0x146;
        if (span <= 0) {
            len = 0x14a;
            span = 4;
        }
        sx = (sx << 10) / span;
        sw = (w << 10) / span;
        sh = (h << 10) / span;
        M_0040b0e0(buf[0], 1, x, y, 0, 1);
        rows = (len - y) / 32 + 1;
        M_0040b0e0(buf[1], 1, cx, len, rows, 1);
        step = delta * 4;
        saved = buf[2 + delta];
        M_0040b0e0(saved, 1, w, h, 0, 1);
        M_0040b0e0(buf[3 + delta], 1, w, h, rows, 1);
        if (delta > 0) {
            table = &DAT_004e246c[tick];
            for (j = 0; j < delta; j++, table++) {
                t = *table;
                v = t - 0x146;
                M_0040b0e0(buf[2 + j], 1, v * sx / 1024 + 0x140, t, 2, 1);
                M_0040b0e0(buf[4 + delta + j], 1, sw * v / 1024, sh * v / 1024, 2, 1);
            }
        }
        for (i = 0; i < delta + 2; i++) {
            for (j = 0; j < 2; j++) {
                v = buf[i + j * delta];
                M_0040aff0(5, v);
                if (i == delta + 1)
                    m_87cbec->m_10 = 0;
                else
                    m_87cbec->m_10 = buf[i + j * delta + 1] - v;
            }
        }
        for (i = 0; i < 25; i++) {
            if (m_87cbd4->m_2c[i] > 0) {
                (&m_87cbc8->m_2c)[i] = m_87cbd4->m_2c[i];
                m_87cbc8->m_18 = 0;
            }
        }
        m_87cbc8->m_64 = buf[0];
        m_87cbc8->m_6c = saved;
    }
    M_0040ba00(m_87cbc8->m_80);
    M_0040ba00(m_87cbc8->m_84);
    M_0040ba00(m_87cbc8->m_78);
    M_0040ba00(m_87cbc8->m_7c);
    M_0040ba00(m_87cbc8->m_60);
    M_0040ba00(m_87cbc8->m_68);
    M_0040ba00(m_87cbc8->m_70);
    m_87cbc8->m_10 = 1;
    for (i = 0; i < 0x40; i++) {
        if (i < count)
            (&m_87cbc8->m_0)[0x24 + i] = buf[i];
        else
            (&m_87cbc8->m_0)[0x24 + i] = 0;
    }
    m_a7978++;
    if (m_a7978 >= 3)
        m_a7978 = 0;
    M_00408650(m_a7978 + 0xb);
    return 0;
}
