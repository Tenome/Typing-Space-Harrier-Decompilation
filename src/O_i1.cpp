#include "game.h"

void FUN_00416350(ReadbackJob *job)
{
    int baseH = job->baseH;
    int baseW = job->baseW;
    unsigned char *base = job->base;
    int bufPitch = job->bufPitch;
    int x = job->x;
    int w = job->width;
    int basePitch = job->basePitch;
    unsigned char *buf = job->buf;
    int y = job->y;
    unsigned char *bufEnd = buf + job->height * bufPitch;
    int srcX = x;
    unsigned char *baseEnd;
    unsigned char *srcRow;
    unsigned char *row;
    unsigned char *srcEnd;
    unsigned char *src;
    unsigned char *dst;
    unsigned char *dstEnd;

    if (x < 0) {
        w += x;
        if (w < 0)
            return;
        srcX = 0;
        buf -= x;
    }
    if (srcX < baseW) {
        baseEnd = base + baseH * basePitch;
        srcRow = base + y * basePitch;
        for (; buf < bufEnd; buf += bufPitch) {
            if (srcRow < base)
                row = base;
            else {
                row = srcRow;
                if (srcRow >= baseEnd)
                    row = baseEnd - basePitch;
            }
            dstEnd = buf + w;
            srcEnd = row + baseW;
            for (dst = buf, src = row + srcX; dst < dstEnd; dst++, src++) {
                if (src >= srcEnd)
                    break;
                *dst = *src;
            }
            srcRow += basePitch;
        }
    }
}
