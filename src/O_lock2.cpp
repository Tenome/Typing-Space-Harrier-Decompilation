#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>
#include "game.h"

int FUN_00413280(IDirectDrawSurface *surf, unsigned char value)
{
    DDSURFACEDESC desc;
    HRESULT hr;
    unsigned char *line;
    int y;
    int rowSkip;
    unsigned char *end;

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
    rowSkip = desc.lPitch - desc.dwWidth;
    line = (unsigned char *)desc.lpSurface;
    for (y = desc.dwHeight; y > 0; y--) {
        end = line + desc.dwWidth;
        while (line < end)
            *line++ = value;
        line += rowSkip;
    }
    surf->Unlock(NULL);
    return 1;
}
