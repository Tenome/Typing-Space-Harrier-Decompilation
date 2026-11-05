#include <stdio.h>
#include <string.h>
#include "game.h"

// OPEN: 0x417530 distortion blit, 447 = 447 B, 89 %. Prologue declared in the original load order (baseH, base, baseW, width, table, buf, basePitch, bufPitch, y, rows, x) then hill-climbed; left: the x register in the wrap block (original keeps x in edx and moves it through eax around cdq) and the slot order of base/basePitch/width.
void FUN_00417530(DistortJob *job)
{
    int baseW = job->baseW;
    int baseH = job->baseH;
    unsigned char *buf = job->buf;
    int width = job->width;
    int basePitch = job->basePitch;
    int step;
    int bufPitch = job->bufPitch;
    int x = job->x;
    int sx;
    DistortRow *end;
    int y = job->y;
    unsigned char *srcRow;
    int sy;
    int rows = job->rows;
    unsigned char *dstRow;
    DistortRow *row = job->table;
    unsigned char *dst;
    int fx;
    unsigned char *base = job->base;
    int wrapW = baseW << 16;
    int widthFixed = width << 16;
    unsigned char *dstEnd;

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

// OPEN: 0x4176f0 keyed distortion blit, 459 vs 503 B. keys[srcRow[fx>>16]] twice (no c local) is closer than the c local; the original also spills step and reloads wrapW/keys into ebp each iteration (register pressure), so one more live value than mine.
void FUN_004176f0(DistortJob *job)
{
    int rows = job->rows;
    unsigned char *dstEnd;
    int sy;
    int basePitch = job->basePitch;
    unsigned char *dstRow;
    int baseH = job->baseH;
    int sx;
    int x = job->x;
    int fx;
    int wrapW = baseW << 16;
    int width = job->width;
    int *keys = job->keys;
    unsigned char *srcRow;
    int baseW = job->baseW;
    unsigned char *buf = job->buf;
    unsigned char *dst;
    DistortRow *end;
    unsigned char *base = job->base;
    int widthFixed = width << 16;
    int bufPitch = job->bufPitch;
    DistortRow *row = job->table;
    int step;
    int y = job->y;

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
            if (keys[srcRow[fx >> 16]] == 0)
                *dst = srcRow[fx >> 16];
            fx += step;
        }
    }
}

// OPEN: 0x417900 scaled 16.16 wrap blit, 291 vs 284 B. Original keeps width in the dead job slot for the first idiv and hoists x<<16 to a slot.
void FUN_00417900(ScaledBlitJob *job)
{
    int y = job->m_28;
    int x = job->x;
    unsigned char *base = job->base;
    unsigned char *dst;
    int wrapH = baseH << 16;
    unsigned char *dstEnd;
    unsigned char *buf = job->buf;
    int fy;
    int baseW = job->baseW;
    int rows = job->rows;
    unsigned char *dstRow;
    int basePitch = job->basePitch;
    int bufPitch = job->bufPitch;
    int fx;
    int wrapW = baseW << 16;
    int stepY = (job->m_30 << 16) / rows;
    int baseH = job->baseH;
    unsigned char *srcRow;
    int stepX = (job->m_2c << 16) / width;
    unsigned char *bufEnd;
    int width = job->width;

    if (y < 0)
        y += (1 - y / baseH) * baseH;
    if (y >= baseH)
        y -= y / baseH * baseH;
    bufEnd = buf + bufPitch * rows;
    fy = y << 16;
    for (dstRow = buf; dstRow < bufEnd; dstRow += bufPitch) {
        while (fy >= wrapH)
            fy -= wrapH;
        srcRow = base + (fy >> 16) * basePitch;
        fx = x << 16;
        dstEnd = dstRow + width;
        for (dst = dstRow; dst < dstEnd; dst++) {
            while (fx >= wrapW)
                fx -= wrapW;
            *dst = srcRow[fx >> 16];
            fx += stepX;
        }
        fy += stepY;
    }
}

