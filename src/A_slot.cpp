#include <string.h>
#include "game.h"

void Game::M_00401ab0()
{
    int single;
    int id;
    int mark;
    int state;
    int x, y, w, h;
    int fx, ignored;

    if (m_8c4->m_0 == 0)
        return;
    single = 0;
    if (m_8b4 == 1)
        single = 1;
    if (!M_00401da0(m_8c4->m_8) || !single)
        return;
    state = 0;
    if (m_8c4->m_70 != 0)
        return;
    id = m_8c4->m_10;
    mark = m_882cc8[m_8c4->m_8][m_8c4->m_c];
    M_0040af40(3, id);
    if (m_87cbd4->m_0 == 0 || id != mark || m_8c4->m_8 != m_87cbd4->m_c) {
        m_882cc8[m_8c4->m_8][m_8c4->m_c] = DAT_00430658;
        return;
    }
    M_0040b4d0(id, 0xe, &fx, &ignored);
    M_00401c90(id, &x, &y, &w, &h);
    if (fx == POS_NONE)
        state = 1;
    if (w == 0 || h == 0)
        state = 2;
    m_8c4->m_1c = x - m_8c4->m_24 / 2;
    m_8c4->m_20 = y + h;
    if (m_8c4->m_20 < 0x18)
        m_8c4->m_20 = 0x18;
    if (m_8c4->m_20 + m_8c4->m_28 >= 0x1c8)
        m_8c4->m_20 = 0x1c8 - m_8c4->m_28;
    if (state != 0) {
        m_8c4->m_1c = -0x280;
        m_8c4->m_20 = -0x1e0;
    }
    m_8c4->m_74++;
    if (m_8c4->m_74 >= m_8c4->m_78) {
        m_8c4->m_70 = 1;
        m_8c4->m_78 = 0;
        m_8c4->m_74 = 0;
    }
}

void Game::M_00401c90(int id, int *x, int *y, int *w, int *h)
{
    M_0040af40(3, id);
    if (m_87cbd4->m_0 <= 0) {
        *y = POS_NONE;
        *x = POS_NONE;
        *h = 0;
        *w = 0;
        return;
    }
    M_0040b4d0(id, 1, x, y);
    M_0040b4d0(id, 2, w, h);
    if (*x == POS_NONE || *y == POS_NONE) {
        *y = POS_NONE;
        *x = POS_NONE;
        *h = 0;
        *w = 0;
    } else if (id != m_87cbc4->m_7d5228) {
        M_0040a520(x, y, m_87cbd4->m_14);
    }
}

void __stdcall FUN_00401d70(int n, int *a, int *b)
{
    *a = DAT_004bddf8[n].m_50;
    *b = DAT_004bddf8[n].m_54;
}

void Game::M_004020a0()
{
    if (m_a795c != 8) {
        switch (M_004020e0()) {
        case 1:
            break;
        default:
            M_00402310();
            M_00402930();
            M_00402960();
            M_00402200();
            break;
        }
    }
}

int Game::M_004020e0()
{
    int phase;
    int t;
    Slot *s;

    if (m_a795c == 8)
        return 1;
    phase = M_004021d0();
    switch (phase) {
    case 0:
        break;
    case 1:
        m_8c4->m_4c++;
        return 1;
    default:
        s = m_8c4;
        if (s->m_4c < s->m_48 + s->m_44 && s->m_44 != 0) {
            t = s->m_4c - s->m_48;
            s->m_1c = ((s->m_58 - s->m_50) * t) / s->m_44 + s->m_50;
            s = m_8c4;
            s->m_20 = ((s->m_5c - s->m_54) * t) / s->m_44 + s->m_54;
            m_8c4->m_4c++;
            return phase;
        }
        s->m_1c = s->m_58;
        m_8c4->m_20 = m_8c4->m_5c;
        m_8c4->m_48 = 0;
        m_8c4->m_44 = 0;
        m_8c4->m_4c = 0;
        break;
    }
    return 0;
}

int Game::M_004021d0()
{
    Slot *s = m_8c4;
    int total = s->m_44 + s->m_48;

    if (total == 0)
        return 0;
    if (s->m_4c > total)
        return 0;
    return (s->m_4c >= s->m_48) + 1;
}

void Game::M_004022e0(int a, int b, int c, int d, int e)
{
    M_0040ec90(a, b, c, d, e);
}

void Game::M_00401dd0(int n)
{
    int outcome;
    int r;
    int k;
    Slot *s;

    M_00401400(n);
    M_00401ab0();
    switch (m_8c4->m_0) {
    case 1:
        m_8c4->m_0 = 3;
        break;
    case 3:
        break;
    case 4:
        if (n == m_8b8)
            M_00401480(-1);
        s = m_8c4;
        outcome = 0;
        if (s->m_68 == 0) {
            if (s->m_14 != 0) {
                M_004068e0(DAT_00430658);
                M_00401480(-1);
                M_0040f860();
            }
            outcome = 2;
        } else if (s->m_80 >= s->m_84) {
            outcome = 1;
        }
        M_004076a0(outcome > 0);
        M_00407740();
        if (outcome == 0) {
            s = m_8c4;
            M_004066d0((((s->m_84 - s->m_80) * 1000) / (s->m_84 + 1) + (int)strlen(s->m_1494) * 500) * m_3b0 / m_3b4);
        }
        M_0040cf50(m_8c4->m_8, outcome);
        if (m_8c4->m_0 == 0)
            return;
        m_3b8--;
        if (m_3b8 < 0)
            m_3b8 = 0;
        M_00401630(n);
        return;
    default:
        return;
    }
    if (n == m_8b8) {
        r = M_00402be0();
        if (r > 0) {
            M_00408650(m_a7970 + 1);
            m_a7970++;
            if (m_a7970 >= 3)
                m_a7970 = 0;
            M_0040a100();
        } else if (r < 0) {
            M_00408650(4);
            m_a7974++;
            if (m_a7974 >= 3)
                m_a7974 = 0;
        }
    }
    if (M_004021d0() == 0)
        m_8c4->m_80++;
    if (m_8c4->m_80 >= m_8c4->m_84) {
        if (n == m_8b8)
            M_00401480(-1);
        m_8c4->m_80 = m_8c4->m_84;
        if (m_8c4->m_14 != 0) {
            M_004068e0(DAT_00430658);
            M_0040f860();
        }
        m_8c4->m_50ec = 0;
        m_8c4->m_0 = m_8c4->m_0 + 1;
    }
    M_00401400(n);
    M_004020a0();
    if (m_8c4->m_50ec != 0) {
        M_00401480(-1);
        k = DAT_004bddf8[m_8c4->m_8].m_24;
        if (k >= 0x17 && k <= 0x28)
            M_004066d0(5000);
        if (k >= 0x2a && k <= 0x43) {
            M_00410bd0(k + 0x17);
            M_004066d0(10000);
        }
        m_8c4->m_0 = m_8c4->m_0 + 1;
    }
}
