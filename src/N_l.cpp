#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "game.h"

void Game::M_00405b80()
{
    int i;
    int n;
    int k;

    memset(DAT_00a23310, 0, sizeof DAT_00a23310);
    for (i = 0; DAT_00429040[i].kana[0] != 0; i++) {
        n = 0;
        for (k = 0; k < 4; k++) {
            if (DAT_00429040[i].rom[k][0] == 0)
                break;
            n = k + 1;
        }
        DAT_00a23310[i] = n;
    }
}
