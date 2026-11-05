#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_0040a270()
{
    int placed;
    int obj;
    int k;
    int depth;
    int bucket;
    int a;
    int j;
    int n;
    int b;
    int c;
    int swap;

    placed = 0;
    obj = m_87cbc4->m_7d5214;
    for (k = 0; k < 0x81; k++)
        m_87cbc4->m_7b5004[k] = 0;
    m_87cbc4->m_7c520c = 0;
    for (; obj < 0x4000; obj++) {
        M_0040af40(0, obj);
        if (m_87cbc8->m_0 > 0) {
            depth = M_0040b350(obj);
            if (depth == 0)
                depth = M_0040b350(obj);
            if (depth != 0x8002) {
                bucket = depth >> 8;
                if (bucket >= 0x81)
                    bucket = 0x80;
                else if (bucket < 0)
                    bucket = 0;
                m_87cbc4->m_785004[placed] = depth;
                m_87cbc4->m_7a5004[placed] = obj;
                m_87cbc4->m_795004[placed] = bucket;
                m_87cbc4->m_7b5004[bucket]++;
                placed++;
            }
        }
    }
    m_87cbc4->m_785000 = placed;
    for (b = 0; b < 0x81; b++) {
        if (m_87cbc4->m_7b5004[b] != 0) {
            n = 0;
            for (j = 0; j < m_87cbc4->m_785000; j++) {
                if (b == m_87cbc4->m_795004[j]) {
                    m_87cbc4->m_7b520c[n] = j;
                    n++;
                }
            }
            if (n > 0) {
                for (a = 0; a < n; a++) {
                    for (c = a + 1; c < n; c++) {
                        if (m_87cbc4->m_785004[m_87cbc4->m_7b520c[a]] > m_87cbc4->m_785004[m_87cbc4->m_7b520c[c]]) {
                            swap = m_87cbc4->m_7b520c[a];
                            m_87cbc4->m_7b520c[a] = m_87cbc4->m_7b520c[c];
                            m_87cbc4->m_7b520c[c] = swap;
                        }
                    }
                }
                for (k = 0; k < n; k++) {
                    m_87cbc4->m_7c5210[m_87cbc4->m_7c520c] = m_87cbc4->m_7a5004[m_87cbc4->m_7b520c[k]];
                    m_87cbc4->m_7c520c++;
                }
            }
        }
    }
}
