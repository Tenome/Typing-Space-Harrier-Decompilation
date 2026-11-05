#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "game.h"

void FUN_004169f0(ReadbackJob *job)
{
    unsigned char *base = job->base;
    int baseW = job->baseW;
    int baseH = job->baseH;
    int basePitch = job->basePitch;
    unsigned char *buf = job->buf;
    int width = job->width;
    int height = job->height;
    int bufPitch = job->bufPitch;
    int x = job->x;
    int y = job->y;
    unsigned char *baseEnd;
    unsigned char *srcRow;
    unsigned char *dstRow;
    unsigned char *row;
    unsigned char *src;
    unsigned char *dst;
    unsigned char *from;
    unsigned char *dstEnd;
    unsigned char *srcEnd;
    unsigned char *bufEnd;

    bufEnd = buf + bufPitch * height;
    baseEnd = base + baseH * basePitch;
    for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd; dstRow += bufPitch, srcRow += basePitch) {
        if (srcRow < base)
            row = base;
        else if (srcRow >= baseEnd)
            row = baseEnd - basePitch;
        else
            row = srcRow;
        dstEnd = dstRow + width;
        srcEnd = row + baseW;
        for (dst = dstRow, src = row + x; dst < dstEnd; dst++, src++) {
            if (src < row)
                from = row;
            else if (src >= srcEnd)
                from = srcEnd - 1;
            else
                from = src;
            *dst = *from;
        }
    }
}
