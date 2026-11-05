#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_00405b80()
{
    char *key;
    char *p;
    int *out;
    int n;
    char next;

    memset(DAT_00a23310, 0, sizeof DAT_00a23310);
    if (DAT_00429040[0].kana[0] != 0) {
        key = DAT_00429040[0].kana;
        out = DAT_00a23310;
        do {
            n = 0;
            p = key;
            do {
                p += 7;
                if (*p == 0)
                    break;
                n++;
            } while (n < 4);
            next = key[0x24];
            key += 0x24;
            *out = n;
            out++;
        } while (next != 0);
    }
}
