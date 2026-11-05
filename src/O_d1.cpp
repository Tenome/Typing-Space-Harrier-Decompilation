#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

void FUN_004147c0(PaletteTable *dst, const PaletteTable *src)
{
    int i;

    for (i = 0; i < 256; i++) {
        if (src->used[i] == 1) {
            if (dst == &DAT_00a4a890 &&
                (dst->entry[i].r != src->entry[i].r || dst->entry[i].g != src->entry[i].g ||
                 dst->entry[i].b != src->entry[i].b || dst->entry[i].flags != src->entry[i].flags))
                DAT_00a4a88c = 1;
            dst->entry[i] = src->entry[i];
            dst->used[i] = 1;
        }
    }
}
