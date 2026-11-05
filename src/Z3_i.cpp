#include <string.h>
#include "game.h"

void Game::M_00406910(int n, int arg)
{
    int i = 0;
    int k;
    int count;

    if (n < 0 || DAT_009a5e98.m_0 < n)
        n = DAT_009a5e98.m_0;
    count = DAT_009a5e98.m_0;
    for (k = 0; k < 0x100; k++) {
        Obj_009a629c *o = &DAT_009a629c[k];
        DAT_00a3d6ec = o;
        if (o->m_0 != 0 && o->m_8 > 0) {
            o->m_8 -= n;
            DAT_00a3d6ec->m_1c = (0x14 - DAT_00a3d6ec->m_8) * 0x20;
            DAT_00a3d6ec->m_20 = 0;
            if (DAT_00a3d6ec->m_8 < 1) {
                DAT_00a3d6ec->m_30 = 1;
                DAT_00a3d6ec->m_3c = 0;
                DAT_00a3d6ec->m_14 = DAT_00a3d6ec->m_c;
                DAT_00a3d6ec->m_18 = DAT_00a3d6ec->m_10;
            }
            if (DAT_00a3d6ec->m_0 == 3) {
                DAT_00a3d6ec->m_3c = 0;
                DAT_00a3d6ec->m_0 = 2;
                DAT_00a3d6ec->m_24 = 0;
                DAT_00a3d6ec->m_28 = arg;
                DAT_00a3d6ec->m_14 = DAT_00a3d6ec->m_c;
                DAT_00a3d6ec->m_18 = DAT_00a3d6ec->m_10;
            }
            count = DAT_009a5e98.m_0;
            if (DAT_00a3d6ec->m_8 < 1) {
                count = DAT_009a5e98.m_0 - 1;
                DAT_009a5e98.m_0 = count;
            }
        }
    }
    if (count > 0) {
        do {
            DAT_009a5e98.m_4[i] = DAT_009a5e98.m_4[n + i];
            i++;
            count = DAT_009a5e98.m_0;
        } while (i < DAT_009a5e98.m_0);
    }
    if (count < 0x100)
        memset(&DAT_009a5e98.m_4[count], 0, (0x100 - count) * sizeof(int));
}
