#include <string.h>
#include "game.h"

void Game::M_00403690()
{
    m_87cbc4 = &m_a7984;
    memset(&m_a7984, 0, sizeof m_a7984.m_0);
    memset(m_87cbc4->m_640000, 0, sizeof m_87cbc4->m_640000);
    memset(m_87cbc4->m_780000, 0, sizeof m_87cbc4->m_780000);
    m_87cbc4->m_7d5224 = 1;
    m_87cbc4->m_7d5220 = 1;
    m_87cbc4->m_7d5214 = 1;
    m_87cbc4->m_7d5210 = 1;
    m_87cbc4->m_7d5218 = 0;
    m_87cbc4->m_7d521c = 0;
    m_87cbc4->m_7d5228 = -1;
}

void Game::M_00403730()
{
    m_87cbc4->m_7d5224 = m_87cbc4->m_7d5214;
    m_87cbc4->m_7d5220 = m_87cbc4->m_7d5210;
    M_0040af40(0, m_87cbc4->m_7d5214);
    M_0040aff0(0, m_87cbc4->m_7d5210);
    memset(m_87cbc8, 0, (0x4000 - m_87cbc4->m_7d5214) * 0x190);
    memset(m_87cbd8, 0, (0x10000 - m_87cbc4->m_7d5210) * 0x14);
}

void Game::M_004037f0()
{
    memset(DAT_004efbd8, 0, 0x255 * sizeof(int));
    memset(DAT_008f069c, 0, 0x255 * sizeof(int));
    DAT_004efbd4 = 0;
    DAT_00a3d704 = 1;
    DAT_0099cfd0 = 1;
    M_004078f0();
    DAT_0099d770 = M_0040bf80(0xd, 0);
    m_a7950 = 0x146;
}

void Game::M_00403850()
{
    M_00404040();
    if (m_a795c == 3)
        M_00407850();
}

void Game::M_00403870()
{
    M_00403940(0);
}

void Game::M_00403880()
{
    M_00403940(1);
}

int Game::M_00403890(int n)
{
    int best = DAT_00430658;
    StageRow *r;

    if (*DAT_0043065c[n] != '*') {
        for (r = DAT_004bddf8; ; r++) {
            int t = r->m_10;
            int kind = r->m_0;
            int live = r->m_4;
            if (t < 0 && r->m_1c < 0)
                break;
            if (n + 1 == kind && live != 0 && (best == DAT_00430658 || best > t))
                best = t;
        }
        if (best >= 0)
            return best;
    }
    return 0;
}

void Game::M_004038f0(int a, int b)
{
    StageRow *r;
    int i = 0;

    for (r = DAT_004bddf8; ; i++, r++) {
        int kind = r->m_0;
        if (r->m_10 < 0 && r->m_1c < 0)
            break;
        if (kind != 0 && kind <= a + 1 && (a + 1 != kind || b > r->m_10))
            m_87fcc8[i] = 2;
    }
}
