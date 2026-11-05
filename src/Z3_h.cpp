#include <windows.h>
#include <string.h>
#include "game.h"

SheetImage *FUN_004129a0(const unsigned char *file, unsigned int bufferStart, unsigned int *limit)
{
    SheetImage *image;
    BITMAPINFOHEADER info;
    PaletteTable *palette;
    unsigned char *pixels;
    unsigned char *row;
    unsigned int rowBytes;
    unsigned int skip;
    unsigned int end;
    unsigned int count;
    int colors;
    int i;
    int x;

    image = (SheetImage *)((bufferStart + 3) & ~3u);
    if ((unsigned int)(image + 1) >= *limit)
        return 0;
    if (file[0] != 'B')
        return 0;
    if (file[1] != 'M')
        return 0;
    colors = (*(unsigned int *)(file + 10) - 0x36) >> 2;
    if (colors < 0 || colors > 0x100)
        colors = 0x100;
    info = *(const BITMAPINFOHEADER *)(file + 14);
    file += 0x36;
    image->width = info.biWidth;
    image->height = info.biHeight;
    image->bpp = info.biBitCount;
    if (info.biBitCount != 8) {
        image->palette = 0;
        pixels = (unsigned char *)(((unsigned int)(image + 1) + 3) & ~3u);
    } else {
        palette = (PaletteTable *)(((unsigned int)(image + 1) + 3) & ~3u);
        image->palette = palette;
        if ((unsigned int)(palette + 1) >= *limit)
            return 0;
        i = 0;
        if (colors > 0) {
            for (i = colors; i != 0; i--) {
                unsigned int rgb = *(const unsigned int *)file;
                int n = colors - i;
                palette->used[n] = 0;
                palette->entry[n].r = (unsigned char)(rgb >> 16);
                palette->entry[n].g = (unsigned char)(rgb >> 8);
                palette->entry[n].b = (unsigned char)rgb;
                palette->entry[n].flags = 0;
                file += 4;
            }
            i = colors;
        }
        if (colors < 0x100) {
            for (; i < 0x100; i++) {
                palette->used[i] = 0;
                palette->entry[i].r = 0;
                palette->entry[i].g = 0;
                palette->entry[i].b = 0;
                palette->entry[i].flags = 0;
            }
        }
        pixels = (unsigned char *)(((unsigned int)(palette + 1) + 3) & ~3u);
    }
    rowBytes = (unsigned int)(image->width * image->bpp) >> 3;
    image->pixels = pixels;
    end = rowBytes * image->height + (unsigned int)pixels;
    skip = ((rowBytes + 3) & ~3u) - rowBytes;
    if (end >= *limit)
        return 0;
    *limit = end;
    if (image->height != 0) {
        count = image->height;
        row = pixels + rowBytes * (count - 1);
        do {
            memcpy(row, file, rowBytes);
            file += rowBytes + skip;
            if (image->bpp == 8) {
                for (x = 0; x < (int)rowBytes; x++)
                    image->palette->used[row[x]] = 1;
            }
            row -= rowBytes;
            count--;
        } while (count > 0);
    }
    return image;
}
