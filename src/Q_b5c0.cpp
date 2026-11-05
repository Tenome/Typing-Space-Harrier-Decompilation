#include "game.h"

void Game::M_0040b5c0(int n)
{
    int i;
    int k;
    int result;
    int mode;
    int first;
    int second;
    int third;
    int head;

    M_0040af40(0, n);
    if (m_87cbc8->m_0 != 0) {
        for (i = 0, k = 0; i < 6; i++, k += 4) {
            if (m_87cbc8->m_4 & (1 << (k / 4 + 1)))
                continue;
            for (;;) {
                mode = DAT_004e261c[k];
                first = (&m_87cbc8->m_2c)[DAT_004e261c[k + 1]];
                second = (&m_87cbc8->m_2c)[DAT_004e261c[k + 2]];
                third = (&m_87cbc8->m_2c)[DAT_004e261c[k + 3]];
                result = M_0040ba50(mode, first, second, third);
                if (result == 10000)
                    goto failed;
                M_0040af40(0, n);
                if (result == 9999) {
                    if (m_87cbc8->m_0 > 0)
                        m_87cbc8->m_0 = -m_87cbc8->m_0;
                } else if (m_87cbc8->m_0 < 0) {
                    m_87cbc8->m_0 = -m_87cbc8->m_0;
                }
                if (result == third)
                    break;
                (&m_87cbc8->m_2c)[DAT_004e261c[k + 2]] = third;
                if (result != 0)
                    (&m_87cbc8->m_2c)[DAT_004e261c[k + 3]] = result;
                M_0040aff0(0, third);
                if (third == 0 || m_87cbd8->m_c != 0 || m_87cbd8->m_10 == 0)
                    break;
            }
        }
        goto finish;
failed:
        head = m_87cbc8->m_c;
        M_0040b940(n);
        M_004085f0(head);
finish:
        M_0040b7b0(m_87cbc8->m_38, m_87cbc8->m_50, -1);
        M_0040b7b0(m_87cbc8->m_30, m_87cbc8->m_48, -1);
        M_0040b7b0(m_87cbc8->m_34, m_87cbc8->m_4c, -1);
        M_0040b7b0(m_87cbc8->m_44, m_87cbc8->m_58, -1);
        if (m_87cbc8->m_18 != 0) {
            M_0040aff0(0, m_87cbc8->m_2c);
            if (m_87cbc8->m_18 >= m_87cbd8->m_c)
                m_87cbc8->m_18 = m_87cbd8->m_c - 1;
        }
    }
}
