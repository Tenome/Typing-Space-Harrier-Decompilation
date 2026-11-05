#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "game.h"
extern int DAT_00a460c4;
extern char DAT_00a461d0[];

void FUN_0041e190()
{
    int i;

    DAT_00a4625c = 0;
    DAT_00a46260 = 0;
    DAT_00a460c4 = 0;
    DAT_00a461d0[0] = 0;
    for (i = 0; i < 8; i++)
        (&DAT_00a46174)[i] = 0;
    for (i = 0; i < 8; i++)
        DAT_00a46264[i] = 0;
}
