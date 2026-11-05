#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "game.h"

int Game::M_004081b0(int scanOnly)
{
    int i;
    StageRow *row;
    int distance;
    int highest;
    int firstStage;
    int lastStage;

    highest = m_3d8[0xc];
    for (i = 0, row = DAT_004bddf8;; i++, row++) {
        if (row->m_0 > 0 && row->m_0 != m_3d8[0] + 1)
            continue;
        lastStage = row->m_1c;
        firstStage = row->m_10;
        if (firstStage < 0 && lastStage < 0) {
            if (scanOnly)
                m_3d8[0xc] = highest;
            return highest;
        }
        if (m_87fcc8[i] == 1) {
            M_0040af40(0, m_880cc8[i]);
            distance = m_87cbc8->m_0;
            if (distance < 0)
                distance = -distance;
            if (distance != m_881cc8[i])
                m_87fcc8[i] = 2;
        }
        if (firstStage <= m_3d8[0xc] && m_87fcc8[i] == 0) {
            if (row->m_34 > 0)
                M_00408660(row->m_34, row->m_38);
            if (firstStage < 0) {
                m_87fcc8[i] = 2;
            } else {
                if (firstStage > highest)
                    highest = m_3d8[0xc];
                if (scanOnly == 0) {
                    M_00408ad0(i, lastStage);
                    if (lastStage >= 0x114 && lastStage <= 0x122)
                        M_00410af0();
                }
            }
        }
    }
}

