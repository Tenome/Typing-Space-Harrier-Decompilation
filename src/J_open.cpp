#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define FUN_00419e80 STUB_00419e80
#include "game.h"
#undef FUN_00419e80
int FUN_0041d940();

// OPEN: 0x41a880 - 1168 vs 1124 bytes. Induction base of the 8-case switch stores is DAT_01021808[i][0].m_20 here, original uses [1].m_20 (esi=0x1021880); ea08 block uses add where original uses lea (stores interleaved); final 0xeacc block register/schedule (original loads fdcc early). Class: register/schedule + loop IV choice.
void FUN_0041a880()
{
    int i;
    int j;
    unsigned int k;
    int scale;
    int x;
    int y;

    FUN_00413a90((BlitRequest *)&DAT_0101e4f8);
    if (DAT_0101fdb8 == 1) {
        if (DAT_0101e550 < 6) {
            FUN_0041ad10(1, 3, 2, 0, 0, 0);
            FUN_0041ad10(1, 6, 2, 0, 0, 0);
        } else {
            FUN_0041ad10(0, 3, 2, 0, 0, 0);
            FUN_0041ad10(0, 6, 2, 0, 0, 0);
        }
        DAT_0101e550 = (DAT_0101e550 + 1) % 9;
    } else {
        DAT_0101e550 = 0;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 13; j++) {
            if (DAT_0101e350[i][j].on == 1) {
                DAT_01020620[i][j].m_50 = ((int *)DAT_0101fdd0)[2 + DAT_0101e350[i][j].kind];
                FUN_00413a90((BlitRequest *)&DAT_01020620[i][j]);
            }
        }
    }
    if (DAT_0101e574 == 1) {
        DAT_0101f3b8.m_20 = DAT_01022308 * 0x93 / 0xff + 0x14d;
        FUN_00413a90((BlitRequest *)&DAT_0101f3b8);
    }
    if (DAT_0101e558 == 1)
        FUN_00413a90((BlitRequest *)&DAT_0101fd48);
    if (DAT_0101fdac == 1) {
        DAT_0101e578.m_1c = DAT_0101eac8;
        DAT_0101e578.m_20 = DAT_0101eac4;
        FUN_00413a90((BlitRequest *)&DAT_0101e578);
    }
    for (i = 0; i < 4; i++) {
        if (DAT_0101f410[i] == 1) {
            for (k = 0; k < 8; k++) {
                switch (k) {
                case 0:
                    DAT_01021808[i * 8 + 0].m_1c = DAT_01121590[i];
                    DAT_01021808[i * 8 + 0].m_20 = DAT_0101e55c[i];
                    break;
                case 1:
                    DAT_01021808[i * 8 + 1].m_20 = DAT_0101e55c[i];
                    DAT_01021808[i * 8 + 1].m_1c = DAT_01121590[i] + 0x7e;
                    break;
                case 2:
                    DAT_01021808[i * 8 + 2].m_1c = DAT_01121590[i];
                    DAT_01021808[i * 8 + 2].m_20 = DAT_0101e55c[i] + 0x12;
                    break;
                case 3:
                    DAT_01021808[i * 8 + 3].m_1c = DAT_01121590[i] + 0x7e;
                    DAT_01021808[i * 8 + 3].m_20 = DAT_0101e55c[i] + 0x12;
                    break;
                case 4:
                    DAT_01021808[i * 8 + 4].m_1c = DAT_01121590[i] + 6;
                    DAT_01021808[i * 8 + 4].m_20 = DAT_0101e55c[i];
                    break;
                case 5:
                    DAT_01021808[i * 8 + 5].m_1c = DAT_01121590[i] + 6;
                    DAT_01021808[i * 8 + 5].m_20 = DAT_0101e55c[i] + 0x12;
                    break;
                case 6:
                    DAT_01021808[i * 8 + 6].m_1c = DAT_01121590[i];
                    DAT_01021808[i * 8 + 6].m_20 = DAT_0101e55c[i] + 6;
                    break;
                case 7:
                    DAT_01021808[i * 8 + 7].m_1c = DAT_01121590[i] + 0x7e;
                    DAT_01021808[i * 8 + 7].m_20 = DAT_0101e55c[i] + 6;
                    break;
                }
                FUN_00413a90((BlitRequest *)&DAT_01021808[i * 8 + k]);
            }
        }
    }
    if (DAT_0101fda4 == 1) {
        DAT_0101ea10.m_1c = DAT_0101e5dc;
        DAT_0101ea10.m_20 = DAT_0101fc90;
        FUN_00413a90((BlitRequest *)&DAT_0101ea10);
    }
    for (i = 0; i < 12; i++) {
        if (DAT_0101f424[i] == 1) {
            DAT_0101e5e8[i].m_1c = DAT_0101ead4[i];
            DAT_0101e5e8[i].m_20 = DAT_0101fc60[i];
            FUN_00413a90((BlitRequest *)&DAT_0101e5e8[i]);
        }
        if (DAT_01121560[i] == 1)
            FUN_00413a90((BlitRequest *)&DAT_0101fdd8[i][0]);
    }
    for (i = 0; i < (int)DAT_0101e56c; i++)
        FUN_00413a90((BlitRequest *)&DAT_01022320[i]);
    DAT_0101e56c = 0;
    if (DAT_0102231c == 1) {
        if (DAT_0101eac0 < 0xc) {
            DAT_0101ea68.m_1c = DAT_01022310;
            DAT_0101ea68.m_20 = DAT_0101fc94;
            FUN_00413a90((BlitRequest *)&DAT_0101ea68);
        }
        DAT_0101eac0 = (DAT_0101eac0 + 1) % 0x12;
    }
    if (DAT_0101ea08 == 1) {
        x = DAT_0101fda0;
        y = DAT_01021800;
        DAT_0101fcf0.m_1c = x;
        DAT_0101fc98.m_1c = x + 6;
        DAT_0101fcf0.m_20 = y;
        DAT_0101fc98.m_20 = y + 6;
        FUN_00413a90((BlitRequest *)&DAT_0101fcf0);
        FUN_00413a90((BlitRequest *)&DAT_0101fc98);
    }
    if (DAT_0101eacc == 1) {
        scale = (DAT_0101fdc8 + 1) * 0x42 / 0x100 * 3;
        DAT_0101f308[0].m_18 = scale;
        DAT_0101f308[0].m_10 = 0xc6 - scale;
        DAT_0101f308[0].m_20 = DAT_0101f308[0].m_10 + 0x22;
        DAT_0101f308[1].m_10 = (0x42 - (DAT_0101fdcc + 1) * 0x42 / 0x100) * 3;
        DAT_0101f308[1].m_20 = DAT_0101f308[1].m_10 + 0x22;
        FUN_00413a90((BlitRequest *)&DAT_0101f308[0]);
        FUN_00413a90((BlitRequest *)&DAT_0101f308[1]);
    }
}

