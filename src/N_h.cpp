#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

int Game::M_00405220(const char *path)
{
    FILE *f;
    int size, got;

    f = fopen(path, "rb");
    if (f == 0)
        return DAT_00430658;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    got = fread(DAT_008f144c, 1, size, f);
    fclose(f);
    if (got != size)
        return DAT_00430658;
    return size;
}
