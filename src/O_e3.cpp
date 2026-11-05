#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

int FUN_0041d950(int n)
{
    char path[2048];
    int slot;
    int i;

    if (n >= 0 && n <= 0x30) {
        sprintf(path, "%s%s", DAT_0101eb04, DAT_004ec3cc[n]);
        for (slot = 0; slot < 3; slot++) {
            if (FUN_004189a0(slot + 1) == 0)
                break;
        }
        if (slot == 3)
            slot = 0;
        for (i = 0; i < 3; i++) {
            if (FUN_004185a0(path, slot + 1, 1, 0, 0) != 0)
                return 1;
        }
    }
    return 0;
}