// OPEN: 0x41d0d0 - same 259 instructions; only the head of the second loop differs (original tests steps, then computes span, ebp counter = steps; slot order steps>center). 788 bytes incl. padding vs 737. Class: register/schedule.
int FUN_0041d0d0(int key, int *done)
{
    int row;
    int col;
    int alive;
    int center;
    int steps;
    int first;
    int last;
    int n;
    int cell;
    int span;

    if (DAT_011215a8 == 0)
        return 0;
    FUN_0041c790();
    DAT_0101fdb0++;
    if (DAT_0101fdb0 > 0) {
        DAT_0101fdb0 = 0;
        DAT_0101f420--;
    }
    if (DAT_0101f420 < 0) {
        FUN_0041d950(2);
        alive = 0;
        *done = 1;
        DAT_0101f420 = 0;
    } else {
        *done = 0;
        alive = 1;
        steps = 0;
        n = DAT_004ec81c[DAT_0101e5e0];
        while (steps < n) {
            if ((steps + 1) * 0x100 / n > 0xff - DAT_0101f420)
                break;
            steps++;
        }
        switch (DAT_0101e5e0) {
        case 0:
        case 1:
        case 2:
        case 7:
        case 8:
            first = DAT_0101e5e0 * 4;
            center = DAT_0101f454 + first;
            last = DAT_0101e5e0 * 4 + 3;
            break;
        case 3:
        case 5:
        case 9:
            first = DAT_0101e5e0 * 4;
            center = DAT_0101f454 + first;
            last = DAT_0101e5e0 * 4 + 7;
            if (DAT_0101e5e0 == 9)
                last += 4;
            break;
        case 4:
        case 6:
        case 10:
            first = DAT_0101e5e0 * 4 - 4;
            center = DAT_0101f454 + first + 4;
            last = DAT_0101e5e0 * 4 + 3;
            if (DAT_0101e5e0 == 10)
                last += 4;
            break;
        case 11:
            first = 0x24;
            center = DAT_0101f454 + 0x2c;
            last = 0x2f;
            break;
        }
        if (last > 0x2f)
            last = 0x2f;
        for (cell = first; cell <= last; cell++) {
            if (cell / 4 == 0xb && cell % 4 == 3)
                FUN_0041ad10(1, 0xc, 0, 0, 0, 3);
            else
                FUN_0041ad10(1, cell / 4, cell % 4, 0, 0, 3);
        }
        span = center - first;
        while (steps--) {
            int col2;
            int row2;
            if (last - center < span) {
                col2 = first / 4;
                row2 = first % 4;
                first++;
                span--;
            } else {
                col2 = last / 4;
                row2 = last % 4;
                last--;
            }
            if (col2 == 0xb && row2 == 3) {
                col2 = 0xc;
                row2 = 0;
            }
            FUN_0041ad10(0, col2, row2, 0, 0, 3);
        }
        if ((key >= 0x30 && key <= 0x39) || (key >= 0x41 && key <= 0x5a)) {
            FUN_0041ad10(1, -1, -1, key, 0, 0);
            if (FUN_0041adc0(key, 0, &col, &row) != 0) {
                if (col == DAT_0101e5e0 && row == DAT_0101f454) {
                    FUN_0041d950(1);
                    alive = 0;
                } else {
                    FUN_0041d950(2);
                }
            }
        }
    }
    FUN_0041d080(DAT_004ec7bc[0].x, DAT_004ec7bc[0].y + 0x3c, DAT_0101f420);
    FUN_0041cf70(0);
    if (alive == 0 && *done == 0)
        DAT_0101f420 = -1;
    return alive;
}

// OPEN: 0x419e80 - 2442 vs 2426 bytes (init section matches except: loop-base choice in the 8-case switch, zero-register pattern for DAT_0101e55c/01121590 stores, free(buf) hoisted before the m_34 test). Needs game_prelude_00/A to declare it int (declared void there; worked around by the STUB define above).

