#include "game.h"

// OPEN: 241 vs 239 bytes. Flow identical; VC6 strength-reduces buf + w (hoisted lea/add ebp) while the original recomputes it per row and keeps w live (one more stack slot). Wall class: register/schedule (iv strength reduction).

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

// OPEN: 912 vs 913 bytes, flow identical; callee-saved push order/register naming differ (original pushes ebx,esi,edi and ebp late; n in ebp). Globals are modelled as three region structs (StageData at 0xf4c328, OverlayData 0xa4b168, LayerData 0x101de48, StageTable 0xfb1d70); field-sensitive aliasing explains why the original reloads values after stores.

void FUN_00418f70(int stage)
{
    int i;
    int count;
    int n;

    stage--;
    if (stage < 0)
        stage = 0;
    else if (stage > 0x11)
        stage = 0x11;
    DAT_00f4c328.rec = DAT_00fb1d70.rec[stage];
    ImgRec *third = DAT_00f4c328.rec.third;
    DAT_0101de48.img = *DAT_00f4c328.rec.first;
    DAT_00a4b168.img2 = *DAT_00f4c328.rec.first;
    DAT_00f4c328.sceneA = *third;
    n = DAT_00f4c328.rec.extraCount;
    DAT_00f4c328.sceneB = DAT_00f4c328.rec.optional ? *DAT_00f4c328.rec.optional : *third;
    DAT_00a4b168.img[0] = *DAT_00f4c328.rec.fourth;
    DAT_00a4b168.img[1] = *DAT_00f4c328.rec.fifth;
    for (i = 0; i < n; i++)
        DAT_00f4c328.slotImg[i] = *DAT_00f4c328.rec.extra[i];
    DAT_00a4b168.count = n;
    DAT_00fb1d70.bufA = *DAT_00f4c328.rec.first->data;
    DAT_00f4c328.bufA = *DAT_00f4c328.rec.first->data;
    DAT_00f4c328.bufB = *DAT_00f4c328.rec.third->data;
    DAT_00f4c328.bufC = DAT_00f4c328.rec.optional ? *DAT_00f4c328.rec.optional->data : *DAT_00f4c328.rec.third->data;
    DAT_00a4b168.buf[0] = *DAT_00f4c328.rec.fourth->data;
    DAT_00a4b168.buf[1] = *DAT_00f4c328.rec.fifth->data;
    for (i = 0; i < n; i++)
        DAT_00fb1d70.slotBuf[i] = *DAT_00f4c328.rec.extra[i]->data;
    DAT_0101de48.node[2].flags = 0xa0a20000;
    DAT_0101de48.node[2].m_1c = 0;
    DAT_0101de48.node[2].m_20 = 0;
    DAT_0101de48.node[2].m_4c = &DAT_00fb1d70.bufB;
    DAT_0101de48.node[2].img = &DAT_0101de48.img;
    DAT_0101de48.img.data = &DAT_00fb1d70.bufA;
    if (DAT_00f4c328.rec.layered == 1) {
        DAT_00a4b168.node.flags = 0xa0a20000;
        DAT_00a4b168.node.m_1c = 0;
        DAT_00a4b168.node.m_20 = 0;
        DAT_00a4b168.node.m_4c = &DAT_00fb87c8;
        DAT_00a4b168.node.img = &DAT_00a4b168.img2;
        DAT_00a4b168.img2.data = &DAT_00f4c328.bufA;
    }
    DAT_00a4b168.node2.flags = 0x80820000;
    DAT_00a4b168.node2.m_1c = 0;
    DAT_00a4b168.node2.m_20 = 0;
    DAT_00a4b168.node2.img = &DAT_00f4c328.sceneA;
    DAT_00f4c328.sceneA.data = &DAT_00f4c328.bufB;
    DAT_00f4c328.sceneB.data = &DAT_00f4c328.bufC;
    {
        LayerNode *node = DAT_0101de48.node;
        ImgRec *img = DAT_00a4b168.img;
        BigBuf *buf = DAT_00a4b168.buf;
        do {
            node->flags = 0x88820000;
            node->m_1c = 0;
            node->m_20 = 0;
            node->m_50 = 0;
            node->m_54 = 0;
            node->img = img;
            img->data = buf;
            img++;
            node++;
            buf++;
        } while ((int)img < (int)(DAT_00a4b168.img + 2));
    }
    {
        SlotNode *slot = DAT_0101de48.slot;
        ImgRec *img = DAT_00f4c328.slotImg;
        BigBuf *buf = DAT_00fb1d70.slotBuf;
        for (i = 0; i < DAT_00a4b168.count; i++) {
            slot->flags = 0x402;
            slot->m_1c = 0;
            slot->m_20 = 0;
            slot->m_38 = 0;
            slot->m_3c = 0;
            slot->img = img;
            img->data = buf;
            img++;
            slot++;
            buf++;
        }
    }
    count = DAT_00a4b168.count;
    DAT_00a4b168.node2.m_4 = 0;
    DAT_00a4b168.node2.m_8 = &DAT_0101de48.node[0];
    DAT_0101de48.node[1].m_4 = &DAT_0101de48.node[0];
    DAT_0101de48.node[0].m_4 = &DAT_00a4b168.node2;
    DAT_0101de48.node[0].m_8 = &DAT_0101de48.node[1];
    DAT_0101de48.node[1].m_8 = 0;
    DAT_0101de48.node[2].m_4 = 0;
    if (DAT_00f4c328.rec.layered == 0) {
        DAT_0101de48.node[2].m_8 = 0;
    } else {
        DAT_0101de48.node[2].m_8 = &DAT_00a4b168.node;
        DAT_00a4b168.node.m_4 = &DAT_0101de48.node[2];
        DAT_00a4b168.node.m_8 = 0;
    }
    for (i = 0; i < count; i++) {
        if (i == 0)
            DAT_0101de48.slot[i].prev = 0;
        else
            DAT_0101de48.slot[i].prev = &DAT_0101de48.slot[i - 1];
        if (i == count - 1)
            DAT_0101de48.slot[i].next = 0;
        else
            DAT_0101de48.slot[i].next = &DAT_0101de48.slot[i + 1];
    }
    DAT_00f4c324 = 0;
    DAT_00f4c328.clear1 = 0;
    DAT_00f4c328.clear2 = 0;
    DAT_00f4c328.clear3 = 0;
    DAT_00fb1d70.m_3140 = 0;
    DAT_00f4c328.m_dbf8 = 0;
}

