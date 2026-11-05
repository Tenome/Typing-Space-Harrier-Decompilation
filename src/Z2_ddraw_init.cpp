#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <string.h>
#include "game.h"

int FUN_00412400(void *window, int width, int height, int bpp, int coopFlags, unsigned int offWidth, unsigned int offHeight, unsigned long keyHigh, unsigned long keyLow)
{
    DDSURFACEDESC desc;
    DDSCAPS caps;
    DDCOLORKEY key;
    unsigned long lowValue;
    int i;

    if (DAT_00a4b0a8 == 1)
        return 1;
    DAT_00a4a6fc = 0;
    DAT_00a4b094 = 0;
    DAT_00a4a864 = 0;
    DAT_00a4a824 = 0;
    DAT_00a4a87c = 0;
    PaletteChangeHook = 0;
    if (DirectDrawCreate(0, &DAT_00a4a6fc, 0) != 0) {
        FUN_00412730();
        return 0;
    }
    DAT_00a4a6f8 = (int)window;
    if (DAT_00a4a6fc->SetCooperativeLevel((HWND)window, coopFlags) != 0) {
        FUN_00412730();
        return 0;
    }
    DAT_00a4b090 = coopFlags;
    if (DAT_00a4a6fc->SetDisplayMode(width, height, bpp) != 0) {
        FUN_00412730();
        return 0;
    }
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    desc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    desc.dwBackBufferCount = 1;
    if (DAT_00a4a6fc->CreateSurface(&desc, &DAT_00a4b094, 0) != 0) {
        FUN_00412730();
        return 0;
    }
    caps.dwCaps = DDSCAPS_BACKBUFFER;
    if (DAT_00a4b094->GetAttachedSurface(&caps, &DAT_00a4a864) != 0) {
        FUN_00412730();
        return 0;
    }
    DAT_00a4a6f4 = width;
    DAT_00a4b0a4 = height;
    DAT_00a4a6f0 = bpp;
    if (bpp != 8) {
        DAT_00a4a87c = 0;
    } else {
        DAT_00a4a88c = 0;
        for (i = 0; i < 256; i++) {
            DAT_00a4a890.used[i] = 0;
            DAT_00a4a890.entry[i].r = 0;
            DAT_00a4a890.entry[i].g = 0;
            DAT_00a4a890.entry[i].b = 0;
            DAT_00a4a890.entry[i].flags = 0;
        }
        if (DAT_00a4a6fc->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, (PALETTEENTRY *)DAT_00a4a890.entry, &DAT_00a4a87c, 0) != 0) {
            FUN_00412730();
            return 0;
        }
        if (DAT_00a4a87c != 0 && DAT_00a4b094->SetPalette(DAT_00a4a87c) != 0) {
            FUN_00412730();
            return 0;
        }
    }
    if (offWidth > 0 && offHeight > 0) {
        memset(&desc, 0, sizeof(desc));
        desc.dwSize = sizeof(desc);
        desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
        desc.dwWidth = offWidth;
        desc.dwHeight = offHeight;
        if (DAT_00a4a6fc->CreateSurface(&desc, &DAT_00a4a824, 0) != 0) {
            FUN_00412730();
            return 0;
        }
        DAT_00a4a860 = offWidth;
        DAT_00a4a820 = bpp;
        DAT_00a4b0a0 = offHeight;
    } else {
        DAT_00a4a824 = 0;
    }
    DAT_00a4a888 = keyLow;
    DAT_00a4b098 = keyHigh;
    if (DAT_00a4a824 != 0) {
        key.dwColorSpaceHighValue = FUN_00412880(DAT_00a4a824, keyHigh);
        lowValue = key.dwColorSpaceHighValue;
        if (keyHigh != keyLow)
            lowValue = FUN_00412880(DAT_00a4a824, keyLow);
        key.dwColorSpaceLowValue = lowValue;
        if (DAT_00a4a824->SetColorKey(DDCKEY_SRCBLT, &key) != 0) {
            FUN_00412730();
            return 0;
        }
        DAT_00a4a880 = key.dwColorSpaceLowValue;
        DAT_00a4a884 = key.dwColorSpaceHighValue;
    }
    DAT_00a4b0a8 = 1;
    return 1;
}