// OPEN: 0x417b80 scaled blit, 404 vs 407 B, prologue declared in the original load order; table/row load schedule and x-in-param-slot normalisation differ.
struct ScaleRowZ4 {
    int lineShift;
    int rowShift;
    int divisor;
};

void FUN_00417b80(ScaledBlitJob *job)
{
    int fy;
    int stepX;
    int baseH = job->baseH;
    int baseW = job->baseW;
    unsigned char *base = job->base;
    int basePitch = job->basePitch;
    int wrapW = baseW << 16;
    int rows = job->rows;
    unsigned char *dstRow = job->buf;
    int x = job->x;
    int width = job->width;
    int wrapH = baseH << 16;
    unsigned char *srcRow;
    int bufPitch = job->bufPitch;
    int fx;
    int originX = job->m_2c << 16;
    ScaleRowZ4 *row = (ScaleRowZ4 *)job->table;
    int originY = job->m_28 << 16;
    int stepY = (job->m_30 << 16) / rows;
    ScaleRowZ4 *end = row + rows;
    unsigned char *dst;
    unsigned char *dstEnd;

    if (x < 0)
        x += (1 - x / baseW) * baseW;
    if (x >= baseW)
        x -= x / baseW * baseW;
    for (; row < end; row++, dstRow += bufPitch) {
        if (row->lineShift == -1 || row->divisor <= 0)
            continue;
        stepX = originX / row->divisor;
        dstEnd = dstRow + width;
        fy = row->lineShift * stepY + originY;
        while (fy < 0)
            fy += wrapH;
        while (fy >= wrapH)
            fy -= wrapH;
        srcRow = base + (fy >> 16) * basePitch;
        fx = x - (row->rowShift * stepX >> 16);
        while (fx < 0)
            fx += baseW;
        while (fx >= baseW)
            fx -= baseW;
        fx <<= 16;
        for (dst = dstRow; dst < dstEnd; dst++) {
            while (fx >= wrapW)
                fx -= wrapW;
            *dst = srcRow[fx >> 16];
            fx += stepX;
        }
    }
}

