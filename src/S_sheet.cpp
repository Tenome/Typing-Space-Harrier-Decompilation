#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include "game.h"

// OPEN: register/spill order of the global snapshot differs (289 vs 279 bytes)
SheetImage *FUN_00412ec0()
{
    IDirectDrawSurface *surf = DAT_00a4a830;
    SheetImage *image = DAT_00a4a834;
    COLORREF fg = DAT_00a4a840;
    int fgIndex = DAT_00a4a848;
    COLORREF bg = DAT_00a4a844;
    DWORD fgPixel = DAT_00a4a850;
    PaletteTable *palette = DAT_00a4a838;
    DWORD bgPixel = DAT_00a4a854;
    unsigned i;
    int bgIndex = DAT_00a4a84c;
    DeleteObject(DAT_00a4a85c);
    surf->ReleaseDC(DAT_00a4a858);
    if (FUN_00413350(surf, DAT_00a4a83c, fgIndex, bgIndex, fgPixel, bgPixel) == 0) {
        surf->Release();
        return 0;
    }
    if (image->bpp == 8) {
        for (i = 0; i != 256; i++) {
            palette->used[i] = 0;
            palette->entry[i].r = 0;
            palette->entry[i].g = 0;
            palette->entry[i].b = 0;
            palette->entry[i].flags = 0;
        }
        palette->used[bgIndex] = 1;
        palette->entry[bgIndex].r = GetRValue(bg);
        palette->entry[bgIndex].g = (unsigned char)(bg >> 8);
        palette->entry[bgIndex].b = GetBValue(bg);
        palette->used[fgIndex] = 1;
        palette->entry[fgIndex].r = GetRValue(fg);
        palette->entry[fgIndex].g = (unsigned char)(fg >> 8);
        palette->entry[fgIndex].b = GetBValue(fg);
    }
    surf->Release();
    return image;
}
