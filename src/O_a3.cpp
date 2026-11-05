#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

int Game::M_00406a50()
{
    int notFound = DAT_00430658;
    int i;

    for (i = 0; i < 0x100; i++) {
        DAT_00a3d6ec = &DAT_009a629c[i];
        if (DAT_00a3d6ec->m_0 == 0)
            return i;
    }
    return notFound;
}
