#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <string.h>
#include "game.h"
int FUN_00412800()
{
    int ok;
    HRESULT result;

    if (DAT_00a4b0a8 == 0)
        return 0;
    ok = 1;
    if (DAT_00a4a87c != 0 && DAT_00a4a88c == 1) {
        if (DAT_00a4a87c->SetEntries(0, 0, 256, (PALETTEENTRY *)&DAT_00a4a890) != 0)
            return 0;
        DAT_00a4a88c = 0;
    }
    while (1) {
        result = DAT_00a4b094->Flip(0, 0);
        if (result == 0)
            break;
        if (result == DDERR_SURFACELOST) {
            ok = 0;
            break;
        }
        if (result != DDERR_WASSTILLDRAWING)
            break;
    }
    return ok;
}