// OPEN: 0x40dd60 speech balloon, 1337 = 1337 B, 88.6 %. Only the zero register (ebx in the original, reading in esi) differs; locals reorder, local copy of reading and strlen reuse did not change it.
int Game::M_0040dd60(int index, char *reading, char *text, int weight, int lines)
{
    int k;
    int cx;
    int box[7];
    int top;
    int boxH;
    int textRows;
    int left;
    int boxW;
    int length;
    int cy;
    int textCols;
    int rows;
    int cells;
    int limit;
    int star;

    if (*text == 0)
        return DAT_00430658;
    star = 0;
    if (*text == '*')
        star = 1;
    k = M_00401930();
    if (k < 0)
        return k;
    limit = M_0040f720(index);
    m_8c4->m_0 = 1;
    m_8c4->m_60 = star;
    strcpy(m_8c4->m_1494, text);
    strcpy(m_8c4->m_94, reading);
    m_8c4->m_88 = strlen(m_8c4->m_1494) / 2;
    m_8c4->m_8 = index;
    m_8c4->m_84 = weight;
    if (m_8c4->m_84 > limit)
        m_8c4->m_84 = limit;
    m_8c4->m_7c = 5;
    m_8c4->m_68 = 1;
    m_8c4->m_6c = 0;
    m_8c4->m_78 = lines;
    m_8c4->m_70 = 0;
    m_8c4->m_74 = 0;
    M_00402e90();
    M_00402f10(0);
    cells = strlen(reading) / 2;
    rows = (cells - 1) / 25 + 1;
    m_8c4->m_8c = 25;
    m_8c4->m_90 = 6;
    if (cells < 25)
        m_8c4->m_8c = cells;
    if (rows < 6)
        m_8c4->m_90 = rows;
    m_8c4->m_50e0 = -1;
    if (M_00410440(m_8c4->m_8)) {
        m_8c4->m_18 = 1;
        m_8c4->m_14 = 1;
    } else {
        m_8c4->m_18 = 0;
        m_8c4->m_14 = 0;
    }
    if (m_398 != 0) {
        m_8c4->m_18 = 0;
        m_8c4->m_14 = 0;
    }
    length = strlen(reading);
    textCols = length;
    if (length >= 50)
        textCols = 50;
    textRows = (length - 1) / 50 + 1;
    if (textRows >= 6)
        textRows = 6;
    box[0] = textCols * 10;
    if (textRows > 1)
        box[1] = textRows * 24 - 4;
    else
        box[1] = 20;
    box[2] = M_00402a30();
    box[3] = M_00402a50();
    if (!star) {
        if (m_8c4->m_18 != 0) {
            for (k = 0; k < 2; k++) {
                if (box[0] < box[2 + k])
                    box[0] = box[2 + k];
            }
        } else if (box[0] < box[2]) {
            box[0] = box[2];
        }
    }
    boxW = box[0] + 8;
    if (star)
        boxH = box[1] + 8;
    else
        boxH = box[1] + 40;
    cx = 0x140;
    cy = 0xf0;
    if (M_00401da0(m_8c4->m_8) == 0)
        M_00401d70(index, &cx, &cy);
    left = cx - boxW / 2;
    top = cy - boxH / 2;
    if (left < 0x10) {
        cx += 0x10 - left;
        left = 0x10;
    }
    if (left + boxW >= 0x260) {
        cx += 0x260 - left - boxW;
        left = 0x260 - boxW;
    }
    if (top < 0) {
        cy -= top;
        top = 0;
    }
    if (top + boxH >= 0x1b0) {
        cy += 0x1b0 - top - boxH;
        top = 0x1b0 - boxH;
    }
    m_8c4->m_1c = left;
    m_8c4->m_20 = top;
    m_8c4->m_24 = boxW;
    m_8c4->m_28 = boxH;
    m_8c4->m_3c = cx - box[0] / 2;
    m_8c4->m_40 = top + 4;
    m_8c4->m_2c = cx;
    m_8c4->m_30 = m_8c4->m_40 + box[1] + 4;
    m_8c4->m_34 = cx - box[2] / 2;
    m_8c4->m_38 = m_8c4->m_30 + 8;
    if (m_8c4->m_18 == 0 && !star) {
        m_8c4->m_28 = boxH - 12;
        m_8c4->m_38 = m_8c4->m_40 + box[1] + 4;
    }
    m_8c4->m_3c -= m_8c4->m_1c;
    m_8c4->m_40 -= m_8c4->m_20;
    m_8c4->m_2c -= m_8c4->m_1c;
    m_8c4->m_30 -= m_8c4->m_20;
    m_8c4->m_34 -= m_8c4->m_1c;
    m_8c4->m_38 -= m_8c4->m_20;
    m_8c4->m_4c = 0;
    m_8c4->m_48 = 0;
    m_8c4->m_44 = 0;
    m_8c4->m_58 = m_8c4->m_1c;
    m_8c4->m_50 = m_8c4->m_58;
    m_8c4->m_5c = m_8c4->m_20;
    m_8c4->m_54 = m_8c4->m_5c;
    return m_8c0;
}

