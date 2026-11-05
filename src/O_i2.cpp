#include "game.h"

void FUN_00416440(ReadbackJob *job)
{
    unsigned char *base = job->base;
    int baseW = job->baseW;
    int basePitch = job->basePitch;
    int bufPitch = job->bufPitch;
    int x = job->x;
    int *keys = job->keys;
    int baseH = job->baseH;
    unsigned char *buf = job->buf;
    int w = job->width;
    int y = job->y;
    unsigned char *bufEnd = buf + job->height * bufPitch;
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
        buf -= x;
        x = 0;
    }
    if (x < baseW) {
        baseEnd = base + baseH * basePitch;
        srcRow = base + y * basePitch;
        for (; buf < bufEnd; buf += bufPitch) {
            row = base;
            if (srcRow >= base) {
                if (srcRow < baseEnd)
                    row = srcRow;
                else
                    row = baseEnd - basePitch;
            }
            dstEnd = buf + w;
            srcEnd = row + baseW;
            for (dst = buf, src = row + x; dst < dstEnd; dst++, src++) {
                if (src >= srcEnd)
                    break;
                if (keys[*src] == 0)
                    *dst = *src;
            }
            srcRow += basePitch;
        }
    }
}
