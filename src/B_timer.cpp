#include "game.h"

int Game::M_00409300(int n)
{
    float half;

    half = M_0040b390(n) * 0.5f;
    return 0x146 - (int)(half * -134.0f);
}

void Game::M_00409ca0(int n)
{
    DAT_004efa18 = &DAT_00a3ce58[n];
}

void Game::M_00409cc0()
{
    int i;

    i = 0;
    do {
        M_00409ce0(i);
        i++;
    } while (i < 0x40);
}

void Game::M_00409d50(int a, int b, int c, int d)
{
    int slot;

    slot = M_00409d10();
    if (slot >= 0) {
        M_00409ca0(slot);
        DAT_004efa18->m_0 = 1;
        DAT_004efa18->m_4 = a;
        DAT_004efa18->m_8 = b;
        DAT_004efa18->m_c = c;
        DAT_004efa18->m_10 = d;
    }
}
