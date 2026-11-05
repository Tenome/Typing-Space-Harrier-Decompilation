#include "game.h"

int FUN_004136f0(const unsigned char *ch)
{
    unsigned int i;
    int base = 0;
    unsigned char c = *ch;
    int v = c;

    if ((c & 0xe0) == 0x80 || (c & 0xe0) == 0xe0)
        return -1;
    for (i = 0; i < 4; i++) {
        if ((i & 1) == 0) {
            if (c < DAT_004ebb2c[i])
                return -1;
        } else {
            if (c < DAT_004ebb2c[i])
                return base + (v - DAT_004ebb2b[i]);
            base += DAT_004ebb2c[i] - DAT_004ebb2b[i];
        }
    }
    return base;
}