int FUN_00419e80()
{
    char *buf;
    char *packed;
    FILE *file;
    char path[2048];
    LOGFONTA font;
    unsigned int pos;
    unsigned int limit;
    unsigned int size;
    int i;
    int j;
    unsigned int k;
    unsigned int u;
    int x;
    int y;

    if (DAT_004ec3c8 != 0) {
        buf = (char *)malloc(0xf7f0f);
        if (buf != 0) {
        packed = buf + 1000000;
        FUN_0041a820(DAT_0101eb04);
        strcpy(path, DAT_0101eb04);
        strcat(path, DAT_004ecf64);
        file = fopen(path, DAT_004eba40);
        if (file != 0) {
            if (fread(packed, 1, 0x3ccf, file) == 0x3ccf) {
                fclose(file);
                DAT_0101fdd0 = &DAT_0102d320;
                pos = (unsigned int)DAT_0102d358;
                for (i = 0; i < 13; i++) {
                    size = 0xf4204;
                    if (FUN_00411e80(0, packed, 0x3ccf, buf, &size, i) == 0)
                        goto fail;
                    limit = (unsigned int)((int *)DAT_0101fdd0 + 250000);
                    ((int *)DAT_0101fdd0)[i] = FUN_004129a0(buf, pos, &limit);
                    if (((int *)DAT_0101fdd0)[i] == 0)
                        goto fail;
                    pos = limit;
                }
                if (FUN_004139f0(&DAT_0101f460) != 0) {
                    for (i = 0; i < 256; i++) {
                        if (i == 0x3d) {
                            ((PaletteTable *)buf)->entry[i].r = 0x55;
                            ((PaletteTable *)buf)->entry[i].g = 0xff;
                            ((PaletteTable *)buf)->entry[i].b = 0x71;
                            ((PaletteTable *)buf)->entry[i].flags = 0;
                            ((PaletteTable *)buf)->used[i] = 1;
                        } else if (i == 0) {
                            ((PaletteTable *)buf)->entry[i].r = 0;
                            ((PaletteTable *)buf)->entry[i].g = 0;
                            ((PaletteTable *)buf)->entry[i].b = 0;
                            ((PaletteTable *)buf)->entry[i].flags = 0;
                            ((PaletteTable *)buf)->used[i] = 1;
                        } else {
                            ((PaletteTable *)buf)->entry[i].r = 0;
                            ((PaletteTable *)buf)->entry[i].g = 0;
                            ((PaletteTable *)buf)->entry[i].b = 0;
                            ((PaletteTable *)buf)->entry[i].flags = 0;
                            ((PaletteTable *)buf)->used[i] = 0;
                        }
                    }
                    if (FUN_00413a20((PaletteTable *)buf) != 0 && FUN_00413a50() != 0) {
                        font.lfHeight = 12;
                        font.lfWidth = 0;
                        font.lfEscapement = 0;
                        font.lfOrientation = 0;
                        font.lfWeight = 700;
                        font.lfItalic = 0;
                        font.lfUnderline = 0;
                        font.lfStrikeOut = 0;
                        font.lfCharSet = 1;
                        font.lfOutPrecision = 0;
                        font.lfClipPrecision = 0;
                        font.lfQuality = 0;
                        font.lfPitchAndFamily = 0;
                        strcpy(font.lfFaceName, DAT_004ebaf0);
                        limit = (unsigned int)((int *)DAT_0101fdd0 + 250000);
                        DAT_0101fdd0->m_34 = FUN_00413430(0x280, 0, (tagLOGFONTA *)&font, 0x71ff55, 0, 1, 1, (void *)pos, (unsigned long *)&limit);
                        if (DAT_0101fdd0->m_34 == 0) {
                            free(buf);
                            return 0;
                        }
                        free(buf);
                        goto init;
                    }
                }
            } else {
                fclose(file);
            }
        }
fail:
        free(buf);
        }
        return 0;
    }
init:
    DAT_004ec3c8 = 0;
    DAT_0101e4f8.m_00 = 2;
    DAT_0101e4f8.m_04 = 0;
    DAT_0101e4f8.m_08 = 0;
    DAT_0101e4f8.m_1c = 0;
    DAT_0101e4f8.m_20 = 0;
    DAT_0101e4f8.m_50 = ((int *)DAT_0101fdd0)[0];
    DAT_0101f3b8.m_00 = 0x402;
    DAT_0101f3b8.m_04 = 0;
    DAT_0101f3b8.m_08 = 0;
    DAT_0101f3b8.m_1c = 0x55;
    DAT_0101f3b8.m_20 = 0x14d;
    DAT_0101f3b8.m_3c = 0;
    DAT_0101f3b8.m_38 = 0;
    DAT_0101f3b8.m_50 = ((int *)DAT_0101fdd0)[1];
    DAT_0101fd48.m_00 = 0x422;
    DAT_0101fd48.m_04 = 0;
    DAT_0101fd48.m_08 = 0;
    DAT_0101fd48.m_24 = 0xcb;
    DAT_0101fd48.m_28 = 0x4d;
    DAT_0101fd48.m_1c = 0x55;
    DAT_0101fd48.m_20 = 0xb0;
    DAT_0101fd48.m_3c = 0;
    DAT_0101fd48.m_38 = 0;
    DAT_0101fd48.m_50 = ((int *)DAT_0101fdd0)[1];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 13; j++) {
            if (FUN_0041ae90(j, i, &x, &y) != 0) {
                DAT_01020620[i][j].m_00 = 0x402;
                DAT_01020620[i][j].m_04 = 0;
                DAT_01020620[i][j].m_08 = 0;
                DAT_01020620[i][j].m_1c = x;
                DAT_01020620[i][j].m_20 = y;
                DAT_01020620[i][j].m_3c = 0;
                DAT_01020620[i][j].m_38 = 0;
                DAT_01020620[i][j].m_50 = ((int *)DAT_0101fdd0)[2];
            }
        }
    }
    DAT_0101ea68.m_00 = 0x22;
    DAT_0101ea68.m_04 = 0;
    DAT_0101ea68.m_08 = 0;
    DAT_0101ea68.m_24 = 10;
    DAT_0101ea68.m_28 = 10;
    DAT_0101ea68.m_50 = ((int *)DAT_0101fdd0)[7];
    DAT_0101e578.m_00 = 0x22;
    DAT_0101e578.m_04 = 0;
    DAT_0101e578.m_08 = 0;
    DAT_0101e578.m_24 = 0x84;
    DAT_0101e578.m_28 = 0x18;
    DAT_0101e578.m_50 = ((int *)DAT_0101fdd0)[8];
    for (i = 0; i < 4; i++) {
        for (k = 0; k < 8; k++) {
            switch (k) {
            case 0:
                DAT_01021808[i * 8 + 0].m_00 = 0x412;
                DAT_01021808[i * 8 + 0].m_0c = 0;
                DAT_01021808[i * 8 + 0].m_10 = 0;
                DAT_01021808[i * 8 + 0].m_14 = 6;
                DAT_01021808[i * 8 + 0].m_18 = 6;
                break;
            case 1:
                DAT_01021808[i * 8 + 1].m_00 = 0x412;
                DAT_01021808[i * 8 + 1].m_0c = 0x12;
                DAT_01021808[i * 8 + 1].m_10 = 0;
                DAT_01021808[i * 8 + 1].m_14 = 6;
                DAT_01021808[i * 8 + 1].m_18 = 6;
                break;
            case 2:
                DAT_01021808[i * 8 + 2].m_00 = 0x412;
                DAT_01021808[i * 8 + 2].m_0c = 0;
                DAT_01021808[i * 8 + 2].m_10 = 0x12;
                DAT_01021808[i * 8 + 2].m_14 = 6;
                DAT_01021808[i * 8 + 2].m_18 = 6;
                break;
            case 3:
                DAT_01021808[i * 8 + 3].m_00 = 0x412;
                DAT_01021808[i * 8 + 3].m_0c = 0x12;
                DAT_01021808[i * 8 + 3].m_10 = 0x12;
                DAT_01021808[i * 8 + 3].m_14 = 6;
                DAT_01021808[i * 8 + 3].m_18 = 6;
                break;
            case 4:
                DAT_01021808[i * 8 + 4].m_00 = 0x432;
                DAT_01021808[i * 8 + 4].m_0c = 6;
                DAT_01021808[i * 8 + 4].m_10 = 0;
                DAT_01021808[i * 8 + 4].m_14 = 0xc;
                DAT_01021808[i * 8 + 4].m_18 = 6;
                DAT_01021808[i * 8 + 4].m_24 = 0x78;
                DAT_01021808[i * 8 + 4].m_28 = 6;
                break;
            case 5:
                DAT_01021808[i * 8 + 5].m_00 = 0x432;
                DAT_01021808[i * 8 + 5].m_0c = 6;
                DAT_01021808[i * 8 + 5].m_10 = 0x12;
                DAT_01021808[i * 8 + 5].m_14 = 0xc;
                DAT_01021808[i * 8 + 5].m_18 = 6;
                DAT_01021808[i * 8 + 5].m_24 = 0x78;
                DAT_01021808[i * 8 + 5].m_28 = 6;
                break;
            case 6:
                DAT_01021808[i * 8 + 6].m_00 = 0x412;
                DAT_01021808[i * 8 + 6].m_0c = 0;
                DAT_01021808[i * 8 + 6].m_10 = 6;
                DAT_01021808[i * 8 + 6].m_14 = 6;
                DAT_01021808[i * 8 + 6].m_18 = 0xc;
                break;
            case 7:
                DAT_01021808[i * 8 + 7].m_00 = 0x412;
                DAT_01021808[i * 8 + 7].m_0c = 0x12;
                DAT_01021808[i * 8 + 7].m_10 = 6;
                DAT_01021808[i * 8 + 7].m_14 = 6;
                DAT_01021808[i * 8 + 7].m_18 = 0xc;
                break;
            }
            DAT_01021808[i * 8 + k].m_04 = 0;
            DAT_01021808[i * 8 + k].m_08 = 0;
            DAT_01021808[i * 8 + k].m_3c = 0;
            DAT_01021808[i * 8 + k].m_38 = 0;
            DAT_01021808[i * 8 + k].m_50 = ((int *)DAT_0101fdd0)[9];
        }
    }
    DAT_0101ea10.m_00 = 0x402;
    DAT_0101ea10.m_04 = 0;
    DAT_0101ea10.m_08 = 0;
    DAT_0101ea10.m_3c = 0;
    DAT_0101ea10.m_38 = 0;
    DAT_0101ea10.m_50 = ((int *)DAT_0101fdd0)[10];
    for (i = 0; i < 12; i++) {
        DAT_0101e5e8[i].m_00 = 0x402;
        DAT_0101e5e8[i].m_04 = 0;
        DAT_0101e5e8[i].m_08 = 0;
        DAT_0101e5e8[i].m_3c = 0;
        DAT_0101e5e8[i].m_38 = 0;
        DAT_0101e5e8[i].m_50 = ((int *)DAT_0101fdd0)[11];
    }
    for (i = 0; i < 0x200; i++) {
        DAT_01022320[i].m_00 = 0x412;
        DAT_01022320[i].m_3c = 0;
        DAT_01022320[i].m_38 = 0;
        DAT_01022320[i].m_50 = (int)DAT_0101fdd0->m_34->image;
        DAT_01022320[i].m_04 = 0;
        DAT_01022320[i].m_08 = 0;
    }
    for (i = 0; i < 12; i++) {
        for (j = 0; j < 2; j++) {
            DAT_0101fdd8[i][j].m_00 = 0x412;
            DAT_0101fdd8[i][j].m_3c = 0;
            DAT_0101fdd8[i][j].m_38 = 0;
            DAT_0101fdd8[i][j].m_50 = (int)DAT_0101fdd0->m_34->image;
            DAT_0101fdd8[i][j].m_04 = 0;
            DAT_0101fdd8[i][j].m_08 = 0;
        }
    }
    for (u = 0; u < 2; i++) {
        (&DAT_0101fc98)[u].m_00 = 0x22;
        (&DAT_0101fc98)[u].m_04 = 0;
        (&DAT_0101fc98)[u].m_08 = 0;
        if (u == 0) {
            (&DAT_0101fc98)[u].m_24 = 0xff;
            (&DAT_0101fc98)[u].m_28 = 0xc;
        } else {
            (&DAT_0101fc98)[u].m_24 = 0x10b;
            (&DAT_0101fc98)[u].m_28 = 0x18;
        }
        (&DAT_0101fc98)[u].m_50 = ((int *)DAT_0101fdd0)[7 + u];
    }
    for (i = 0; i < 2; i++) {
        DAT_0101f308[i].m_00 = 0x412;
        DAT_0101f308[i].m_04 = 0;
        DAT_0101f308[i].m_08 = 0;
        DAT_0101f308[i].m_3c = 0;
        DAT_0101f308[i].m_38 = 0;
        DAT_0101f308[i].m_1c = 0x240;
        DAT_0101f308[i].m_0c = 0;
        DAT_0101f308[i].m_14 = 0x18;
        DAT_0101f308[i].m_50 = ((int *)DAT_0101fdd0)[12];
    }
    DAT_0101f308[1].m_18 = 3;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 13; j++) {
            DAT_0101e350[i][j].kind = 0;
            DAT_0101e350[i][j].on = 0;
        }
    }
    DAT_0101e55c[0] = 0;
    DAT_01121590[0] = 0;
    DAT_0101e55c[1] = 0;
    DAT_01121590[1] = 0;
    DAT_0101e55c[2] = 0;
    DAT_01121590[2] = 0;
    DAT_0101e55c[3] = 0;
    DAT_0101f410[0] = 0;
    DAT_0101f410[1] = 0;
    DAT_0101f410[2] = 0;
    DAT_0101f410[3] = 0;
    memset(DAT_0101fc60, 0, sizeof(DAT_0101fc60));
    memset(DAT_0101ead4, 0, sizeof(DAT_0101ead4));
    memset(DAT_0101f424, 0, sizeof(DAT_0101f424));
    DAT_01022308 = 0xff;
    DAT_0101e5e4 = 0xff;
    DAT_0101e574 = 0;
    DAT_0101e558 = 0;
    DAT_0102231c = 0;
    DAT_01022310 = 0;
    DAT_0101fc94 = 0;
    DAT_0101eac0 = 0;
    DAT_01121590[3] = 0;
    DAT_0101fdac = 0;
    DAT_0101eac8 = 0;
    DAT_0101eac4 = 0;
    DAT_0101fda4 = 0;
    DAT_0101e5dc = 0;
    DAT_0101fc90 = 0;
    DAT_0101e56c = 0;
    DAT_0101ea08 = 0;
    DAT_0101fda0 = 0;
    DAT_01021800 = 0;
    DAT_01020618 = 0;
    DAT_0102061c = 0;
    DAT_0101fdb8 = 0;
    memset(DAT_01121560, 0, sizeof(DAT_01121560));
    DAT_011215a8 = 0;
    DAT_0101f458 = 0;
    DAT_0101eacc = 0;
    DAT_0101fdc4 = 0;
    DAT_0101fdbc = 0x32;
    DAT_0101fdc0 = 400;
    DAT_0101e554 = 0;
    DAT_0101fdb4 = 0;
    DAT_0101e5d0[2] = 0;
    DAT_0101e5d0[1] = 0;
    DAT_0101e5d0[0] = 0;
    DAT_0102230c = 0;
    DAT_01022314 = 0;
    DAT_0101e550 = 0;
    DAT_01022318 = 1;
    DAT_0101ead0 = 0;
    DAT_0101e4f0 = 0;
    DAT_011215a4 = 0;
    for (i = 0; i < 14; i++) {
        DAT_0101e270[i].len = strlen(DAT_004ec850[i]) >> 1;
        DAT_0101e270[i].state = 0;
        DAT_0101e270[i].m_04 = 0;
        DAT_0101e270[i].m_00 = 0;
    }
    DAT_0101e570 = 0;
    DAT_011215a0 = 0;
    DAT_0101fdcc = 0;
    DAT_0101fdc8 = 0;
    DAT_0101f304 = 0;
    return 1;
}

