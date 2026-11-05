#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include "game.h"

/* Renders one font's glyphs into a system-memory surface and copies the result into a caller-supplied buffer. */
int FUN_00412e80(const char *text, int x, int y)
{
    return TextOut(DAT_00a4a858, x, y, text, strlen(text)) != 0;
}

/* Finishes the glyph sheet started by FUN_00412b90: releases the DC and font, copies the rendered surface into
   the image as palette indices and (8 bpp) rebuilds the image palette from the two text colours. */
/* Nearest entry of the shadow palette to a colour; on equal distance prefer a used (prefer == 1) or free
   (prefer == 0) slot. */
/* Fills a surface with one byte value. */
/* Copies a surface into dst, mapping the two text pixel values to their palette indices. */
/* Cell rectangle of glyph number index inside the sheet. */
int FUN_00413670(int index, GlyphSheet *sheet, int *x, int *y, int *w, int *h)
{
    int columns = sheet->width / sheet->cellW;

    if (index < 0 || index >= sheet->doubleCount + sheet->singleCount)
        return 0;
    *x = (index % columns) * sheet->cellW;
    *y = (index / columns) * sheet->cellH;
    *h = sheet->cellH;
    if (index < sheet->singleCount) {
        *w = sheet->halfCellW;
        return 1;
    }
    *w = sheet->cellW;
    return 1;
}

/* Sheet index of a single-byte character, or -1. */
/* Sheet index of a double-byte character, or -1. */
int FUN_00413610(const unsigned char *ch, GlyphSheet *sheet, int *x, int *y, int *w, int *h)
{
    int index = FUN_004136f0(ch);

    if (index < 0) {
        index = FUN_00413870(ch);
        if (index < 0)
            return 0;
        index += sheet->singleCount;
    }
    return FUN_00413670(index, sheet, x, y, w, h) != 0;
}

int FUN_004139f0(PaletteTable *out)
{
    if (DAT_00a4a6f0 != 8)
        return 0;
    *out = DAT_00a4a890;
    return 1;
}

int FUN_00413a20(const PaletteTable *in)
{
    if (DAT_00a4a6f0 != 8)
        return 0;
    DAT_00a4a890 = *in;
    return 1;
}

int FUN_00413a50()
{
    if (DAT_00a4a6f0 != 8)
        return 0;
    if (DAT_00a4a87c) {
        if (DAT_00a4a87c->SetEntries(0, 0, 256, (LPPALETTEENTRY)&DAT_00a4a890) != 0)
            return 0;
        DAT_00a4a88c = 0;
    }
    return 1;
}

/* Builds a glyph sheet for the whole character set of font in buf: fixed cells, a column count chosen from
   the width or height given (0 = derive it), glyphs drawn one per cell. */
