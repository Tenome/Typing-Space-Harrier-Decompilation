#include "game.h"

void Game::M_004078f0()
{
    int seen[7];
    int cur;
    int k;
    int sprite;

    DAT_004efbd4 = 0;
    for (k = 0; k < 7; k++)
        DAT_004f0534[k] = 0;
    for (k = 0; k < 7; k++)
        DAT_004efa1c[k] = 0;
    DAT_008f0ff4 = 0;
    DAT_004efa14 = 0;
    for (k = 0; k < 7; k++)
        seen[k] = 0;
    for (;;) {
        cur = DAT_00430658;
        M_00407c90(1);
        switch (DAT_004f065c[0]) {
        case -100000:
            DAT_00a46048 = 0;
            for (k = 0; k < 0x255; k++)
                DAT_004efbd8[k] = 0;
            for (k = 0; k < 0x255; k++)
                DAT_008f069c[k] = 0;
            for (k = 0; k < 7; k++)
                DAT_004f0534[k] = 0;
            for (k = 0; k < 7; k++)
                DAT_004efa1c[k] = 0;
            for (k = 0; k < 7; k++)
                seen[k] = 0;
            DAT_00a3d704 = 1;
            DAT_0099cfd0 = 1;
            DAT_008f0ff4 = 0;
            DAT_004efa14 = 0;
            break;
        case -99999:
            for (k = 0; k < 7; k++)
                seen[k] = 0;
            M_00407c90(1);
            DAT_00a46048 = DAT_004f065c[0];
            if (DAT_00a46048 < 0 || DAT_00a46048 >= 0x255)
                return;
            DAT_008f069c[DAT_00a46048] = 0;
            DAT_004efbd8[DAT_00a46048] = DAT_0099cfd0;
            DAT_008f0ff4 = 0;
            DAT_004efa14 = 0;
            break;
        case -99998:
            M_00407c90(8);
            M_00407cd0(DAT_004f0534[0] + DAT_004f065c[0], DAT_004f0534[1] + DAT_004f065c[1],
                       DAT_004f0534[2] + DAT_004f065c[2], DAT_004f0534[3] + DAT_004f065c[3],
                       DAT_004f0534[4] + DAT_004f065c[4], DAT_004f0534[5] + DAT_004f065c[5],
                       DAT_004f0534[6] + DAT_004f065c[6], DAT_004efbd8[DAT_00a46048] + DAT_004f065c[7]);
            DAT_008f0ff4++;
            DAT_008f069c[DAT_00a46048]++;
            break;
        case -99997:
            return;
        case -99996:
            cur = 0;
            if (!seen[0]) {
                seen[0] = 1;
                DAT_004f0534[0] = DAT_00a3d704;
                DAT_004efa1c[0] = 0;
            }
            break;
        case -99995:
            cur = 1;
            if (!seen[1]) {
                seen[1] = 1;
                DAT_004f0534[1] = DAT_00a3d704;
                DAT_004efa1c[1] = 0;
            }
            break;
        case -99994:
            cur = 2;
            if (!seen[2]) {
                seen[2] = 1;
                DAT_004f0534[2] = DAT_00a3d704;
                DAT_004efa1c[2] = 0;
            }
            break;
        case -99992:
            cur = 4;
            if (!seen[4]) {
                seen[4] = 1;
                DAT_004f0534[4] = DAT_00a3d704;
                DAT_004efa1c[4] = 0;
            }
            break;
        case -99991:
            cur = 5;
            if (!seen[5]) {
                seen[5] = 1;
                DAT_004f0534[5] = DAT_00a3d704;
                DAT_004efa1c[5] = 0;
            }
            break;
        case -99990:
            cur = 6;
            if (!seen[6]) {
                seen[6] = 1;
                DAT_004f0534[6] = DAT_00a3d704;
                DAT_004efa1c[6] = 0;
            }
            break;
        case -99993:
            if (!seen[3]) {
                seen[3] = 1;
                DAT_004f0534[3] = DAT_00a3d704;
                DAT_004efa1c[3] = 0;
            }
            DAT_004efa1c[3]++;
            M_00407c90(4);
            sprite = M_0040bf80(DAT_004f065c[0], DAT_004f065c[1]);
            M_00407e40(sprite, 0, DAT_004f065c[2], DAT_004f065c[3]);
            continue;
        }
        if (cur >= 0) {
            DAT_004efa1c[cur]++;
            M_00407c90(4);
            M_00407e40(DAT_004f065c[0], DAT_004f065c[1], DAT_004f065c[2], DAT_004f065c[3]);
        }
    }
}
