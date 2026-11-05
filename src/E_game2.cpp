#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void FUN_0041d3f0(int mode)
{
    int i;

    if (DAT_0101f458 != mode) {
        for (i = 0; i < 14; i++) {
            DAT_0101e270[i].len = strlen(DAT_004ec850[i]) / 2;
            DAT_0101e270[i].state = 0;
            DAT_0101e270[i].m_04 = 0;
            DAT_0101e270[i].m_00 = 0;
        }
        DAT_0101e570 = 0;
        DAT_011215a0 = 0;
    }
    DAT_0101f458 = mode;
    FUN_0041c7b0(mode);
}

void FUN_0041d740(int on)
{
    if (DAT_0101eacc != on) {
        DAT_0101fdcc = 0;
        DAT_0101fdc8 = 0;
        DAT_0101f304 = 0;
    }
    DAT_0101eacc = on;
}

void FUN_0041d770(int level)
{
    int step;

    if (DAT_0101eacc != 0) {
        if (level < 0)
            level = 0;
        else if (level > 0xff)
            level = 0xff;
        step = DAT_0101f304 / 4;
        DAT_0101f304++;
        DAT_0101fdcc += step * step * -3;
        DAT_0101fdc8 = level;
        if (DAT_0101fdcc <= level) {
            DAT_0101fdcc = level;
            DAT_0101f304 = 0;
        }
    }
}

void FUN_0041d7f0()
{
    int roll = rand() % 10;
    int count = DAT_011215ac;

    DAT_011215ac++;
    if (count > roll) {
        DAT_011215ac = 0;
        if (rand() % 4 != 0)
            DAT_011215b4 = rand() % 128 + rand() % 128;
        else
            DAT_011215b4 = rand() % 256;
    }
    DAT_011215b0 += (DAT_011215b4 - DAT_011215b0) / (rand() % 4 + 1);
    FUN_0041d770(DAT_011215b0);
}

int FUN_0041d8b0(int n)
{
    char path[2048];
    int i;

    if (n >= 0 && n < 0x30) {
        sprintf(path, "%s%s%04d.wav", DAT_0101eb04, "TUTORIAL\\", n + 1);
        for (i = 0; i < 3; i++) {
            if (FUN_004185a0(path, 0, 1, 0, 0) != 0)
                return 1;
        }
    }
    return 0;
}

int FUN_0041d920()
{
    int i;

    for (i = 0; i < 3; i++) {
        if (FUN_004188f0(0) != 0)
            return 1;
    }
    return 0;
}

char FUN_0041d940()
{
    return FUN_004189a0(0);
}

