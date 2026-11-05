#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "game.h"

int FUN_004121a0(StreamRec *stream)
{
    int mask = DAT_004ebb1c;

    if (mask == 1) {
        DAT_00a4a6e0 = FUN_00411d20(stream);
        if (DAT_00a4a6e0 != 0xffffffff)
            mask = 0x80;
        else
            return -1;
    } else {
        mask >>= 1;
    }
    DAT_004ebb1c = mask;
    return (DAT_00a4a6e0 & mask) != 0;
}

void Game::M_00405bd0()
{
    int count;
    int last;
    char **name;

    DAT_00a45e30 = 0;
    DAT_00a45e2c = 0;
    DAT_00a45e3c = 0;
    last = 0;
    count = 0;
    if (*DAT_004bd85c[0] != 0) {
        name = DAT_004bd85c;
        do {
            last = count;
            name++;
            count++;
        } while (**name != 0);
    }
    DAT_00a23304 = ((last + 4) / 10) * 5;
    DAT_00a23300 = last + 1;
    if (DAT_00a23300 > 10)
        DAT_00a23300 = 10;
}
