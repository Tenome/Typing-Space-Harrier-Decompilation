#include <string.h>
#include "game.h"

void FUN_0041c780(int v)
{
    DAT_0101e574 = v;
}

void FUN_0041c790()
{
    int row;
    int col;

    for (row = 0; row < 4; row++)
        for (col = 0; col < 13; col++)
            DAT_0101e350[row][col].on = 0;
}

void FUN_0041c7b0(int v)
{
    DAT_0102231c = v;
    if (v == 0)
        DAT_0101eac0 = v;
}

void FUN_0041c7d0(int a, int b)
{
    DAT_01022310 = a;
    DAT_0101fc94 = b;
}

void FUN_0041c7f0(int v)
{
    int i;

    DAT_01020618 = v;
    if (v == 1) {
        for (i = 0; i < 4; i++)
            FUN_0041c860(i, 1);
        FUN_0041c880(1);
        FUN_0041c890(1);
        FUN_0041c7b0(1);
        return;
    }
    for (i = 0; i < 4; i++)
        FUN_0041c860(i, 0);
    FUN_0041c880(0);
    FUN_0041c890(0);
    FUN_0041c7b0(0);
}

void FUN_0041c860(int i, int v)
{
    if (i >= 0 && i < 4)
        DAT_0101f410[i] = v;
}

void FUN_0041c880(int v)
{
    DAT_0101fdac = v;
}

void FUN_0041c890(int v)
{
    DAT_0101fda4 = v;
}

void FUN_0041cac0(int i, int a, int b)
{
    if (i >= 0 && i < 4) {
        DAT_01121590[i] = a;
        DAT_0101e55c[i] = b;
    }
}

void FUN_0041caf0(int a, int b)
{
    DAT_0101eac8 = a;
    DAT_0101eac4 = b;
}

void FUN_0041cb10(int a, int b)
{
    DAT_0101e5dc = a;
    DAT_0101fc90 = b;
}

void FUN_0041cc30(int v)
{
    DAT_0102061c = v;
    if (v == 1) {
        FUN_0041c860(DAT_0101fdb4, 1);
        return;
    }
    FUN_0041c860(DAT_0101fdb4, 0);
    DAT_0102230c = 0;
    DAT_01022314 = 0;
}

void FUN_0041cd70(int v)
{
    DAT_0101fdb8 = v;
    if (v == 0) {
        FUN_0041ad10(0, 3, 2, 0, 0, 0);
        FUN_0041ad10(0, 6, 2, 0, 0, 0);
    }
}

int FUN_0041cda0(int fadeIn)
{
    int alive = 1;
    int level;

    if (DAT_01022318 != 0) {
        level = 0xff;
        DAT_01022318 = 0;
    } else {
        level = DAT_0101e5e4;
    }
    level = level * 3 / 4;
    DAT_0101e5e4 = level;
    if (level == 0) {
        DAT_01022318 = 1;
        alive = 0;
    }
    if (fadeIn)
        FUN_0041ce10(level);
    else
        FUN_0041ce10(0xff - level);
    return alive;
}

void FUN_0041ce10(int v)
{
    if (v >= 0 && v <= 0xff)
        DAT_01022308 = v;
}

void FUN_0041cf60(int i, int v)
{
    DAT_0101f424[i] = v;
}

int FUN_0041cf70(int i)
{
    int x;
    int y;

    if (i < 0 || i >= 12 || DAT_01121560[i] == 0)
        return 0;
    x = DAT_004ec7bc[i].x;
    DAT_0101fdd8[i][0].m_1c = x + 14;
    y = DAT_004ec7bc[i].y;
    DAT_0101fdd8[i][0].m_20 = y + 14;
    FUN_0041cfd0(i, x, y);
    return 1;
}

void FUN_0041cfd0(int i, int x, int y)
{
    DAT_0101ead4[i] = x;
    DAT_0101fc60[i] = y;
}

void FUN_0041cff0(int a, int b, int c)
{
    if (a != DAT_011215a8) {
        DAT_0101f420 = 0xff;
        DAT_0101fdb0 = 0;
        DAT_0101e5e0 = b;
        DAT_0101f454 = c;
    }
    DAT_011215a8 = a;
    FUN_0041ce30(0, a, b, c);
    FUN_0041d070(a);
    if (a != 0)
        FUN_0041d080(DAT_004ec7bc[0].x, DAT_004ec7bc[0].y + 0x3c, DAT_0101f420);
}

void FUN_0041d070(int v)
{
    DAT_0101ea08 = v;
}

void FUN_0041d080(int x, int y, int alpha)
{
    if (alpha >= 0 && alpha <= 0xff) {
        DAT_0101fda0 = x;
        DAT_01021800 = y;
        DAT_0101fc98.m_24 = alpha * 255 / 255;
    }
}
