#include "game.h"

// OPEN: 0x416f30 clip y, wrap x, keyed read-back copy (see build notes in docs/reports/U.md).
void FUN_00416f30(ReadbackJob *job)
{
    unsigned char *base = job->base;
    int baseW = job->baseW;
    int basePitch = job->basePitch;
    unsigned char *buf = job->buf;
    int x = job->x;
    int baseH = job->baseH;
    int width = job->width;
    int height = job->height;
    int bufPitch = job->bufPitch;
    int y = job->y;
    int *keys = job->keys;
    unsigned char *bufEnd = buf + height * bufPitch;
    unsigned char *baseEnd;
    unsigned char *srcRow;
    unsigned char *dstRow;
    unsigned char *src;
    unsigned char *dst;
    unsigned char *dstEnd;
    unsigned char *srcEnd;
    int run;
    unsigned char c;

    if (y < 0) {
        height += y;
        if (height < 0)
            return;
        buf -= y * bufPitch;
        y = 0;
    }
    if (y < baseH) {
        if (x < 0)
            x += (1 - x / baseW) * baseW;
        if (x >= baseW)
            x -= x / baseW * baseW;
        srcRow = base + y * basePitch;
        baseEnd = base + baseH * basePitch;
        run = baseW - x;
        for (dstRow = buf; dstRow < bufEnd && srcRow < baseEnd; dstRow += bufPitch) {
            dstEnd = dstRow + width;
            src = srcRow + x;
            srcEnd = src + run;
            for (dst = dstRow; dst < dstEnd; dst++, src++) {
                if (src >= srcEnd)
                    src = srcRow;
                c = *src;
                if (keys[c] == 0)
                    *dst = c;
            }
            srcRow += basePitch;
        }
    }
}
