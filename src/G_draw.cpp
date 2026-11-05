#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_0040e7a0(int x, int y, int tile, float scale)
{
    int index = tile;
    int height;

    if (M_0040e340(index) != DAT_00430658 && m_8eced8[index] != 0) {
        M_0040be30(index, &tile, &height);
        if (tile != 0 && height != 0) {
            Req1()->flags = 0x422;
            Req1()->src = m_8eced8[index];
            Req1()->m_3c = m_87fbfc;
            Req1()->m_38 = Req1()->m_3c;
            Req1()->srcY = 0;
            Req1()->srcX = 0;
            Req1()->srcW = tile;
            Req1()->srcH = height;
            Req1()->dstW = (int)(tile * scale);
            Req1()->dstH = (int)(height * scale);
            if (DAT_00a45e38 != 0) {
                x -= Req1()->dstW / 2;
                y -= Req1()->dstH / 2;
            }
            Req1()->dstX = x;
            Req1()->dstY = y;
            Req1()->next = 0;
            Req1()->m_04 = 0;
            FUN_00413a90(Req1());
        }
    }
}

void Game::M_0040e900(int a)
{
    m_8ef790 = (char *)&DAT_00a232a8;
    DAT_008f1428.bpp = 8;
    DAT_008f1428.palette = &DAT_00a3c650;
    DAT_008f1428.pixels = DAT_00951fd0;
    if (a != 0) {
        Req0()->flags = 8;
        Req0()->dst = &DAT_008f1428;
    } else {
        Req0()->flags = 2;
        Req0()->src = &DAT_008f1428;
    }
    Req0()->dstY = 0;
    Req0()->srcY = 0;
    Req0()->dstX = 0;
    Req0()->srcX = 0;
    Req0()->dstW = 0x280;
    Req0()->srcW = 0x280;
    DAT_008f1428.width = 0x280;
    Req0()->dstH = 0x1e0;
    Req0()->srcH = 0x1e0;
    DAT_008f1428.height = 0x1e0;
    Req0()->next = 0;
    Req0()->m_04 = 0;
    FUN_00413a90(Req0());
}

void Game::M_0040e9f0(int x, int y, int tile, float scale)
{
    int index = tile;
    int height;

    if (M_0040e340(index) != DAT_00430658 && m_8eced8[index] != 0) {
        M_0040be30(index, &tile, &height);
        if (tile != 0 && height != 0) {
            Req1()->flags = 0x622;
            Req1()->src = m_8eced8[index];
            Req1()->m_3c = m_87fbfc;
            Req1()->m_38 = Req1()->m_3c;
            Req1()->srcY = 0;
            Req1()->srcX = 0;
            Req1()->srcW = tile;
            Req1()->srcH = height;
            Req1()->dstW = (int)(tile * scale);
            Req1()->dstH = (int)(height * scale);
            if (DAT_00a45e38 != 0) {
                x -= Req1()->dstW / 2;
                y -= Req1()->dstH / 2;
            }
            Req1()->dstX = x;
            Req1()->dstY = y;
            Req1()->m_30 = 0x32;
            Req1()->m_34 = blendHalf;
            Req1()->next = 0;
            Req1()->m_04 = 0;
            FUN_00413a90(Req1());
        }
    }
}

void Game::M_0040eb70(int x, int y, int tile)
{
    int index = tile;
    int height;

    if (M_0040e340(index) != DAT_00430658 && m_8ef6d8 != 0) {
        M_0040be30(index, &tile, &height);
        if (tile != 0 && height != 0) {
            Req0()->flags = 0x402;
            Req0()->src = m_8eced8[index];
            Req0()->m_3c = m_87fbfc;
            Req0()->m_38 = Req0()->m_3c;
            Req0()->srcY = 0;
            Req0()->srcX = 0;
            Req0()->srcW = tile;
            Req0()->dstW = tile;
            Req0()->srcH = height;
            Req0()->dstH = height;
            Req0()->dstX = x;
            Req0()->dstY = y;
            Req0()->next = 0;
            Req0()->m_04 = 0;
            FUN_00413a90(Req0());
        }
    }
}

void Game::M_0040ec90(int x, int y, int dstW, int dstH, int tile)
{
    int index = M_0040bf80(1, tile);
    int height;

    if (M_0040e340(index) != DAT_00430658 && m_8eced8[index] != 0) {
        M_0040be30(index, &tile, &height);
        if (tile != 0 && height != 0) {
            if (DAT_00a45e38 != 0) {
                x -= tile;
                y -= height;
            }
            Req1()->flags = 0x22;
            Req1()->src = m_8eced8[index];
            Req1()->srcY = 0;
            Req1()->srcX = 0;
            Req1()->srcW = tile;
            Req1()->srcH = height;
            Req1()->dstX = x;
            Req1()->dstY = y;
            Req1()->dstW = dstW;
            Req1()->dstH = dstH;
            Req1()->next = 0;
            Req1()->m_04 = 0;
            FUN_00413a90(Req1());
        }
    }
}

void Game::M_0040edc0(int x, int y, int dstW, int dstH, int tile)
{
    int index = tile;
    int height;

    if (M_0040e340(index) != DAT_00430658 && m_8eced8[index] != 0) {
        M_0040be30(index, &tile, &height);
        if (tile != 0 && height != 0) {
            if (DAT_00a45e38 != 0) {
                x -= tile;
                y -= height;
            }
            Req1()->flags = 0x422;
            Req1()->src = m_8eced8[index];
            Req1()->srcY = 0;
            Req1()->srcX = 0;
            Req1()->srcW = tile;
            Req1()->srcH = height;
            Req1()->dstX = x;
            Req1()->dstY = y;
            Req1()->dstW = dstW;
            Req1()->dstH = dstH;
            Req1()->m_3c = m_87fbfc;
            Req1()->m_38 = Req1()->m_3c;
            Req1()->next = 0;
            Req1()->m_04 = 0;
            FUN_00413a90(Req1());
        }
    }
}

void Game::M_0040f0a0()
{
    PaletteTable palette;
    LOGFONTA font;
    unsigned long limit;

    if (DAT_00a46050 == 0) {
        DAT_00a46050 = 1;
        FUN_004139f0(&palette);
        palette.entry[0].b = 0;
        palette.entry[0].g = 0;
        palette.entry[0].r = 0;
        palette.entry[255].b = 0xff;
        palette.entry[255].g = 0xff;
        palette.entry[255].r = 0xff;
        FUN_00413a20(&palette);
        FUN_00413a50();
        font.lfHeight = 0x14;
        font.lfWidth = 0;
        font.lfEscapement = 0;
        font.lfOrientation = 0;
        font.lfWeight = 700;
        font.lfItalic = 0;
        font.lfUnderline = 0;
        font.lfStrikeOut = 0;
        font.lfCharSet = 1;
        font.lfOutPrecision = 0;
        font.lfClipPrecision = 0;
        font.lfQuality = 0;
        font.lfPitchAndFamily = 0;
        strcpy(font.lfFaceName, "\202l\202r \203S\203V\203b\203N");
        limit = (unsigned long)(m_8ef798 + sizeof(m_8ef798));
        DAT_00a2330c = (int *)FUN_00413430(0x280, 0x960, &font, 0xffffff, 0, 0, 1, DAT_004f069c, &limit);
        FUN_0040f060();
        FUN_00417ee0(FUN_0040f060);
    }
}
