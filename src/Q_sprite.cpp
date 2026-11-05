#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

int Game::M_0040e380()
{
    char path[0x108];
    FILE *file;
    int group;
    int first;
    int count;
    int i;

    strcpy(path, DAT_011215bc);
    strcat(path, DAT_0042e11c);
    strcat(path, "SPRITE.HJT");
    DAT_004f0654 = M_00405290(path);
    if (DAT_004f0654 <= 0)
        return DAT_00430658;
    DAT_004f0658 = (char *)malloc(DAT_004f0654 + 8);
    if (DAT_004f0658 == 0)
        return DAT_00430658;
    file = fopen(path, "rb");
    if (file == 0) {
        free(DAT_004f0658);
        return DAT_00430658;
    }
    fread(DAT_004f0658, 1, DAT_004f0654, file);
    fclose(file);
    FUN_00411910();
    DAT_004efa38 = DAT_009aa29c;
    first = m_87cbf4;
    for (group = 0; group < 0x7f; group++) {
        if (DAT_0042e12c[group][0] == 0)
            break;
        count = M_0040e570(group, first);
        m_87cbfc[group] = first;
        m_87cffc[group] = count;
        for (i = 0; i < count; i++) {
            m_87d3fc[first + i] = group;
            m_87e7fc[first + i] = i;
        }
        m_87cbf0++;
        m_87cbf8 = first + count - 1;
        first += count;
        if (group >= 9)
            M_0040c2a0(group);
    }
    free(DAT_004f0658);
    FUN_00411920();
    return first;
}

int Game::M_0040e570(int group, int first)
{
    char suffix[0x10];
    char name[0x108];
    int i;
    int *total;
    unsigned int limit;
    int **slot;
    int index;
    int j;
    int *image;

    i = 0;
    limit = 0x60000;
    total = &DAT_0042e32c[group];
    if (DAT_0042e32c[group] > 0) {
        slot = &m_8eced8[first];
        do {
            if (M_0040e340(first + i) < 0)
                return DAT_00430658;
            i++;
            wsprintfA(suffix, "_%02d", i);
            strcpy(name, DAT_0042e12c[group]);
            strcat(name, suffix);
            strcat(name, DAT_0042e124);
            index = DAT_00430658;
            for (j = 0; DAT_0042e52c[j][0] != 0; j++) {
                if (strcmp(name, DAT_0042e52c[j]) == 0) {
                    index = j;
                    break;
                }
            }
            if (index == DAT_00430658)
                return DAT_00430658;
            limit = 0x60000;
            if (FUN_00411e80(0, DAT_004f0658, DAT_004f0654, DAT_004efa38, &limit, index) == 0)
                return DAT_00430658;
            m_8eced4 = m_21ef798;
            image = FUN_004129a0(DAT_004efa38, m_8eced0, &m_8eced4);
            *slot = image;
            m_8ef6d8 = image;
            if (image == 0)
                break;
            slot++;
            m_8eced0 = m_8eced4;
            m_8eced4 = m_21ef798;
        } while (i < *total);
    }
    return *total;
}
