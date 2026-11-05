#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

int Game::M_00406be0()
{
    int idx, k, run, value;
    int *p;

    if (DAT_009a5e98.m_0 < 3)
        return 0;
    idx = DAT_009a5e98.m_0 - 1;
    k = 0;
    value = DAT_009a5e98.m_4[idx];
    p = &DAT_009a5e98.m_4[idx];
    run = 1;
    if (DAT_009a5e98.m_0 > 0) {
        do {
            idx--;
            p--;
            if (idx < 0 || *p != value)
                break;
            run++;
            k++;
        } while (k < DAT_009a5e98.m_0);
        if (run > 2) {
            M_00407590(run);
            M_00407310(value);
        }
    }
    return value;
}
