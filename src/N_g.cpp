#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"

int FUN_00412250()
{
    int zeros = 0;
    int value = 1;
    int i;

    while (FUN_004121a0(&DAT_00a4a680) == 0)
        zeros++;
    for (i = 0; i < zeros; i++)
        value = FUN_004121a0(&DAT_00a4a680) + value * 2;
    return value + 3;
}