// OPEN: 0x409700 stage-object spawn, 71.8 %, W draft unchanged; slot layout of the scalars differs.
int Game::M_00409700(int a, int b, int flag)
{
    int buf[0x40];
    int count;
    int x;
    int y;
    int cx;
    int w;
    int h;
    int len;
    int tick;
    int delta;
    int step;
    int sx;
    int sw;
    int sh;
    int rows;
    int saved;
    int i;
    int j;
    int span;
    int t;
    int v;
    int *table;

    M_0040af40(0, a);
    M_0040af40(3, b);
    if (m_87cbc8->m_10 != 0)
        return 1;
    M_0040b4d0(a, 1, &x, &y);
    cx = x;
    len = M_00409300(a);
    M_0040b4d0(a, 2, &w, &h);
    sx = x - 320;
    w *= 2;
    h *= 2;
    if (len <= 0x146)
        len = 0x14a;
    tick = M_004096b0(len);
    if (tick < 0)
        return DAT_00430658;
    delta = 0x14 - tick;
    if (flag != 0) {
        count = delta * 2 + 4;
        if (count > 0x40)
            count = 0x40;
    } else
        count = 4;
    for (i = 0; i < count; i++) {
        buf[i] = M_0040bbe0();
        if (buf[i] == 0)
            return DAT_00430658;
    }
    M_0040af40(0, a);
    M_0040af40(3, b);
    if (flag == 0) {
        m_87cbc8->m_4 = 0x36;
        m_87cbc8->m_8 = 0x36;
        M_0040b0e0(buf[0], 1, x, y, 0, 1);
        M_0040b0e0(buf[1], 1, x, y, 1, 0);
        M_0040b0e0(buf[2], 1, w, h, 0, 1);
        M_0040b0e0(buf[3], 1, w, h, 1, 0);
        for (i = 0; i < 25; i++) {
            if (m_87cbd4->m_2c[i] > 0)
                (&m_87cbc8->m_2c)[i] = m_87cbd4->m_2c[i];
        }
        m_87cbc8->m_64 = buf[0];
        m_87cbc8->m_6c = buf[2];
        m_87cbc8->m_18 = 0;
    } else {
        span = len - 0x146;
        if (span <= 0) {
            len = 0x14a;
            span = 4;
        }
        sx = (sx << 10) / span;
        sw = (w << 10) / span;
        sh = (h << 10) / span;
        M_0040b0e0(buf[0], 1, x, y, 0, 1);
        rows = (len - y) / 32 + 1;
        M_0040b0e0(buf[1], 1, cx, len, rows, 1);
        step = delta * 4;
        saved = buf[2 + delta];
        M_0040b0e0(saved, 1, w, h, 0, 1);
        M_0040b0e0(buf[3 + delta], 1, w, h, rows, 1);
        if (delta > 0) {
            table = &DAT_004e246c[tick];
            for (j = 0; j < delta; j++, table++) {
                t = *table;
                v = t - 0x146;
                M_0040b0e0(buf[2 + j], 1, v * sx / 1024 + 0x140, t, 2, 1);
                M_0040b0e0(buf[4 + delta + j], 1, sw * v / 1024, sh * v / 1024, 2, 1);
            }
        }
        for (i = 0; i < delta + 2; i++) {
            for (j = 0; j < 2; j++) {
                v = buf[i + j * delta];
                M_0040aff0(5, v);
                if (i == delta + 1)
                    m_87cbec->m_10 = 0;
                else
                    m_87cbec->m_10 = buf[i + j * delta + 1] - v;
            }
        }
        for (i = 0; i < 25; i++) {
            if (m_87cbd4->m_2c[i] > 0) {
                (&m_87cbc8->m_2c)[i] = m_87cbd4->m_2c[i];
                m_87cbc8->m_18 = 0;
            }
        }
        m_87cbc8->m_64 = buf[0];
        m_87cbc8->m_6c = saved;
    }
    M_0040ba00(m_87cbc8->m_80);
    M_0040ba00(m_87cbc8->m_84);
    M_0040ba00(m_87cbc8->m_78);
    M_0040ba00(m_87cbc8->m_7c);
    M_0040ba00(m_87cbc8->m_60);
    M_0040ba00(m_87cbc8->m_68);
    M_0040ba00(m_87cbc8->m_70);
    m_87cbc8->m_10 = 1;
    for (i = 0; i < 0x40; i++) {
        if (i < count)
            (&m_87cbc8->m_0)[0x24 + i] = buf[i];
        else
            (&m_87cbc8->m_0)[0x24 + i] = 0;
    }
    m_a7978++;
    if (m_a7978 >= 3)
        m_a7978 = 0;
    M_00408650(m_a7978 + 0xb);
    return 0;
}

