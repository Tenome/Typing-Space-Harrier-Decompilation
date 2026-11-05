#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

int Game::M_00410440(int index)
{
    int unit = DAT_004bddf8[index].m_1c;
    return M_004103c0(unit);
}
