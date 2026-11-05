#include <string.h>
#include "game.h"

void Game::M_00407590(int count)
{
    int top;
    int i;
    int idx;

    top = DAT_009a5e98.m_0 - 1;
    for (i = 0; i < count; i++) {
        if (top < 0)
            return;
        idx = M_004068a0(top + 1);
        if (idx != DAT_00430658) {
            DAT_00a3d6ec = &DAT_009a629c[idx];
            DAT_00a3d6ec->m_0 = 1;
            DAT_00a3d6ec->m_8 = 0;
            DAT_00a3d6ec->m_14 = DAT_00a3d6ec->m_c;
            DAT_00a3d6ec->m_1c = DAT_00a3d6ec->m_14;
            DAT_00a3d6ec->m_18 = DAT_00a3d6ec->m_10;
            DAT_00a3d6ec->m_20 = DAT_00a3d6ec->m_18;
            DAT_00a3d6ec->m_20 = -0x20;
            DAT_00a3d6ec->m_30 = 0;
            DAT_00a3d6ec->m_2c = 0;
            DAT_00a3d6ec->m_38 = 0;
            DAT_00a3d6ec->m_3c = 0;
            DAT_00a3d6ec->m_24 = 0;
            DAT_00a3d6ec->m_28 = 4;
            DAT_009a5e98.m_4[top] = 0;
            DAT_009a5e98.m_0--;
            top--;
        }
    }
}

void Game::M_00407740()
{
    int top;
    int grade;
    int sprite;
    int slot;
    int x;
    int y;

    top = DAT_009a5e98.m_0;
    M_00410320(m_8c4->m_7c);
    grade = m_8c4->m_7c;
    if (grade != 5) {
        switch (grade) {
        case 1:  sprite = M_0040bf80(0x7b, 0); break;
        case 2:  sprite = M_0040bf80(0x7b, 1); break;
        case 3:  sprite = M_0040bf80(0x7b, 2); break;
        case 4:  sprite = M_0040bf80(0x7b, 3); break;
        default: sprite = M_0040bf80(3, 0x21); break;
        }
        x = m_8c4->m_24 / 2 - 0x10 + m_8c4->m_1c;
        y = m_8c4->m_28 / 2 - 0x10 + m_8c4->m_20;
        slot = M_004067d0(top + 1, x, y, (0x13 - top) * 0x20, 0, sprite, 8, 0, 0, 0, 0xe);
        if (slot >= 0) {
            DAT_00a3d6ec->m_4 = 1;
            if (slot != DAT_00430658) {
                DAT_009a5e98.m_4[DAT_009a5e98.m_0] = m_8c4->m_7c;
                DAT_009a5e98.m_0++;
                if (DAT_009a5e98.m_0 > 3)
                    M_004068e0(DAT_009a5e98.m_0 - 3);
            }
        }
    }
}

void Game::M_00407850()
{
    if (m_a795c == 4) {
        if (m_87fcc4 > 0)
            M_00404110(m_87fcc4);
        M_004066d0(1);
    }
    M_004016b0();
    M_004085d0();
    M_0040f8a0();
    M_00407f70();
    M_0040c690();
    M_00409db0();
    M_0040a210();
    M_0040a5c0();
    if (m_a795c == 4 && DAT_00a4625c != 0) {
        M_004010d0();
        if (DAT_00a45e28 != 1) {
            m_a795c = 8;
            return;
        }
        M_004010d0();
        m_a795c = 0xb;
    }
}
