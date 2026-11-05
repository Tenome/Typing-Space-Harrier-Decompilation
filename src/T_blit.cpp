#include "game.h"
void FUN_004167a0(ReadbackJob *job)
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
    unsigned char *src;
    unsigned char *dst;
    unsigned char *from;
    int i;
    unsigned char *srcEnd;
    unsigned char *dstEnd;
    unsigned char *bufEnd = buf + bufPitch * height;

    if (y < 0) {
        height += y;
        if (height < 0)
            return;
        buf -= y * bufPitch;
        y = 0;
    }
    if (y < baseH) {
        baseEnd = base + baseH * basePitch;
        for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd && srcRow < baseEnd; dstRow += bufPitch, srcRow += basePitch) {
            dstEnd = dstRow + width;
            srcEnd = srcRow + baseW;
            for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
                if (src < srcRow)
                    from = srcRow;
                else if (src >= srcEnd)
                    from = srcEnd - 1;
                else
                    from = src;
                *dst = *from;
            }
        }
    }
}

void FUN_004168b0(ReadbackJob *job)
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
    int *keys = job->keys;
    unsigned char *baseEnd;
    unsigned char *srcRow;
    unsigned char *dstRow;
    unsigned char *src;
    unsigned char *dst;
    unsigned char *from;
    unsigned char c;
    int i;
    unsigned char *srcEnd;
    unsigned char *dstEnd;
    unsigned char *bufEnd = buf + bufPitch * height;

    if (y < 0) {
        height += y;
        if (height < 0)
            return;
        buf -= y * bufPitch;
        y = 0;
    }
    if (y < baseH) {
        baseEnd = base + baseH * basePitch;
        for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd && srcRow < baseEnd; dstRow += bufPitch, srcRow += basePitch) {
            dstEnd = dstRow + width;
            srcEnd = srcRow + baseW;
            for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
                if (src < srcRow)
                    from = srcRow;
                else if (src >= srcEnd)
                    from = srcEnd - 1;
                else
                    from = src;
                if (keys[*from] == 0)
                    *dst = *from;
            }
        }
    }
}
