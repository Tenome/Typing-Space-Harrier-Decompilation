#include <windows.h>
#include <ddraw.h>
#include "game.h"

void FUN_00412730()
{
    if (DAT_00a4b0a8 != 0) {
        if (DAT_00a4a6fc != 0) {
            if (DAT_00a4b094 != 0) {
                DAT_00a4b094->Release();
                DAT_00a4b094 = 0;
            }
            if (DAT_00a4a824 != 0) {
                DAT_00a4a824->Release();
                DAT_00a4a824 = 0;
            }
            if (DAT_00a4a87c != 0) {
                DAT_00a4a87c->Release();
                DAT_00a4a87c = 0;
            }
            DAT_00a4a6fc->Release();
            DAT_00a4a6fc = 0;
        }
        DAT_00a4b0a8 = 0;
    }
}

extern "C" int DisplayReady()
{
    if (DAT_00a4b0a8 == 0)
        return 0;
    if (FUN_004127d0() == 0)
        return 0;
    if (PaletteChangeHook != 0 && PaletteChangeHook() == 0)
        return 0;
    return 1;
}

int FUN_004127d0()
{
    if (DAT_00a4b094->Restore() == 0 && (DAT_00a4a824 == 0 || DAT_00a4a824->Restore() == 0))
        return 1;
    return 0;
}

unsigned long FUN_00412880(IDirectDrawSurface *surf, COLORREF color)
{
    HDC dc;
    COLORREF saved;
    DDSURFACEDESC desc;
    HRESULT result;
    unsigned long value = 0xffffffff;

    if (color == 0xffffffff || surf->GetDC(&dc) != 0) {
        saved = (COLORREF)dc;
    } else {
        saved = GetPixel(dc, 0, 0);
        SetPixel(dc, 0, 0, color);
        surf->ReleaseDC(dc);
    }
    desc.dwSize = sizeof(desc);
    result = surf->Lock(0, &desc, 0, 0);
    while (result == DDERR_WASSTILLDRAWING)
        result = surf->Lock(0, &desc, 0, 0);
    if (result == 0) {
        if ((desc.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXEDTO8) != 0) {
            value = *(unsigned char *)desc.lpSurface;
        } else {
            value = *(unsigned long *)desc.lpSurface;
            if (desc.ddpfPixelFormat.dwRGBBitCount < 0x20)
                value &= (1 << desc.ddpfPixelFormat.dwRGBBitCount) - 1;
        }
        surf->Unlock(0);
    }
    if (color != 0xffffffff && surf->GetDC(&dc) == 0) {
        SetPixel(dc, 0, 0, saved);
        surf->ReleaseDC(dc);
    }
    return value;
}
