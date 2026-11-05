#include <stdlib.h>
#include <string.h>
#include "game.h"

int FUN_0040cf20()
{
    int none = DAT_00430658;
    int i;

    for (i = 0; i < 72; i++) {
        DAT_004f052c = &DAT_008f5450[i];
        if (DAT_004f052c->m_00 == 0)
            return i;
    }
    return none;
}

void Game::M_0040cf50(int index, int flag)
{
    int i;
    int hit;
    int id;

    if (m_8c0 == m_8b8)
        M_00401480(-1);
    if (flag == 0 && m_8c4->m_60 != 0) {
        m_8c4->m_50e8 = m_8c4->m_88;
        M_00408f60(index, 1);
        M_00408d00();
    }
    for (i = 0; i < 72; i++) {
        DAT_004f052c = &DAT_008f5450[i];
        if (DAT_004f052c->m_00 == 0 || index != DAT_004f052c->m_0c)
            continue;
        if (DAT_004f052c->m_24 == 0) {
            if (DAT_004f052c->m_14 != 0) {
                DAT_004f052c->m_04 = 1;
                if (flag != 0) {
                    DAT_004f052c->m_08++;
                    m_8a2cc8[index]--;
                }
            }
            if (DAT_004f052c->m_08 == 0) {
                DAT_004f052c->m_00 = 0;
                return;
            }
        } else {
            hit = M_0040d110(DAT_004f052c->m_14);
            if (hit != 0)
                M_00408f60(index, 1);
            if (flag != 0) {
                M_00408650(0x11);
                DAT_004f052c->m_08++;
                if (hit == 0)
                    m_8a2cc8[index]--;
                if (DAT_004f052c->m_14 == 0x38) {
                    m_87fcc4 = 0x14;
                    m_87fcc0--;
                }
            } else {
                M_00408650(0x10);
            }
            for (i = 0; i <= DAT_004f052c->m_28; i++) {
                id = DAT_004f052c->m_2c[i];
                M_00401440(id);
                if (m_8c8->m_0 != 0 && m_8c8->m_8 == index) {
                    M_00401630(id);
                    if (--m_3b8 < 0)
                        m_3b8 = 0;
                }
            }
            if (DAT_004f052c->m_08 == 0)
                DAT_004f052c->m_00 = 0;
            DAT_004f052c->m_04 = 1;
        }
        return;
    }
}

int Game::M_0040d110(int kind)
{
    int hit = 0;

    if (DAT_004f052c->m_14 >= 0x38 && DAT_004f052c->m_14 <= 0x3c)
        hit = 1;
    return hit;
}

void __stdcall FUN_0040d130(int bank)
{
    DAT_00a232a4 = bank;
    DAT_00a45e34 = &DAT_00901708[bank];
}
