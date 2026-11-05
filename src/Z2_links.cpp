#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

void Game::M_0040b140(int obj)
{
    int links[3];
    int y;
    int x;
    int whole;
    float scale;
    int count;
    int i;
    int link;
    int k;
    int *entry;

    if (obj >= 0 && obj < 0x4000) {
        M_0040af40(0, obj);
        if (m_87cbc8->m_0 > 0) {
            whole = 1;
            count = 0;
            scale = 1.0f;
            for (i = 0, entry = links; i < 3; i++) {
                if (i == 0)
                    link = m_87cbc8->m_28;
                else
                    link = m_87cbd0->m_18[4];
                if (link == 0)
                    break;
                M_0040af40(2, link);
                if (m_87cbd0->m_0 == 0) {
                    whole = 0;
                    break;
                }
                *entry = link;
                count++;
                entry++;
            }
            if (count != 0) {
                if (!whole) {
                    if (count > 0) {
                        entry = links;
                        for (i = count; i > 0; i--) {
                            M_0040b940(*entry);
                            entry++;
                        }
                    }
                    return;
                }
                if (count > 0) {
                    entry = links;
                    for (i = count; i > 0; i--) {
                        scale = M_0040b390(obj) * scale;
                        M_0040af40(2, *entry);
                        M_0040b800(m_87cbc8->m_30, m_87cbd0->m_2c[7]);
                        entry++;
                    }
                }
            }
            M_0040b850(m_87cbc8->m_44, m_87cbc8->m_58, scale);
            M_0040b850(m_87cbc8->m_34, m_87cbc8->m_4c, scale);
            M_0040b8b0(m_87cbc8->m_40, m_87cbc8->m_44, m_87cbc8->m_54, scale);
            M_0040b800(m_87cbc8->m_30, m_87cbc8->m_40);
            k = M_0040b330(obj);
            if (k == 0)
                k = 0x7c01 - (int)(M_0040b390(obj) * 3840.0f);
            M_0040b4d0(obj, 2, &x, &y);
            if (x == 0 || y == 0)
                k = 0x8002;
            M_0040b370(obj, k);
        }
    }
}
