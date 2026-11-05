#include "game.h"

int Game::M_0040cdd0(int index, int kind, int a3, int a4, int a5, int a6)
{
    int i;

    if (FUN_0040cf20() < 0)
        return DAT_00430658;
    i = 0;
    DAT_004f052c->m_00 = 1;
    DAT_004f052c->m_04 = 1;
    DAT_004f052c->m_08 = a4;
    DAT_004f052c->m_0c = index;
    DAT_004f052c->m_10 = a3;
    DAT_004f052c->m_14 = kind;
    DAT_004f052c->m_b0 = DAT_00430658;
    DAT_004f052c->m_28 = 0;
    DAT_004f052c->m_24 = 0;
    DAT_004f052c->m_1c = a5;
    DAT_004f052c->m_20 = a6;
    DAT_004f052c->m_18 = 0;
    DAT_004f052c->m_24 = 0;
    for (; i < 0x21; i++)
        DAT_004f052c->m_2c[i] = -1;
    switch (kind) {
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
        DAT_004f052c->m_28 = kind - 0x32;
        DAT_004f052c->m_24 = 1;
        m_8e3cc8[index] = 0;
        return 0;
    case 0x38:
    case 0x39:
    case 0x3a:
    case 0x3b:
    case 0x3c:
        DAT_004f052c->m_28 = kind - 0x37;
        DAT_004f052c->m_24 = 1;
    }
    m_8e3cc8[index] = 0;
    return 0;
}
