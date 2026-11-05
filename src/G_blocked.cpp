#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

// OPEN + NEEDS REQUEST: lane A declares M_0040f1c0 as void; the function returns 1/0 (int). 333 vs 361 bytes: the original keeps the
// first byte of the character in a dword stack slot (byte store, dword load and 0xff) and sets edi/ebp differently (register/schedule).
int Game::M_0040f1c0(int x, int y, char *text)
{
    char buffer[0x1400];
    DrawRequest request;
    int glyphY;
    unsigned char lead;
    int cellY;
    int glyphX;
    int glyphH;
    int glyphW;
    int column;
    int cellX;
    int rowY;
    char *p;

    strcpy(buffer, text);
    request.flags = 0x416;
    request.dstY = y;
    request.dstX = x;
    request.m_38 = 0;
    request.m_3c = 0;
    request.next = 0;
    request.m_04 = 0;
    column = 0;
    rowY = 0;
    p = buffer;
    while (*p != 0) {
        cellX = column;
        cellY = rowY;
        FUN_00413610((unsigned char *)p, (GlyphSheet *)DAT_00a2330c, &glyphX, &glyphY, &glyphW, &glyphH);
        lead = *p & 0xe0;
        if ((lead == 0xe0) + (lead == 0x80)) {
            p++;
            column++;
        }
        column++;
        if (column >= 50) {
            rowY += 0x18;
            column = 0;
            if (rowY >= 0x90)
                return 1;
        }
        request.srcX = glyphX;
        request.srcY = glyphY;
        request.srcW = glyphW;
        request.srcH = glyphH;
        request.dstX = x + cellX * 10;
        request.dstY = cellY + y;
        if (FUN_00413a90(&request) == 0)
            return 0;
        p++;
    }
    return 1;
}

// OPEN + NEEDS REQUEST: Field needs `int m_7c5210[0x4000]` instead of char m_pad7c5210[0x10000]. 617 vs 638 bytes: the original keeps
// lo/hi/loB/hiB/skip in stack slots and has two more locals; mine keeps them in registers (register/schedule).
void Game::M_0040fd10()
{
    int bestId = DAT_00430658;
    int bestDist = 640000;
    int lo;
    int hi;
    int loB;
    int hiB;
    int skip;
    int found;
    int i;
    int centerX;
    int centerY;
    int id;
    int kind;
    int x;
    int y;
    int dx;
    int dist;
    int spare1;
    int spare2;

    lo = M_0040bf80(0x2e, 0);
    lo += (*m_3d8 > 10) * 3;
    hi = lo + 8;
    loB = M_0040bf80(0x26, 0);
    hiB = loB + 0x26;
    skip = M_0040bf80(2, 0);
    DAT_00a45e38 = 1;
    found = DAT_00430658;
    centerX = 0x140;
    centerY = 0xf0;
    for (i = 0; i < m_87cbc4->m_7c520c; i++) {
        id = m_87cbc4->m_7c5210[i];
        M_0040b4d0(id, 3, &kind, &spare1);
        if (kind == lo) {
            found = i;
            M_0040b4d0(id, 1, &x, &spare2);
            y = M_00409300(id);
            M_0040a520(&x, &y, 0);
            centerX = x;
            centerY = y;
            break;
        }
    }
    if (found != DAT_00430658) {
        found = 0;
        for (i = 0; i < m_87cbc4->m_7c520c; i++) {
            id = m_87cbc4->m_7c5210[i];
            M_0040b4d0(id, 3, &kind, &spare1);
            if ((kind < loB || hiB < kind) && (kind < lo || hi < kind) && kind != skip) {
                M_0040b4d0(id, 1, &x, &spare2);
                y = M_00409300(id);
                M_0040a520(&x, &y, 0);
                if (centerY <= y && x >= 0 && x < 0x280) {
                    dist = (centerY - y) * (centerY - y) + (centerX - x) * (centerX - x);
                    if (dist < 0x9c41 && dist < bestDist) {
                        bestDist = dist;
                        bestId = id;
                    }
                }
            }
        }
        if (bestId != DAT_00430658)
            M_00409d50(bestId, DAT_004efc48, 1, 0);
        DAT_00a45e38 = 0;
    }
}
