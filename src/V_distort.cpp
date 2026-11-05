#include "game.h"
void FUN_00417530(DistortJob *job)
{
    int baseW = job->baseW;
    unsigned char *buf = job->buf;
    int baseH = job->baseH;
    int x = job->x;
    unsigned char *base = job->base;
    int basePitch = job->basePitch;
    int bufPitch = job->bufPitch;
    int rows = job->rows;
    DistortRow *row = job->table;
    int width = job->width;
    int y = job->y;
    int wrapW = baseW << 16;
    int widthFixed = width << 16;
    DistortRow *end;
    int step;
    unsigned char *dstRow;
    unsigned char *dst;
    unsigned char *dstEnd;
    int sy;
    int fx;
    int sx;
    unsigned char *srcRow;

    if (y < 0)
        y += (1 - y / baseH) * baseH;
    if (y >= baseH)
        y -= y / baseH * baseH;
    if (x < 0)
        x += (1 - x / baseW) * baseW;
    if (x >= baseW)
        x -= x / baseW * baseW;
    end = row + rows;
    for (dstRow = buf; row < end; row++, dstRow += bufPitch) {
        if (row->lineShift == -1 || row->divisor <= 0)
            continue;
        step = widthFixed / row->divisor;
        sy = row->lineShift + y;
        sx = x - (row->rowShift * step >> 16);
        while (sy < 0)
            sy += baseH;
        while (sy >= baseH)
            sy -= baseH;
        while (sx < 0)
            sx += baseW;
        while (sx >= baseW)
            sx -= baseW;
        srcRow = base + sy * basePitch;
        fx = sx << 16;
        dstEnd = dstRow + width;
        for (dst = dstRow; dst < dstEnd; dst++) {
            while (fx >= wrapW)
                fx -= wrapW;
            *dst = srcRow[fx >> 16];
            fx += step;
        }
    }
}
