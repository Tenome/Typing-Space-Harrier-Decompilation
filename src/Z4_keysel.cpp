#include "game.h"

int FUN_0041d0d0(int key, int *done)
{
    int row;
    int col;
    int alive;
    int center;
    int steps;
    int first;
    int last;
    int n;
    int cell;

    if (DAT_011215a8 == 0)
        return 0;
    FUN_0041c790();
    DAT_0101fdb0++;
    if (DAT_0101fdb0 > 0) {
        DAT_0101fdb0 = 0;
        DAT_0101f420--;
    }
    if (DAT_0101f420 < 0) {
        FUN_0041d950(2);
        alive = 0;
        *done = 1;
        DAT_0101f420 = 0;
    } else {
        *done = 0;
        alive = 1;
        steps = 0;
        n = DAT_004ec81c[DAT_0101e5e0];
        while (steps < n) {
            if ((steps + 1) * 0x100 / n > 0xff - DAT_0101f420)
                break;
            steps++;
        }
        switch (DAT_0101e5e0) {
        case 0:
        case 1:
        case 2:
        case 7:
        case 8:
            first = DAT_0101e5e0 * 4;
            center = DAT_0101f454 + first;
            last = DAT_0101e5e0 * 4 + 3;
            break;
        case 3:
        case 5:
        case 9:
            first = DAT_0101e5e0 * 4;
            center = DAT_0101f454 + first;
            last = DAT_0101e5e0 * 4 + 7;
            if (DAT_0101e5e0 == 9)
                last += 4;
            break;
        case 4:
        case 6:
        case 10:
            first = DAT_0101e5e0 * 4 - 4;
            center = DAT_0101f454 + first + 4;
            last = DAT_0101e5e0 * 4 + 3;
            if (DAT_0101e5e0 == 10)
                last += 4;
            break;
        case 11:
            first = 0x24;
            center = DAT_0101f454 + 0x2c;
            last = 0x2f;
            break;
        }
        if (last > 0x2f)
            last = 0x2f;
        for (cell = first; cell <= last; cell++) {
            if (cell / 4 == 0xb && cell % 4 == 3)
                FUN_0041ad10(1, 0xc, 0, 0, 0, 3);
            else
                FUN_0041ad10(1, cell / 4, cell % 4, 0, 0, 3);
        }
        while (steps--) {
            int col2;
            int row2;
            if (last - center < center - first) {
                col2 = first / 4;
                row2 = first % 4;
                first++;
            } else {
                col2 = last / 4;
                row2 = last % 4;
                last--;
            }
            if (col2 == 0xb && row2 == 3) {
                col2 = 0xc;
                row2 = 0;
            }
            FUN_0041ad10(0, col2, row2, 0, 0, 3);
        }
        if ((key >= 0x30 && key <= 0x39) || (key >= 0x41 && key <= 0x5a)) {
            FUN_0041ad10(1, -1, -1, key, 0, 0);
            if (FUN_0041adc0(key, 0, &col, &row) != 0) {
                if (col == DAT_0101e5e0 && row == DAT_0101f454) {
                    FUN_0041d950(1);
                    alive = 0;
                } else {
                    FUN_0041d950(2);
                }
            }
        }
    }
    FUN_0041d080(DAT_004ec7bc[0].x, DAT_004ec7bc[0].y + 0x3c, DAT_0101f420);
    FUN_0041cf70(0);
    if (alive == 0 && *done == 0)
        DAT_0101f420 = -1;
    return alive;
}
