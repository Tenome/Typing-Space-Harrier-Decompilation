#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"

/* Functions that are written but do not match yet. Each carries the diagnosed residue. */

int FUN_004121a0(StreamRec *stream)
{
    if (DAT_004ebb1c == 1) {
        DAT_00a4a6e0 = FUN_00411d20(stream);
        if (DAT_00a4a6e0 == 0xffffffff)
            return -1;
        DAT_004ebb1c = 0x80;
        return (DAT_00a4a6e0 & DAT_004ebb1c) != 0;
    }
    DAT_004ebb1c = DAT_004ebb1c >> 1;
    return (DAT_00a4a6e0 & DAT_004ebb1c) != 0;
}
