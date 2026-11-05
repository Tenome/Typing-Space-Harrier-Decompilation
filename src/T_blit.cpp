#include "game.h"

// Clipped / wrapped / clamped read-back copies (family of FUN_00415320): rows of the destination buffer are filled from rows of the source.

void FUN_00416200(ReadbackJob *job)
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
    unsigned char *srcEnd;
    unsigned char *bufEnd = buf + bufPitch * height;

    if (y < 0) {
        height += y;
        if (height < 0)
            return;
        buf -= y * bufPitch;
        y = 0;
    }
    if (y < baseH) {
    if (x < 0) {
        width += x;
        if (width < 0)
            return;
        buf -= x;
        x = 0;
    }
    if (x < baseW) {
        baseEnd = base + baseH * basePitch;
        for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd && srcRow < baseEnd; dstRow += bufPitch, srcRow += basePitch) {
            dstEnd = dstRow + width;
            srcEnd = srcRow + baseW;
            for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
                if (src >= srcEnd)
                    break;
                if (keys[*src] == 0)
                    *dst = *src;
            }
        }
    }
    }
}

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
    unsigned char *dstEnd;
    unsigned char *srcEnd;
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
    unsigned char *dstEnd;
    unsigned char *srcEnd;
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

void FUN_00416570(ReadbackJob *job)
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
    unsigned char *srcEnd;
    unsigned char *bufEnd = buf + bufPitch * height;

    if (y < 0)
        y += (1 - y / baseH) * baseH;
    if (y >= baseH)
        y -= y / baseH * baseH;
    if (x < 0) {
        width += x;
        if (width < 0)
            return;
        buf -= x;
        x = 0;
    }
    if (x < baseW) {
    baseEnd = base + baseH * basePitch;
    for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd; dstRow += bufPitch, srcRow += basePitch) {
        if (srcRow >= baseEnd)
            srcRow = base;
        dstEnd = dstRow + width;
        srcEnd = srcRow + baseW;
        for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
            if (src >= srcEnd)
                break;
            *dst = *src;
        }
    }
    }
}

void FUN_00416670(ReadbackJob *job)
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
    unsigned char *srcEnd;
    unsigned char *bufEnd = buf + bufPitch * height;

    if (y < 0)
        y += (1 - y / baseH) * baseH;
    if (y >= baseH)
        y -= y / baseH * baseH;
    if (x < 0) {
        width += x;
        if (width < 0)
            return;
        buf -= x;
        x = 0;
    }
    if (x < baseW) {
    baseEnd = base + baseH * basePitch;
    for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd; dstRow += bufPitch, srcRow += basePitch) {
        if (srcRow >= baseEnd)
            srcRow = base;
        dstEnd = dstRow + width;
        srcEnd = srcRow + baseW;
        for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
            if (src >= srcEnd)
                break;
            if (keys[*src] == 0)
                *dst = *src;
        }
    }
    }
}

void FUN_00416ad0(ReadbackJob *job)
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
            if (keys[*from] == 0)
                *dst = *from;
        }
    }
}

void FUN_00416be0(ReadbackJob *job)
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
    unsigned char *dstEnd;
    unsigned char *srcEnd;
    unsigned char *bufEnd;

    if (y < 0)
        y += (1 - y / baseH) * baseH;
    if (y >= baseH)
        y -= y / baseH * baseH;
    bufEnd = buf + bufPitch * height;
    baseEnd = base + baseH * basePitch;
    for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd; dstRow += bufPitch, srcRow += basePitch) {
        if (srcRow >= baseEnd)
            srcRow = base;
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

void FUN_00416ce0(ReadbackJob *job)
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
    unsigned char *dstEnd;
    unsigned char *srcEnd;
    unsigned char *bufEnd;

    if (y < 0)
        y += (1 - y / baseH) * baseH;
    if (y >= baseH)
        y -= y / baseH * baseH;
    bufEnd = buf + bufPitch * height;
    baseEnd = base + baseH * basePitch;
    for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd; dstRow += bufPitch, srcRow += basePitch) {
        if (srcRow >= baseEnd)
            srcRow = base;
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

void FUN_00416350(ReadbackJob *job)
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
    unsigned char *srcEnd;
    unsigned char *bufEnd = buf + bufPitch * height;

    if (x < 0) {
        width += x;
        if (width < 0)
            return;
        buf -= x;
        x = 0;
    }
    if (x < baseW) {
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
            if (src >= srcEnd)
                break;
            *dst = *src;
        }
    }
    }
}

void FUN_00416440(ReadbackJob *job)
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
    unsigned char *srcEnd;
    unsigned char *bufEnd = buf + bufPitch * height;

    if (x < 0) {
        width += x;
        if (width < 0)
            return;
        buf -= x;
        x = 0;
    }
    if (x < baseW) {
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
            if (src >= srcEnd)
                break;
            if (keys[*src] == 0)
                *dst = *src;
        }
    }
    }
}

void FUN_00416100(ClipJob *job)
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
    unsigned char *srcEnd;
    unsigned char *bufEnd = buf + bufPitch * height;

    if (y < 0) {
        height += y;
        if (height < 0)
            return;
        buf -= y * bufPitch;
        y = 0;
    }
    if (y < baseH) {
    if (x < 0) {
        width += x;
        if (width < 0)
            return;
        buf -= x;
        x = 0;
    }
    if (x < baseW) {
        baseEnd = base + baseH * basePitch;
        for (dstRow = buf, srcRow = base + y * basePitch; dstRow < bufEnd && srcRow < baseEnd; dstRow += bufPitch, srcRow += basePitch) {
            dstEnd = dstRow + width;
            srcEnd = srcRow + baseW;
            for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
                if (src >= srcEnd)
                    break;
                *dst = *src;
            }
        }
    }
    }
}
