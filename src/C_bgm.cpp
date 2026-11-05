#include <stdio.h>
#include <string.h>
#include "game.h"

int Game::M_00410ca0(char *name, int size)
{
    char path[0x108];
    FILE *fp;
    int got;
    int len;

    memset(DAT_004efa10, 0, sizeof(DAT_00a3d360));
    strcpy(path, DAT_011215bc);
    strcat(path, "BGM\\");
    strcat(path, name);
    fp = fopen(path, "rb");
    if (fp == 0)
        return DAT_00430658;
    fseek(fp, 0, 2);
    len = ftell(fp);
    DAT_004efa10[0] = len;
    fseek(fp, len - 8, 0);
    got = fread(DAT_004efa10 + 1, 1, 8, fp);
    fclose(fp);
    if (DAT_004efa10[0] != size)
        return DAT_00430658;
    if (got != 8)
        return DAT_00430658;
    return strcmp((char *)(DAT_004efa10 + 1), DAT_004e2970) ? DAT_00430658 : 0;
}
