#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

GlyphSheet *FUN_00413430(unsigned int width, unsigned int height, LOGFONTA *font, COLORREF fg, COLORREF bg, int prefer, int fillBg, void *buf, DWORD *limit)
{
    GlyphSheet *sheet;
    int count;
    int i;
    int x, y, w, h;
    unsigned int columns;
    char text[4];
    DWORD end;

    sheet = (GlyphSheet *)(((DWORD)buf + 3) & ~3);
    if ((DWORD)(sheet + 1) >= *limit)
        return 0;
    FUN_00413780(0, 0, sheet);
    FUN_00413900(0, 0, sheet);
    sheet->cellH = font->lfHeight;
    sheet->cellW = font->lfHeight;
    sheet->halfCellW = (sheet->cellW + 1) >> 1;
    count = sheet->doubleCount + sheet->singleCount;
    if (width == 0) {
        if (height < (unsigned int)sheet->cellH)
            return 0;
        columns = height / sheet->cellH;
        width = ((int)(columns - 1 + count) / (int)columns) * sheet->cellW;
    } else if (height == 0) {
        if (width < (unsigned int)sheet->cellW)
            return 0;
        columns = width / sheet->cellW;
        height = ((int)(columns - 1 + count) / (int)columns) * sheet->cellH;
    } else if ((int)((width / sheet->cellW) * (height / sheet->cellH)) < count) {
        return 0;
    }
    sheet->width = width;
    sheet->height = height;
    end = *limit;
    if (FUN_00412b90(width, height, 0, font, fg, bg, prefer, fillBg, sheet + 1, &end) == 0)
        return 0;
    for (i = 0; i < count; i++) {
        FUN_00413670(i, sheet, &x, &y, &w, &h);
        if (i < sheet->singleCount) {
            FUN_00413780(i, text, sheet);
            text[1] = 0;
        } else {
            FUN_00413900(i - sheet->singleCount, text, sheet);
            text[2] = 0;
        }
        if (FUN_00412e80(text, x, y) == 0) {
            FUN_00412ec0();
            return 0;
        }
    }
    sheet->image = FUN_00412ec0();
    if (sheet->image == 0)
        return 0;
    *limit = end;
    return sheet;
}
