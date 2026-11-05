#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

int FUN_0041cb30(const unsigned char *text, int x, int y, int fade)
{
    int shade;
    int height;
    int width;
    int extra;

    {
        while (*text != 0) {
            if (DAT_0101e56c >= 0x200)
                return 0;
            if (FUN_00413610(text, DAT_0101fdd0->m_34, &shade, &height, &width, &extra) == 0)
                return 0;
            DAT_01022320[DAT_0101e56c].m_0c = shade;
            DAT_01022320[DAT_0101e56c].m_10 = height;
            DAT_01022320[DAT_0101e56c].m_14 = width;
            DAT_01022320[DAT_0101e56c].m_18 = extra;
            DAT_01022320[DAT_0101e56c].m_1c = x + fade * 12 / 255;
            DAT_01022320[DAT_0101e56c].m_20 = y;
            if ((*text & ~0x1f) == 0x80)
                text++;
            text++;
            x += width;
            DAT_0101e56c++;
        }
    }
    return 1;
}
