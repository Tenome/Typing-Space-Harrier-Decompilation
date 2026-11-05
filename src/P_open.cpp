#include "game.h"

// Clipped / wrapped / edge-clamped read-back copies of FUN_00415320 (family of 0x416200-0x417a20).
// Common wall for every function here: the flow and the dst = src - row - x + buf start match, but VC6 gives
// one more live value in the inner loop (srcRow + baseW is spilled instead of recomputed from baseW/srcRow)
// and different callee-saved registers (register/schedule class).

// OPEN: 340 vs 326 bytes: clip y, clip x, keyed copy; same flow, frame 0x20 vs 0x24, registers/spills differ.
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
    unsigned char *bufEnd = buf + bufPitch * height;
    unsigned char *baseEnd;
    unsigned char *srcRow;
    unsigned char *dstRow;
    unsigned char *src;
    unsigned char *dst;
    unsigned char *srcEnd;
    unsigned char c;

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
            srcRow = base + y * basePitch;
            baseEnd = base + baseH * basePitch;
            for (dstRow = buf; dstRow < bufEnd && srcRow < baseEnd; dstRow += bufPitch, srcRow += basePitch) {
                srcEnd = srcRow + baseW;
                for (src = srcRow + x, dst = dstRow; dst < dstRow + width; src++, dst++) {
                    if (src >= srcEnd)
                        break;
                    c = *src;
                    if (keys[c] == 0)
                        *dst = c;
                }
            }
        }
    }
}

// OPEN: 255 vs 247 bytes: wrap y, clip x, plain copy; one extra spill slot (srcRow + baseW temp), edi/eax roles swapped.
void FUN_00416570(ReadbackJob *job)
{
    unsigned char *base = job->base;
    int baseW = job->baseW;
    int baseH = job->baseH;
    int basePitch = job->basePitch;
    unsigned char *buf = job->buf;
    int width = job->width;
    int bufPitch = job->bufPitch;
    int x = job->x;
    int y = job->y;
    unsigned char *bufEnd = buf + job->height * bufPitch;
    unsigned char *baseEnd;
    unsigned char *srcRow;
    unsigned char *dstRow;
    unsigned char *src;
    unsigned char *dst;
    unsigned char *dstEnd;

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
        srcRow = base + y * basePitch;
        baseEnd = base + baseH * basePitch;
        for (dstRow = buf; dstRow < bufEnd; dstRow += bufPitch) {
            if (srcRow >= baseEnd)
                srcRow = base;
            dstEnd = dstRow + width;
            for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
                if (src >= srcRow + baseW)
                    break;
                *dst = *src;
            }
            srcRow += basePitch;
        }
    }
}

// OPEN: 317 vs 296 bytes: keyed variant of 416570 (wrap y, clip x); same wall.
void FUN_00416670(ReadbackJob *job)
{
    unsigned char *base = job->base;
    int baseW = job->baseW;
    int baseH = job->baseH;
    int basePitch = job->basePitch;
    unsigned char *buf = job->buf;
    int width = job->width;
    int bufPitch = job->bufPitch;
    int x = job->x;
    int y = job->y;
    int *keys = job->keys;
    unsigned char *bufEnd = buf + job->height * bufPitch;
    unsigned char *baseEnd;
    unsigned char *srcRow;
    unsigned char *dstRow;
    unsigned char *src;
    unsigned char *dst;
    unsigned char *dstEnd;
    unsigned char c;

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
        srcRow = base + y * basePitch;
        baseEnd = base + baseH * basePitch;
        for (dstRow = buf; dstRow < bufEnd; dstRow += bufPitch) {
            if (srcRow >= baseEnd)
                srcRow = base;
            dstEnd = dstRow + width;
            for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
                if (src >= srcRow + baseW)
                    break;
                c = *src;
                if (keys[c] == 0)
                    *dst = c;
            }
            srcRow += basePitch;
        }
    }
}

// OPEN: 229 vs 243 bytes: wrap y, clamped x (src < row -> row, src >= row + baseW -> row + baseW - 1), plain copy; same wall.
void FUN_00416be0(ReadbackJob *job)
{
    unsigned char *base = job->base;
    int baseW = job->baseW;
    int baseH = job->baseH;
    int basePitch = job->basePitch;
    unsigned char *buf = job->buf;
    int width = job->width;
    int bufPitch = job->bufPitch;
    int x = job->x;
    int y = job->y;
    unsigned char *bufEnd = buf + job->height * bufPitch;
    unsigned char *baseEnd;
    unsigned char *srcRow;
    unsigned char *dstRow;
    unsigned char *src;
    unsigned char *dst;
    unsigned char *dstEnd;
    unsigned char *from;

    if (y < 0)
        y += (1 - y / baseH) * baseH;
    if (y >= baseH)
        y -= y / baseH * baseH;
    srcRow = base + y * basePitch;
    baseEnd = base + baseH * basePitch;
    for (dstRow = buf; dstRow < bufEnd; dstRow += bufPitch) {
        if (srcRow >= baseEnd)
            srcRow = base;
        dstEnd = dstRow + width;
        for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
            if (src < srcRow)
                from = srcRow;
            else if (src >= srcRow + baseW)
                from = srcRow + baseW - 1;
            else
                from = src;
            *dst = *from;
        }
        srcRow += basePitch;
    }
}

// OPEN: 289 vs 277 bytes: keyed variant of 416be0; same wall.
void FUN_00416ce0(ReadbackJob *job)
{
    unsigned char *base = job->base;
    int baseW = job->baseW;
    int baseH = job->baseH;
    int basePitch = job->basePitch;
    unsigned char *buf = job->buf;
    int width = job->width;
    int bufPitch = job->bufPitch;
    int x = job->x;
    int y = job->y;
    int *keys = job->keys;
    unsigned char *bufEnd = buf + job->height * bufPitch;
    unsigned char *baseEnd;
    unsigned char *srcRow;
    unsigned char *dstRow;
    unsigned char *src;
    unsigned char *dst;
    unsigned char *dstEnd;
    unsigned char *from;
    unsigned char c;

    if (y < 0)
        y += (1 - y / baseH) * baseH;
    if (y >= baseH)
        y -= y / baseH * baseH;
    srcRow = base + y * basePitch;
    baseEnd = base + baseH * basePitch;
    for (dstRow = buf; dstRow < bufEnd; dstRow += bufPitch) {
        if (srcRow >= baseEnd)
            srcRow = base;
        dstEnd = dstRow + width;
        for (dst = dstRow, src = srcRow + x; dst < dstEnd; dst++, src++) {
            if (src < srcRow)
                from = srcRow;
            else if (src >= srcRow + baseW)
                from = srcRow + baseW - 1;
            else
                from = src;
            c = *from;
            if (keys[c] == 0)
                *dst = c;
        }
        srcRow += basePitch;
    }
}
