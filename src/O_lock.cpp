#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>
#include "game.h"
int FUN_00413350(IDirectDrawSurface *surf, unsigned char *dst, unsigned char fgIndex, unsigned char bgIndex, unsigned int fgPixel, unsigned int bgPixel)
{
    DDSURFACEDESC desc;
    HRESULT hr;
    unsigned char *src;
    unsigned char *end;
    unsigned char pixel;
    int y;
    int rowSkip;

    desc.dwSize = sizeof(desc);
    while (1) {
        hr = surf->Lock(NULL, &desc, 0, NULL);
        if (hr == DD_OK)
            break;
        if (hr == DDERR_SURFACELOST)
            return 0;
        if (hr != DDERR_WASSTILLDRAWING)
            return 0;
    }
    src = (unsigned char *)desc.lpSurface;
    rowSkip = desc.lPitch - desc.dwWidth;
    for (y = desc.dwHeight; y > 0; y--) {
        end = src + desc.dwWidth;
        while (src < end) {
            pixel = *src++;
            if (pixel == fgPixel)
                pixel = fgIndex;
            if (pixel == bgPixel)
                pixel = bgIndex;
            *dst++ = pixel;
        }
        src += rowSkip;
    }
    surf->Unlock(NULL);
    return 1;
}
