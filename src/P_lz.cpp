#include "game.h"

int FUN_004120e0()
{
    int bit;
    int dist;
    int len;
    int copied;
    int i;
    int rest;
    int byte;

    bit = FUN_00412190();
    if (bit == -1)
        return -1;
    if (bit == 0) {
        byte = FUN_004122a0();
        if (byte == -1)
            return -1;
        FUN_004122d0(byte);
        FUN_00412350(1);
        return 1;
    }
    dist = FUN_004121f0(FUN_004123f0());
    len = FUN_00412250();
    copied = 0;
    rest = len;
    for (i = 0; i < len; i++) {
        if (FUN_004122d0(FUN_00412300(i + dist)) == 0) {
            FUN_00412350(copied);
            dist -= copied;
            rest -= copied;
            copied = 0;
        }
        copied++;
    }
    FUN_00412350(rest);
    return 1;
}
