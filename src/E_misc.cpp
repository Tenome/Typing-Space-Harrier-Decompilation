#include "game.h"

void FUN_00419310(int level)
{
    if (level > 0xff)
        level = 0xff;
    else if (level < 0)
        level = 0;
    DAT_00f4dbf8 = level;
    DAT_00fb3140 = level * 180 / 255;
}

void FUN_00419350(int level)
{
    if (level > 0xff)
        level = 0xff;
    else if (level < 0)
        level = 0;
    if (level > 0x7f) {
        DAT_00f4dbf8 = level * 2 - 255;
        DAT_00fb3140 = 180;
    } else {
        DAT_00f4dbf8 = 0;
        DAT_00fb3140 = level * 180 / 127;
    }
}
