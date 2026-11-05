#include "game.h"

void Game::M_004013b0()
{
    SeEntry *e = DAT_004306e8;
    char c = *e->name;
    DAT_00901700 = e;
    while (c != 0) {
        SeEntry *cur = e;
        if (cur->id != 3)
            FUN_0041e290(cur->id);
        cur++;
        DAT_00901700 = cur;
        e = cur;
        c = *e->name;
    }
    M_004089a0(-1);
}
