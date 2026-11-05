#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "game.h"

int Game::M_004103c0(int index)
{
    int i;
    int found = 0;
    int base = DAT_004efbd8[index];
    int span = DAT_008f069c[index];

    i = 0;
    if (base <= i || span == i)
        return 0;
    for (i = 0; i < span; i++, base++) {
        M_0040af40(3, base);
        if (0 < m_87cbd4->m_2c[24]) {
            found = 1;
            break;
        }
    }
    return found;
}
