#include <string.h>
#include "game.h"

void Game::M_0040f330()
{
    int n;

    DAT_008f1010 = DAT_004ef908;
    M_0040f380();
    DAT_0099d774 = 0;
    DAT_00a0a29c = DAT_004e0ef8;
    if (DAT_00a0a29c->m_00 != 0) {
        n = 1;
        do {
            DAT_0099d774 = n;
            DAT_00a0a29c++;
            n = DAT_0099d774 + 1;
        } while (DAT_00a0a29c->m_00 != 0);
    }
}

void Game::M_0040f380()
{
    memset(DAT_008f1010, 0, 0x108);
    *DAT_008f1010 = -1;
}

void FUN_0040f060()
{
    DrawObj obj;

    obj.m_00 = 1;
    obj.m_04 = 0;
    obj.m_08 = 0;
    obj.m_20 = 0;
    obj.m_1c = 0;
    obj.m_50 = DAT_00a2330c[7];
    FUN_00413a90((BlitRequest *)&obj);
}
