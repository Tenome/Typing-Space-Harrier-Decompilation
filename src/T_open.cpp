#include "game.h"

// OPEN: register allocation (x/baseH registers swapped against the original)
void FUN_00416e00(ReadbackJob *job)
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
    if (x < 0)
        x += (1 - x / baseW) * baseW;
    if (x >= baseW)
        x -= x / baseW * baseW;
        baseEnd = base + baseH * basePitch;
        for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd && srcRow < baseEnd; dstRow += bufPitch, srcRow += basePitch) {
            dstEnd = dstRow + width;
            for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
                if (src >= srcRow + baseW)
                    src = srcRow;
                *dst = *src;
            }
        }
    }
}

// OPEN: register allocation (x/baseH registers swapped against the original)
void FUN_004170a0(ReadbackJob *job)
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
    unsigned char *dstEnd;
    unsigned char *bufEnd;

    if (x < 0)
        x += (1 - x / baseW) * baseW;
    if (x >= baseW)
        x -= x / baseW * baseW;
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
        for (dst = dstRow, src = row + x; dst < dstEnd; dst++, src++) {
            if (src >= row + baseW)
                src = row;
            *dst = *src;
        }
    }
}

// OPEN: register allocation (x/baseH registers swapped against the original)
void FUN_004171b0(ReadbackJob *job)
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
    unsigned char *row;
    unsigned char *src;
    unsigned char *dst;
    unsigned char *dstEnd;
    unsigned char *bufEnd;

    if (x < 0)
        x += (1 - x / baseW) * baseW;
    if (x >= baseW)
        x -= x / baseW * baseW;
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
        for (dst = dstRow, src = row + x; dst < dstEnd; dst++, src++) {
            if (src >= row + baseW)
                src = row;
            if (keys[*src] == 0)
                *dst = *src;
        }
    }
}

// OPEN: register allocation (x/baseH registers swapped against the original)
void FUN_004172e0(ReadbackJob *job)
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
    unsigned char *dstEnd;
    unsigned char *bufEnd;

    if (y < 0)
        y += (1 - y / baseH) * baseH;
    if (y >= baseH)
        y -= y / baseH * baseH;
    if (x < 0)
        x += (1 - x / baseW) * baseW;
    if (x >= baseW)
        x -= x / baseW * baseW;
    bufEnd = buf + bufPitch * height;
    baseEnd = base + baseH * basePitch;
    for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd; dstRow += bufPitch, srcRow += basePitch) {
        if (srcRow >= baseEnd)
            srcRow = base;
        dstEnd = dstRow + width;
        for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
            if (src >= srcRow + baseW)
                src = srcRow;
            *dst = *src;
        }
    }
}

// OPEN: register allocation (x/baseH registers swapped against the original)
void FUN_004173f0(ReadbackJob *job)
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
    unsigned char *dstEnd;
    unsigned char *bufEnd;

    if (y < 0)
        y += (1 - y / baseH) * baseH;
    if (y >= baseH)
        y -= y / baseH * baseH;
    if (x < 0)
        x += (1 - x / baseW) * baseW;
    if (x >= baseW)
        x -= x / baseW * baseW;
    bufEnd = buf + bufPitch * height;
    baseEnd = base + baseH * basePitch;
    for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd; dstRow += bufPitch, srcRow += basePitch) {
        if (srcRow >= baseEnd)
            srcRow = base;
        dstEnd = dstRow + width;
        for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
            if (src >= srcRow + baseW)
                src = srcRow;
            if (keys[*src] == 0)
                *dst = *src;
        }
    }
}