// OPEN: 292 vs 303 bytes. The original derives dst as src0 - row - x + buf (induction variable of an eliminated column index); not reproduced.
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

// OPEN: 225 vs 221 bytes. Original keeps dst as a pointer induction variable (d0 = src0 - row - x + buf, test dst < buf + w); mine keeps the column index i. Clamp structure matches.

void FUN_004169f0(ReadbackJob *job)
{
    int w = job->width;
    int basePitch = job->basePitch;
    int x = job->x;
    int bufPitch = job->bufPitch;
    int baseW = job->baseW;
    int baseH = job->baseH;
    unsigned char *base = job->base;
    unsigned char *buf = job->buf;
    unsigned char *bufEnd = buf + job->height * bufPitch;
    unsigned char *srcRow = base + job->y * basePitch;
    unsigned char *row;
    unsigned char *p;
    int i;

    for (; buf < bufEnd; buf += bufPitch) {
        if (srcRow < base)
            row = base;
        else if (srcRow < base + baseH * basePitch)
            row = srcRow;
        else
            row = base + baseH * basePitch - basePitch;
        for (i = 0; i < w; i++) {
            p = row + x + i;
            if (p < row)
                p = row;
            else if (p >= row + baseW)
                p = row + baseW - 1;
            buf[i] = *p;
        }
        srcRow += basePitch;
    }
}
