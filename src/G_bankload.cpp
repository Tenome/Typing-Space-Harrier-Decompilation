#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

// NEEDS REQUEST: lane B declares M_0040c9f0 as void, it returns the entry count (int); build passes with that one change.
void Game::M_0040c780(int n)
{
    char path[0x108];
    FILE *file;
    int size;
    int got;
    int count;
    unsigned char *data;
    size_t bytes;

    DAT_00a45e34 = &DAT_00901708[n];
    DAT_00a45e34->m_00[2] = 0;
    wsprintfA(path, "%sword\\word_%02d.dat", DAT_011215bc, n);
    file = fopen(path, "rb");
    if (file != 0) {
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, 0, SEEK_SET);
        DAT_00a45e34->m_00[1] = (int)malloc(size + 8);
        if (DAT_00a45e34->m_00[1] == 0) {
            fclose(file);
            return;
        }
        got = fread((void *)DAT_00a45e34->m_00[1], 1, size, file);
        fclose(file);
        if (got != size) {
            M_0040c710(DAT_00a45e34);
            return;
        }
        data = (unsigned char *)DAT_00a45e34->m_00[1];
        if (data[0] != '\r' || data[1] != '\n')
            M_004051b0(data, got);
        DAT_00a45e34->m_00[2] = size;
        ((char *)DAT_00a45e34->m_00[1])[size] = 0x1a;
        ((char *)DAT_00a45e34->m_00[1])[size + 1] = 0;
        M_0040c9c0(DAT_00a45e34);
        count = M_0040c9f0(DAT_00a45e34, 0);
        if (count < 1) {
            M_0040c710(DAT_00a45e34);
            return;
        }
        bytes = count * 4 + 0x80;
        DAT_00a45e34->m_10 = (char **)malloc(bytes);
        if (DAT_00a45e34->m_10 == 0) {
            M_0040c710(DAT_00a45e34);
            return;
        }
        DAT_00a45e34->m_14 = (char **)malloc(bytes);
        if (DAT_00a45e34->m_14 == 0) {
            M_0040c710(DAT_00a45e34);
            return;
        }
        M_0040c9f0(DAT_00a45e34, 1);
    }
}