// OPEN: 0x41aef0 - 6040 vs 6102 bytes. Same switch/jump-table shape; original keeps constant 1 in edi and never hoists 0 (3 pushes), mine hoists 0 in several arms and keeps 1 in ebx. FUN_0041d940 declared int locally (callers use eax without movsx).

int FUN_0041aef0(int key)
{
    int action;
    int done;
    
    switch (DAT_0101ead0) {
    case 0:
        FUN_0041d8b0(0);
        FUN_0041d3f0(1);
        FUN_0041d740(1);
        FUN_0041d460();
        FUN_0041d7f0();
        DAT_0101ead0++;
        return 1;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        FUN_0041d460();
        FUN_0041d7f0();
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041d770(0);
        FUN_0041d8b0(DAT_0101ead0);
        if (DAT_0101ead0 + 1 == 3) {
            DAT_0101ead0 = 4;
            DAT_011215a4 = 0;
            return 1;
        }
        DAT_0101ead0++;
        DAT_011215a4 = 0;
        return 1;
    case 7:
        FUN_0041d3f0(0);
        FUN_0041c790();
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            if (DAT_011215a4 == 0) {
                FUN_0041d770(0);
                FUN_0041d8b0(DAT_0101ead0);
                DAT_011215a4++;
                return 1;
            }
        } else if (DAT_011215a4 == 0) {
            FUN_0041d7f0();
            return 1;
        }
        if (DAT_011215a4 > 3000) {
            FUN_0041d770(0);
            FUN_0041d8b0(DAT_0101ead0 + 1);
            DAT_011215a4 = 1;
        }
        DAT_011215a4++;
        if (FUN_0041d940() == 0)
            FUN_0041d770(0);
        else
            FUN_0041d7f0();
        FUN_0041c7f0(1);
        action = FUN_0041c8a0(key);
        switch (action) {
        case 0:
            FUN_0041c7f0(0);
            FUN_0041d770(0);
            FUN_0041d920();
            DAT_0101ead0 = 0xa;
            DAT_011215a4 = 0;
            return 1;
        case 1:
            FUN_0041c7f0(0);
            FUN_0041d770(0);
            FUN_0041d920();
            DAT_0101ead0 = 0xb;
            DAT_011215a4 = 0;
            return 1;
        case 2:
            FUN_0041c7f0(0);
            FUN_0041d770(0);
            FUN_0041d920();
            DAT_0101ead0 = 0xc;
            DAT_011215a4 = 0;
            return 1;
        case 3:
            FUN_0041c7f0(0);
            FUN_0041d740(0);
            FUN_0041d920();
            FUN_0041d770(0);
            FUN_0041c7b0(1);
            FUN_0041c7d0(0x32, 0xd4);
            DAT_0101ead0 = 0x2e;
            DAT_011215a4 = 0;
            return 1;
        }
        break;
    case 0xa:
        FUN_0041d8b0(0xa);
        FUN_0041c7f0(0);
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        DAT_0101ead0 = 0xd;
        return 1;
    case 0xb:
        FUN_0041d8b0(0xb);
        FUN_0041c7f0(0);
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        DAT_0101ead0 = 0x1a;
        return 1;
    case 0xc:
        FUN_0041d8b0(0xc);
        FUN_0041c7f0(0);
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        DAT_0101ead0 = 0x24;
        return 1;
    case 0xd:
    case 0xe:
    case 0xf:
    case 0x10:
    case 0x11:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            FUN_0041d770(0);
            FUN_0041d8b0(DAT_0101ead0);
            DAT_0101ead0++;
            return 1;
        }
        break;
    case 0x12:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            FUN_0041d770(0);
            FUN_0041d8b0(DAT_0101ead0);
            FUN_0041cd70(1);
            FUN_0041ce30(0, 1, 3, 2);
            FUN_0041ce30(1, 1, 6, 2);
            FUN_0041cf70(0);
            FUN_0041cf70(1);
            DAT_0101ead0++;
            return 1;
        }
        break;
    case 0x13:
    case 0x14:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        FUN_0041cd70(1);
        FUN_0041ce30(0, 1, 3, 2);
        FUN_0041ce30(1, 1, 6, 2);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            FUN_0041d770(0);
            FUN_0041d8b0(DAT_0101ead0);
            DAT_0101ead0++;
            DAT_011215a4 = 0;
            return 1;
        }
        break;
    case 0x15:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        FUN_0041cd70(1);
        FUN_0041ce30(0, 1, 3, 2);
        FUN_0041ce30(1, 1, 6, 2);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041c780(1);
        FUN_0041cda0(1);
        FUN_0041d770(0);
        FUN_0041d8b0(DAT_0101ead0);
        DAT_0101ead0++;
        DAT_011215a4 = 0;
        return 1;
    case 0x16:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041cd70(1);
        FUN_0041ce30(0, 1, 3, 2);
        FUN_0041ce30(1, 1, 6, 2);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041c780(1);
        if (DAT_011215a4 == 0 && FUN_0041cda0(1) == 0)
            DAT_011215a4++;
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0)
            FUN_0041d770(0);
        else
            FUN_0041d7f0();
        if (DAT_011215a4 != 0 && (DAT_0101e4f0 == 0 || key == 0xd)) {
            FUN_0041d770(0);
            FUN_0041d8b0(DAT_0101ead0);
            DAT_0101ead0++;
            return 1;
        }
        break;
    case 0x17:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        FUN_0041cd70(1);
        FUN_0041ce30(0, 1, 3, 2);
        FUN_0041ce30(1, 1, 6, 2);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041c780(1);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041cda0(0);
        FUN_0041d770(0);
        FUN_0041d8b0(DAT_0101ead0);
        DAT_0101ead0++;
        DAT_011215a4 = 0;
        return 1;
    case 0x18:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041cd70(1);
        FUN_0041ce30(0, 1, 3, 2);
        FUN_0041ce30(1, 1, 6, 2);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041c780(1);
        if (DAT_011215a4 == 0 && FUN_0041cda0(0) == 0)
            DAT_011215a4++;
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0)
            FUN_0041d770(0);
        else
            FUN_0041d7f0();
        if (DAT_011215a4 == 0)
            return 1;
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041c780(0);
        FUN_0041d770(0);
        FUN_0041d8b0(DAT_0101ead0);
        FUN_0041d7f0();
        DAT_0101ead0++;
        DAT_011215a4 = 0;
        return 1;
    case 0x19:
        FUN_0041cc30(1);
        FUN_0041cc80();
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0)
            FUN_0041d770(0);
        else
            FUN_0041d7f0();
        if (DAT_011215a4 < 3) {
            FUN_0041cd70(1);
            FUN_0041ce30(0, 1, 3, 2);
            FUN_0041ce30(1, 1, 6, 2);
            FUN_0041cf70(0);
            FUN_0041cf70(1);
            if (key == 0x46) {
                FUN_0041d950(1);
                DAT_011215a4 |= 1;
                return 1;
            }
            if (key == 0x4a) {
                FUN_0041d950(1);
                DAT_011215a4 |= 2;
                return 1;
            }
            if ((key > 0x2f && key < 0x3a) || (key > 0x40 && key < 0x5b)) {
                FUN_0041d950(2);
                return 1;
            }
        } else {
            if (DAT_011215a4 == 3) {
                FUN_0041cd70(1);
                FUN_0041ce30(0, 1, 3, 2);
                FUN_0041ce30(1, 1, 6, 2);
                FUN_0041cf70(0);
                FUN_0041cf70(1);
                FUN_0041d770(0);
                FUN_0041d8b0(DAT_0101ead0);
                DAT_011215a4++;
                return 1;
            }
            FUN_0041cd70(0);
            FUN_0041ce30(0, 0, 3, 2);
            FUN_0041ce30(1, 0, 6, 2);
            DAT_0101e4f0 = FUN_0041d940();
            if (DAT_0101e4f0 == 0 || key == 0xd) {
                FUN_0041d770(0);
                FUN_0041d920();
                if (DAT_0101e5d0[0] == 1 && DAT_0101e5d0[1] == 1 && DAT_0101e5d0[2] == 1) {
                    FUN_0041c7b0(0);
                    FUN_0041c790();
                    DAT_0101ead0 = 0x2d;
                    DAT_011215a4 = 0;
                    return 1;
                }
                DAT_0101ead0 = 7;
                DAT_011215a4 = 0;
                return 1;
            }
        }
        break;
    case 0x1a:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041d770(0);
        FUN_0041d8b0(DAT_0101ead0);
        DAT_0101ead0++;
        DAT_011215a4 = 0;
        return 1;
    case 0x1b:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            if (DAT_011215a4 == 0) {
                DAT_011215a4 = 1;
                FUN_0041d770(0);
                FUN_0041d8b0(DAT_0101ead0);
            }
        } else if (DAT_011215a4 == 0)
            return 1;
        FUN_0041ce30(0, 1, 0, 0);
        FUN_0041ce30(1, 1, 0, 1);
        FUN_0041ce30(2, 1, 0, 2);
        FUN_0041ce30(3, 1, 0, 3);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041cf70(2);
        FUN_0041cf70(3);
        FUN_0041ad10(1, 0, 0, 0, 0, 3);
        FUN_0041ad10(1, 0, 1, 0, 0, 3);
        FUN_0041ad10(1, 0, 2, 0, 0, 3);
        FUN_0041ad10(1, 0, 3, 0, 0, 3);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041c790();
        FUN_0041d770(0);
        FUN_0041d920();
        DAT_0101ead0++;
        DAT_011215a4 = 0;
        return 1;
    case 0x1c:
        if (DAT_011215a4 == 0) {
            DAT_011215a4 = 1;
            FUN_0041d8b0(0x1c);
        }
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        FUN_0041ce30(0, 1, 1, 0);
        FUN_0041ce30(1, 1, 1, 1);
        FUN_0041ce30(2, 1, 1, 2);
        FUN_0041ce30(3, 1, 1, 3);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041cf70(2);
        FUN_0041cf70(3);
        FUN_0041ad10(1, 1, 0, 0, 0, 3);
        FUN_0041ad10(1, 1, 1, 0, 0, 3);
        FUN_0041ad10(1, 1, 2, 0, 0, 3);
        FUN_0041ad10(1, 1, 3, 0, 0, 3);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041c790();
        FUN_0041d770(0);
        FUN_0041d920();
        DAT_0101ead0++;
        DAT_011215a4 = 0;
        return 1;
    case 0x1d:
        if (DAT_011215a4 == 0) {
            DAT_011215a4 = 1;
            FUN_0041d8b0(0x1d);
        }
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        FUN_0041ce30(0, 1, 2, 0);
        FUN_0041ce30(1, 1, 2, 1);
        FUN_0041ce30(2, 1, 2, 2);
        FUN_0041ce30(3, 1, 2, 3);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041cf70(2);
        FUN_0041cf70(3);
        FUN_0041ad10(1, 2, 0, 0, 0, 3);
        FUN_0041ad10(1, 2, 1, 0, 0, 3);
        FUN_0041ad10(1, 2, 2, 0, 0, 3);
        FUN_0041ad10(1, 2, 3, 0, 0, 3);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041c790();
        FUN_0041d770(0);
        FUN_0041d920();
        DAT_0101ead0++;
        DAT_011215a4 = 0;
        return 1;
    case 0x1e:
    case 0x1f:
    case 0x20:
        if (DAT_011215a4 == 0) {
            DAT_011215a4 = 1;
            FUN_0041d8b0(DAT_0101ead0);
        }
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        FUN_0041ce30(0, 1, 3, 0);
        FUN_0041ce30(1, 1, 3, 1);
        FUN_0041ce30(2, 1, 3, 2);
        FUN_0041ce30(3, 1, 3, 3);
        FUN_0041ce30(4, 1, 4, 0);
        FUN_0041ce30(5, 1, 4, 1);
        FUN_0041ce30(6, 1, 4, 2);
        FUN_0041ce30(7, 1, 4, 3);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041cf70(2);
        FUN_0041cf70(3);
        FUN_0041cf70(4);
        FUN_0041cf70(5);
        FUN_0041cf70(6);
        FUN_0041cf70(7);
        FUN_0041ad10(1, 3, 0, 0, 0, 3);
        FUN_0041ad10(1, 3, 1, 0, 0, 3);
        FUN_0041ad10(1, 3, 2, 0, 0, 3);
        FUN_0041ad10(1, 3, 3, 0, 0, 3);
        FUN_0041ad10(1, 4, 0, 0, 0, 3);
        FUN_0041ad10(1, 4, 1, 0, 0, 3);
        FUN_0041ad10(1, 4, 2, 0, 0, 3);
        FUN_0041ad10(1, 4, 3, 0, 0, 3);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            DAT_0101ead0++;
            if (DAT_0101ead0 > 0x20)
                FUN_0041c790();
            FUN_0041d770(0);
            FUN_0041d920();
            DAT_011215a4 = 0;
            return 1;
        }
        break;
    case 0x21:
        FUN_0041cc30(1);
        FUN_0041cc80();
        if (DAT_011215a4 == 0) {
            DAT_011215a4 = 1;
            FUN_0041d8b0(DAT_0101ead0);
            FUN_0041ce30(0, 0, 3, 0);
            FUN_0041ce30(1, 0, 3, 1);
            FUN_0041ce30(2, 0, 3, 2);
            FUN_0041ce30(3, 0, 3, 3);
            FUN_0041ce30(4, 0, 4, 0);
            FUN_0041ce30(5, 0, 4, 1);
            FUN_0041ce30(6, 0, 4, 2);
            FUN_0041ce30(7, 0, 4, 3);
            FUN_0041c790();
        }
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd) {
            FUN_0041d7f0();
            return 1;
        }
        FUN_0041d770(0);
        if (DAT_0101e4f0 != 0)
            FUN_0041d920();
        if (DAT_004ecb20 < 0 || DAT_004ecb24 < 0) {
            DAT_004ecb20 = rand() % 5;
            DAT_004ecb24 = rand() % 4;
        }
        FUN_0041cff0(1, DAT_004ecb20, DAT_004ecb24);
        if (FUN_0041d0d0(key, &done) == 0) {
            FUN_0041cff0(0, DAT_004ecb20, DAT_004ecb24);
            DAT_004ecb24 = -1;
            DAT_004ecb20 = -1;
            if (done == 0)
                DAT_011215a4++;
        }
        if (DAT_011215a4 < 0x15)
            return 1;
        DAT_011215a4 = 0;
        FUN_0041cff0(0, 0, 0);
        if (DAT_0101e5d0[0] == 1 && DAT_0101e5d0[1] == 1 && DAT_0101e5d0[2] == 1) {
            FUN_0041c7b0(0);
            FUN_0041c790();
            DAT_0101ead0 = 0x2d;
            return 1;
        }
        DAT_0101ead0 = 7;
        return 1;
    case 0x24:
    case 0x25:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041d770(0);
        FUN_0041d8b0(DAT_0101ead0);
        DAT_0101ead0++;
        DAT_011215a4 = 0;
        return 1;
    case 0x26:
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            if (DAT_011215a4 == 0) {
                DAT_011215a4 = 1;
                FUN_0041d770(0);
                FUN_0041d8b0(DAT_0101ead0);
            }
        } else if (DAT_011215a4 == 0)
            return 1;
        FUN_0041ce30(0, 1, 5, 0);
        FUN_0041ce30(1, 1, 5, 1);
        FUN_0041ce30(2, 1, 5, 2);
        FUN_0041ce30(3, 1, 5, 3);
        FUN_0041ce30(4, 1, 6, 0);
        FUN_0041ce30(5, 1, 6, 1);
        FUN_0041ce30(6, 1, 6, 2);
        FUN_0041ce30(7, 1, 6, 3);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041cf70(2);
        FUN_0041cf70(3);
        FUN_0041cf70(4);
        FUN_0041cf70(5);
        FUN_0041cf70(6);
        FUN_0041cf70(7);
        FUN_0041ad10(1, 5, 0, 0, 0, 3);
        FUN_0041ad10(1, 5, 1, 0, 0, 3);
        FUN_0041ad10(1, 5, 2, 0, 0, 3);
        FUN_0041ad10(1, 5, 3, 0, 0, 3);
        FUN_0041ad10(1, 6, 0, 0, 0, 3);
        FUN_0041ad10(1, 6, 1, 0, 0, 3);
        FUN_0041ad10(1, 6, 2, 0, 0, 3);
        FUN_0041ad10(1, 6, 3, 0, 0, 3);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            FUN_0041ce30(4, 0, 6, 0);
            FUN_0041ce30(5, 0, 6, 1);
            FUN_0041ce30(6, 0, 6, 2);
            FUN_0041ce30(7, 0, 6, 3);
            FUN_0041c790();
            FUN_0041d770(0);
            FUN_0041d920();
            DAT_011215a4 = 0;
            DAT_0101ead0++;
            return 1;
        }
        break;
    case 0x27:
        if (DAT_011215a4 == 0) {
            DAT_011215a4 = 1;
            FUN_0041d8b0(0x27);
        }
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        FUN_0041ce30(0, 1, 7, 0);
        FUN_0041ce30(1, 1, 7, 1);
        FUN_0041ce30(2, 1, 7, 2);
        FUN_0041ce30(3, 1, 7, 3);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041cf70(2);
        FUN_0041cf70(3);
        FUN_0041ad10(1, 7, 0, 0, 0, 3);
        FUN_0041ad10(1, 7, 1, 0, 0, 3);
        FUN_0041ad10(1, 7, 2, 0, 0, 3);
        FUN_0041ad10(1, 7, 3, 0, 0, 3);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            FUN_0041c790();
            FUN_0041d770(0);
            FUN_0041d920();
            DAT_011215a4 = 0;
            DAT_0101ead0++;
            return 1;
        }
        break;
    case 0x28:
        if (DAT_011215a4 == 0) {
            DAT_011215a4 = 1;
            FUN_0041d8b0(0x28);
        }
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        FUN_0041ce30(0, 1, 8, 0);
        FUN_0041ce30(1, 1, 8, 1);
        FUN_0041ce30(2, 1, 8, 2);
        FUN_0041ce30(3, 1, 8, 3);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041cf70(2);
        FUN_0041cf70(3);
        FUN_0041ad10(1, 8, 0, 0, 0, 3);
        FUN_0041ad10(1, 8, 1, 0, 0, 3);
        FUN_0041ad10(1, 8, 2, 0, 0, 3);
        FUN_0041ad10(1, 8, 3, 0, 0, 3);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041c790();
        FUN_0041d770(0);
        FUN_0041d920();
        DAT_0101ead0++;
        DAT_011215a4 = 0;
        return 1;
    case 0x29:
    case 0x2a:
        if (DAT_011215a4 == 0) {
            DAT_011215a4 = 1;
            FUN_0041d8b0(DAT_0101ead0);
        }
        FUN_0041cc30(1);
        FUN_0041cc80();
        FUN_0041d7f0();
        FUN_0041ce30(0, 1, 9, 0);
        FUN_0041ce30(1, 1, 9, 1);
        FUN_0041ce30(2, 1, 9, 2);
        FUN_0041ce30(3, 1, 9, 3);
        FUN_0041ce30(4, 1, 0xa, 0);
        FUN_0041ce30(5, 1, 0xa, 1);
        FUN_0041ce30(6, 1, 0xa, 2);
        FUN_0041ce30(7, 1, 0xa, 3);
        FUN_0041ce30(8, 1, 0xb, 0);
        FUN_0041ce30(9, 1, 0xb, 1);
        FUN_0041ce30(0xa, 1, 0xb, 2);
        FUN_0041ce30(0xb, 1, 0xc, 0);
        FUN_0041cf70(0);
        FUN_0041cf70(1);
        FUN_0041cf70(2);
        FUN_0041cf70(3);
        FUN_0041cf70(4);
        FUN_0041cf70(5);
        FUN_0041cf70(6);
        FUN_0041cf70(7);
        FUN_0041cf70(8);
        FUN_0041cf70(9);
        FUN_0041cf70(10);
        FUN_0041cf70(11);
        FUN_0041ad10(1, 9, 0, 0, 0, 3);
        FUN_0041ad10(1, 9, 1, 0, 0, 3);
        FUN_0041ad10(1, 9, 2, 0, 0, 3);
        FUN_0041ad10(1, 9, 3, 0, 0, 3);
        FUN_0041ad10(1, 0xa, 0, 0, 0, 3);
        FUN_0041ad10(1, 0xa, 1, 0, 0, 3);
        FUN_0041ad10(1, 0xa, 2, 0, 0, 3);
        FUN_0041ad10(1, 0xa, 3, 0, 0, 3);
        FUN_0041ad10(1, 0xb, 0, 0, 0, 3);
        FUN_0041ad10(1, 0xb, 1, 0, 0, 3);
        FUN_0041ad10(1, 0xb, 2, 0, 0, 3);
        FUN_0041ad10(1, 0xc, 0, 0, 0, 3);
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            DAT_0101ead0++;
            if (DAT_0101ead0 > 0x2a)
                FUN_0041c790();
            FUN_0041d770(0);
            FUN_0041d920();
            DAT_011215a4 = 0;
            return 1;
        }
        break;
    case 0x2b:
        FUN_0041cc30(1);
        FUN_0041cc80();
        if (DAT_011215a4 == 0) {
            DAT_011215a4 = 1;
            FUN_0041d8b0(DAT_0101ead0);
            FUN_0041ce30(0, 0, 9, 0);
            FUN_0041ce30(1, 0, 9, 1);
            FUN_0041ce30(2, 0, 9, 2);
            FUN_0041ce30(3, 0, 9, 3);
            FUN_0041ce30(4, 0, 0xa, 0);
            FUN_0041ce30(5, 0, 0xa, 1);
            FUN_0041ce30(6, 0, 0xa, 2);
            FUN_0041ce30(7, 0, 0xa, 3);
            FUN_0041ce30(8, 0, 0xb, 0);
            FUN_0041ce30(9, 0, 0xb, 1);
            FUN_0041ce30(0xa, 0, 0xb, 2);
            FUN_0041ce30(0xb, 0, 0xc, 0);
            FUN_0041c790();
        }
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd) {
            FUN_0041d7f0();
            return 1;
        }
        FUN_0041d770(0);
        if (DAT_0101e4f0 != 0)
            FUN_0041d920();
        if (DAT_004ecb20 < 0 || DAT_004ecb24 < 0) {
            DAT_004ecb20 = rand() % 5 + 5;
            if (DAT_004ecb20 < 7)
                DAT_004ecb24 = rand() % 4;
            else if (DAT_004ecb20 < 9)
                DAT_004ecb24 = rand() % 3;
            else
                DAT_004ecb24 = rand() % 2;
        }
        FUN_0041cff0(1, DAT_004ecb20, DAT_004ecb24);
        if (FUN_0041d0d0(key, &done) == 0) {
            FUN_0041cff0(0, DAT_004ecb20, DAT_004ecb24);
            DAT_004ecb24 = -1;
            DAT_004ecb20 = -1;
            if (done == 0)
                DAT_011215a4++;
        }
        if (DAT_011215a4 < 0x15)
            return 1;
        DAT_011215a4 = 0;
        FUN_0041cff0(0, 0, 0);
        if (DAT_0101e5d0[0] == 1 && DAT_0101e5d0[1] == 1 && DAT_0101e5d0[2] == 1) {
            FUN_0041c7b0(0);
            FUN_0041c790();
            DAT_0101ead0 = 0x2d;
            return 1;
        }
        DAT_0101ead0 = 7;
        return 1;
    case 0x2c:
    case 0x2d:
        FUN_0041d7f0();
        if (DAT_011215a4 == 0) {
            DAT_011215a4 = 1;
            FUN_0041d8b0(DAT_0101ead0);
        }
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 != 0 && key != 0xd)
            return 1;
        FUN_0041d920();
        if (DAT_0101ead0 + 1 < 0x2e) {
            DAT_0101ead0++;
            DAT_011215a4 = 0;
            return 1;
        }
        DAT_0101ead0 = 7;
        DAT_011215a4 = 0;
        return 1;
    case 0x2e:
    case 0x2f:
    case 0x30:
    case 0x31:
    case 0x32:
        FUN_0041d7f0();
        if (DAT_011215a4 == 0) {
            DAT_011215a4 = 1;
            FUN_0041d8b0(DAT_0101ead0);
        }
        DAT_0101e4f0 = FUN_0041d940();
        if (DAT_0101e4f0 == 0 || key == 0xd) {
            FUN_0041d920();
            DAT_011215a4 = 0;
            DAT_0101ead0++;
            if (DAT_0101ead0 > 0x32)
                return 0;
        }
    }
    return 1;
}
