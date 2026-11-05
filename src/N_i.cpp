#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "game.h"

int Game::M_00409d10()
{
    int i;
    int none = DAT_00430658;

    for (i = 0; i < 0x40; i++) {
        M_00409ca0(i);
        if (DAT_004efa18->m_0 == 0)
            return i;
    }
    return none;
}
