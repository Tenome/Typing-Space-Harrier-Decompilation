#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "game.h"

void Game::M_00410760()
{
    DAT_008f544c = &DAT_0099d6c0;
    memset(DAT_008f544c, 0, sizeof(TypedRec));
    DAT_008f544c->m_00 = DAT_00430658;
    DAT_008f544c->m_10[0] = 0;
    DAT_008f544c->m_08 = 0;
    DAT_008f544c->m_04 = 0;
    DAT_008f544c->m_90 = 0x1c0;
    DAT_008f544c->m_94 = 0x1d0;
    DAT_008f544c->m_ac = 0;
    DAT_008f544c->m_a8 = 0;
    DAT_008f544c->m_9c = 0;
    DAT_008f544c->m_98 = 0;
}
