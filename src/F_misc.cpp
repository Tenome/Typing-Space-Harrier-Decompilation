#include <string.h>
#include "game.h"

int Game::M_00408ad0(int stage, int index)
{
    int level;

    level = M_00409330(stage, index);
    m_880cc8[stage] = level;
    m_881cc8[stage] = index;
    m_87fcc8[stage] = 1;
    if (level < 0) {
        m_87fcc8[stage] = 2;
        return DAT_00430658;
    }
    M_00408b70(stage);
    if (DAT_004bddf8[stage].m_28 > 0) {
        M_0040cdd0(stage, DAT_004bddf8[stage].m_2c, DAT_004bddf8[stage].m_24, DAT_004bddf8[stage].m_28,
                   DAT_004bddf8[stage].m_14, DAT_004bddf8[stage].m_18);
        m_8e3cc8[stage] = 0;
    }
    return 0;
}

void Game::M_00408b70(int stage)
{
    int first;
    int second;
    int chain;
    int mask;
    int link;
    int guard;
    int word;
    int head;

    head = m_880cc8[stage];
    if (head != 0) {
        M_0040af40(0, head);
        first = 0;
        word = 0;
        if (m_87cbc8->m_2c > 0) {
            M_0040aff0(0, m_87cbc8->m_2c);
            word = m_87cbd8->m_10;
            first = m_87cbd8->m_c;
        }
        chain = 0;
        mask = DAT_008f069c[m_881cc8[stage]];
        link = m_880cc8[stage];
        for (guard = 0; guard < 0x40; guard++) {
            if (link <= 0 || link >= 0x4000)
                break;
            M_0040af40(0, link);
            if (m_87cbc8->m_0 == 0)
                break;
            second = m_87cbc8->m_24;
            chain++;
            if (link == second)
                break;
            link = second;
        }
        m_8e4cc8[stage] = first;
        m_8e5cc8[stage] = word;
        m_8e6cc8[stage] = chain;
        m_8e7cc8[stage] = mask;
        if (first > 0) {
            m_8eacc8[stage] = first;
            m_8e9cc8[stage] = first;
            m_8ebcc8[stage] = 1;
            return;
        }
        m_8eacc8[stage] = mask;
        m_8e9cc8[stage] = mask;
        m_8ebcc8[stage] = 2;
    }
}

void Game::M_00408f60(int stage, int percent)
{
    int kind;
    int count;
    int i;
    int live;
    int kept;
    short entry;
    int value;
    int needed;
    int ticks;

    kind = DAT_004bddf8[stage].m_2c;
    if (kind >= 0x38 && kind <= 0x3c) {
        count = m_8a2cc8[stage];
        needed = kind - 0x37;
        ticks = m_8e3cc8[stage];
        if (percent != 0 || (ticks != 0 && ticks % 100 == 0)) {
            live = 0;
            for (i = 0; i < count; i++) {
                if (percent < 100 || m_8c3cc8[stage][i] == 0)
                    live++;
            }
            if (percent != 0 || live != needed) {
                kept = 0;
                for (i = 0; i < count; i++) {
                    entry = m_8c3cc8[stage][i];
                    value = entry;
                    if (value <= 0 || (value < 100 && percent != 0)) {
                        m_8a2cc8[stage]--;
                    } else {
                        m_8c3cc8[stage][kept] = entry;
                        m_8a3cc8[stage][kept] = m_8a3cc8[stage][i];
                        kept++;
                    }
                }
                for (i = kept; i < count; i++) {
                    m_8a3cc8[stage][i] = DAT_00430658;
                    m_8c3cc8[stage][i] = 0;
                }
                M_004090a0(stage);
            }
        }
    }
}

void Game::M_00409190(int stage, int skip)
{
    int kind;
    int count;
    int i;
    int flag;
    int word;

    kind = DAT_004bddf8[stage].m_30;
    M_00409270(stage);
    count = m_8ecec8 - skip;
    for (i = 0; i < count; i++) {
        word = m_8eccc8[m_8ecec8 - i - 1];
        switch (kind) {
        case 1:
        case 3:
            flag = 0;
            break;
        case 2:
            flag = 1;
            break;
        default:
            continue;
        }
        M_00409d50(word, DAT_004efc48, flag, i * 3);
    }
}

int Game::M_00409330(int stage, int index)
{
    int result;
    int kind;

    if (index < 0 || index >= 0x255)
        return DAT_00430658;
    if (DAT_004bddf8[stage].m_3c != 0)
        result = M_004093b0(stage, index, DAT_004efbd8[index], -1);
    else
        result = M_004093b0(stage, index, DAT_004efbd8[index], DAT_008f069c[index]);
    kind = DAT_004bddf8[stage].m_1c;
    if ((kind > 4 && kind < 0x1b) || kind == 0x18e)
        m_87cbc4->m_7d5228 = result;
    return result;
}

void Game::M_0040a7b0()
{
    int i;
    int id;
    int x;
    int y;
    int x2;
    int y2;
    int top;
    float scale;

    DAT_00a45e38 = 1;
    for (i = 0; i < m_87cbc4->m_7c520c; i++) {
        id = m_87cbc4->m_7c5210[i];
        if (M_0040b430(id, 0, 1) == 0) {
            M_0040b4d0(id, 1, &x, &top);
            M_0040b4d0(id, 2, &x2, &y2);
            y = M_00409300(id);
            scale = M_0040b390(id);
            if (id != m_87cbc4->m_7d5228) {
                M_0040a520(&x, &y, 0);
            } else {
                m_a7948 = x;
                m_a794c = y;
            }
            M_0040bf30(x, y, DAT_0099d770, scale);
        }
    }
    DAT_00a45e38 = 0;
}

float Game::M_0040b390(int n)
{
    int denominator;
    int numerator;

    denominator = M_0040b3d0(n);
    numerator = M_0040b3f0(n);
    if (denominator == 0)
        return 1.0f;
    return (float)numerator / denominator;
}

void Game::M_0040b850(int a, int b, float scale)
{
    M_0040aff0(5, b);
    M_0040aff0(4, a);
    m_87cbdc->m_4 = (int)(m_87cbec->m_4 * scale);
    m_87cbdc->m_8 = (int)(m_87cbec->m_8 * scale);
}

void Game::M_0040b8b0(int a, int b, int c, float scale)
{
    int angle;

    M_0040aff0(4, a);
    M_0040aff0(5, b);
    M_0040aff0(1, c);
    angle = m_87cbe0->m_4 % 360;
    if (angle < 0)
        angle += 360;
    m_87cbdc->m_4 = (DAT_0042ca9c[angle * 2] * m_87cbec->m_4) >> 16;
    m_87cbdc->m_8 = -(DAT_0042d5dc[angle * 2] * m_87cbec->m_8) >> 16;
}
