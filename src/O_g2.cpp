#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

int Game::M_0040f720(int n)
{
    int best;
    int pass;
    int count;
    int rec;
    int run;
    int len;
    int sprite;
    int *entry;

    rec = DAT_004bde14[n * 0x16];
    best = 0;
    pass = 0;
    n = DAT_004efbd8[rec];
    count = DAT_008f069c[rec];
    if (count > 0) {
        for (; pass < count; pass++, n++) {
            if (n >= m_87cbc4->m_7d5214)
                break;
            M_0040af40(3, n);
            for (entry = DAT_004e27f8; entry < DAT_004e27f8 + 3; entry++) {
                run = 0;
                len = 0;
                sprite = m_87cbd4->m_2c[*entry];
                do {
                    if (sprite >= m_87cbc4->m_7d5210) {
                        run = 0;
                        break;
                    }
                    M_0040aff0(5, sprite);
                    run += m_87cbec->m_c;
                    if (m_87cbec->m_10 == 10000)
                        break;
                    if (m_87cbec->m_10 <= 0) {
                        run = 0;
                        break;
                    }
                    len++;
                    sprite++;
                } while (len < 0x400);
                if (best < run)
                    best = run;
            }
        }
        if (best != 0)
            return best;
    }
    return DAT_00a460bc << 10;
}
