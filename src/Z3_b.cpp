#include "game.h"

int FUN_004121f0(int bits)
{
    int value = 0;
    int top = 1;
    int bit;

    while (top * 2 < bits) {
        top *= 2;
        value = FUN_004121a0(&DAT_00a4a680) + value * 2;
    }
    top <<= 1;
    if (value >= top - bits) {
        bit = FUN_004121a0(&DAT_00a4a680);
        return value * 2 - top + bits + bit;
    }
    return value;
}
