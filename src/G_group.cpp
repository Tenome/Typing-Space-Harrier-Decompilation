#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

int Game::M_0040ca80()
{
    int i;
    int n;

    for (n = 0; n < 72; n++) {
        DAT_004f052c = &DAT_008f5450[n];
        if (DAT_004f052c->m_00 != 0) {
            if (DAT_004f052c->m_24 != 0)
                M_0040cc70();
            if (DAT_004f052c->m_08 >= 1) {
                if (m_87fcc8[DAT_004f052c->m_0c] == 2) {
                    DAT_004f052c->m_00 = 0;
                } else if (DAT_00a3d6f0 == 0 && DAT_00a45e28 != 1 && DAT_004f052c->m_04 != 0) {
                    DAT_004f052c->m_04 = 0;
                    switch (DAT_004f052c->m_14) {
                    case 0:
                        for (i = 0; i < DAT_004f052c->m_08; i++)
                            M_0040d160(DAT_004f052c->m_0c, DAT_004f052c->m_10, DAT_004f052c->m_20);
                        DAT_004f052c->m_00 = 0;
                        DAT_004f052c->m_08 = 0;
                        DAT_004f052c->m_04 = 0;
                        break;
                    case 1:
                        M_0040d160(DAT_004f052c->m_0c, DAT_004f052c->m_10, DAT_004f052c->m_20);
                        DAT_004f052c->m_04 = 0;
                        DAT_004f052c->m_08--;
                        break;
                    case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
                    case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19:
                    case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28:
                    case 29: case 30: case 31: case 32: case 33: case 34: case 35: case 36: case 37:
                    case 38: case 39: case 40: case 41: case 42: case 43: case 44: case 45: case 46:
                    case 47: case 48: case 49: case 50: case 51:
                        M_0040d2c0(DAT_004f052c->m_0c, DAT_004f052c->m_10, DAT_004f052c->m_14 - 1, DAT_004f052c->m_20);
                        DAT_004f052c->m_08--;
                        break;
                    case 52: case 53: case 54: case 55:
                        M_0040d400(DAT_004f052c->m_0c, DAT_004f052c->m_10, DAT_004f052c->m_14 - 0x32, 0, DAT_004f052c->m_20);
                        DAT_004f052c->m_08--;
                        break;
                    case 56: case 57: case 58: case 59: case 60:
                        M_0040d400(DAT_004f052c->m_0c, DAT_004f052c->m_10, DAT_004f052c->m_14 - 0x37, 1, DAT_004f052c->m_20);
                        DAT_004f052c->m_08--;
                        if (DAT_004f052c->m_14 == 0x38)
                            m_87fcc0++;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

int Game::M_0040cc70()
{
    int i = 0;
    int pending = 0;
    int last;
    int id;
    int played;

    if (DAT_004f052c->m_04 == 0 && DAT_004f052c->m_24 != 0) {
        last = DAT_00430658;
        if (DAT_004f052c->m_28 >= 0) {
            do {
                M_00401440(DAT_004f052c->m_2c[i]);
                if (m_8c8->m_0 == 0 || m_8c8->m_8 != DAT_004f052c->m_0c) {
                    pending++;
                    last = i;
                }
                i++;
            } while (i <= DAT_004f052c->m_28);
        }
        played = 0;
        switch (pending) {
        case 0:
            return 0;
        case 1:
            M_00401440(DAT_004f052c->m_2c[last]);
            if (m_8c8->m_68 > 0)
                played = 1;
            for (i = 0; i <= DAT_004f052c->m_28; i++) {
                id = DAT_004f052c->m_2c[i];
                M_00401440(id);
                if (m_8c8->m_0 != 0 && m_8c8->m_8 == DAT_004f052c->m_0c) {
                    M_00401630(id);
                    if (--m_3b8 < 0)
                        m_3b8 = 0;
                }
            }
            break;
        }
        DAT_004f052c->m_04 = 1;
        if (played == 0) {
            M_004068e0(DAT_00430658);
            M_0040f860();
        }
        return pending;
    }
    return 0;
}
