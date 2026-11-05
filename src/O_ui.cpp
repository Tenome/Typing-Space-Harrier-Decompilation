#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

int FUN_0041ad10(int on, int col, int row, unsigned char key, const char *name, int kind)
{
    KeyIcon *icon;
    int slot;

    if (on == 1 && (kind < 0 || kind > 4))
        return 0;
    if (col >= 0 && col < 13 && row >= 0 && row < 4 && !(col == 11 && row == 3) && !(col == 12 && row != 0)) {
        slot = col + row * 13;
    } else {
        if (!FUN_0041adc0(key, name, &col, &row))
            return 0;
        slot = col + row * 13;
    }
    icon = &DAT_0101e350[0][slot];
    icon->on = on;
    icon->kind = kind;
    return 1;
}
