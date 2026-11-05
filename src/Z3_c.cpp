#include "game.h"

int Game::M_00406be0()
{
    int k = 0;
    int idx;
    int run;
    int value;
    int *p;

    if (DAT_009a5e98.m_0 < 3)
        return 0;
    idx = DAT_009a5e98.m_0 - 1;
    value = DAT_009a5e98.m_4[idx];
    p = &DAT_009a5e98.m_4[idx];
    run = 1;
    if (DAT_009a5e98.m_0 > 0) {
        while (1) {
            idx--;
            p--;
            if (idx < 0 || *p != value)
                break;
            run++;
            k++;
            if (k >= DAT_009a5e98.m_0)
                break;
        }
        if (run > 2) {
            M_00407590(run);
            M_00407310(value);
        }
    }
    return value;
}
