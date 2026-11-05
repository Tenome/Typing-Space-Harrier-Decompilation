#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>
#include "game.h"

// OPEN: stack slot order of the colour temporaries and bgPixel/palette register sharing differ; size 732 vs 738
int FUN_00412b90(int width, int height, int charExtra, LOGFONTA *font, COLORREF fg, COLORREF bg, int prefer, int fillBg, void *buf, DWORD *limit)
{
    IDirectDrawSurface *surf;
    HDC hdc;
    HFONT hfont;
    int fgIndex;
    DWORD fgPixel;
    COLORREF fgRgb;
    DWORD bgPixel;
    COLORREF bgRgb;
    int bgIndex;
    DDSURFACEDESC desc;
    SheetImage *image;
    PaletteTable *palette;
    unsigned char *pixels;
    DWORD end;

    FUN_00412fe0(fg, bg, prefer, &fgRgb, &bgRgb, &fgIndex, &bgIndex);
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(DDSURFACEDESC);
    desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    desc.dwWidth = width;
    desc.dwHeight = height;
    if (DAT_00a4a6fc->CreateSurface(&desc, &surf, NULL) != 0)
        return 0;
    fgPixel = FUN_00412880(surf, fgRgb);
    bgPixel = FUN_00412880(surf, bgRgb);
    if (fillBg != 1 || FUN_00413280(surf, bgIndex)) {
        image = (SheetImage *)(((DWORD)buf + 3) & ~3);
        if ((DWORD)(image + 1) < *limit) {
            image->width = width;
            image->height = height;
            image->bpp = DAT_00a4a6f0;
            if (DAT_00a4a6f0 == 8) {
                image->palette = palette = (PaletteTable *)(((DWORD)(image + 1) + 3) & ~3);
                if ((DWORD)(palette + 1) >= *limit)
                    goto fail;
                pixels = (unsigned char *)(((DWORD)(palette + 1) + 3) & ~3);
            } else {
                image->palette = 0;
                palette = (PaletteTable *)bgPixel;
                pixels = (unsigned char *)(((DWORD)(image + 1) + 3) & ~3);
            }
            image->pixels = pixels;
            end = (image->bpp * width * height >> 3) + (DWORD)pixels;
            if (end >= *limit) {
                surf->Release();
                return 0;
            }
            *limit = end;
            if (surf->GetDC(&hdc) != 0)
                return 0;
            hfont = CreateFontIndirect(font);
            if (hfont) {
                SelectObject(hdc, hfont);
                if (SetTextCharacterExtra(hdc, charExtra) == 0x80000000) {
                    DeleteObject(hfont);
                    surf->ReleaseDC(hdc);
                    surf->Release();
                    return 0;
                }
                SetBkColor(hdc, bg);
                if (SetTextColor(hdc, fg) != CLR_INVALID) {
                    DAT_00a4a840 = fg;
                    DAT_00a4a830 = surf;
                    DAT_00a4a83c = pixels;
                    DAT_00a4a84c = bgIndex;
                    DAT_00a4a834 = image;
                    DAT_00a4a844 = bg;
                    DAT_00a4a848 = fgIndex;
                    DAT_00a4a85c = hfont;
                    DAT_00a4a838 = palette;
                    DAT_00a4a858 = hdc;
                    DAT_00a4a850 = fgPixel;
                    DAT_00a4a854 = bgPixel;
                    return 1;
                }
                DeleteObject(hfont);
            }
            surf->ReleaseDC(hdc);
        }
    }
fail:
    surf->Release();
    return 0;
}

// OPEN: register/spill order of the global snapshot differs (289 vs 279 bytes)
SheetImage *FUN_00412ec0()
{
    COLORREF bg = DAT_00a4a844;
    SheetImage *image = DAT_00a4a834;
    COLORREF fg = DAT_00a4a840;
    IDirectDrawSurface *surf = DAT_00a4a830;
    int bgIndex = DAT_00a4a84c;
    PaletteTable *palette = DAT_00a4a838;
    DWORD fgPixel = DAT_00a4a850;
    DWORD bgPixel = DAT_00a4a854;
    int fgIndex = DAT_00a4a848;
    int i;

    DeleteObject(DAT_00a4a85c);
    surf->ReleaseDC(DAT_00a4a858);
    if (!FUN_00413350(surf, DAT_00a4a83c, fgIndex, bgIndex, fgPixel, bgPixel)) {
        surf->Release();
        return 0;
    }
    if (image->bpp == 8) {
        for (i = 0; i < 256; i++) {
            palette->used[i] = 0;
            palette->entry[i].r = 0;
            palette->entry[i].g = 0;
            palette->entry[i].b = 0;
            palette->entry[i].flags = 0;
        }
        palette->used[bgIndex] = 1;
        palette->entry[bgIndex].r = GetRValue(bg);
        palette->entry[bgIndex].g = GetGValue(bg);
        palette->entry[bgIndex].b = GetBValue(bg);
        palette->used[fgIndex] = 1;
        palette->entry[fgIndex].r = GetRValue(fg);
        palette->entry[fgIndex].g = GetGValue(fg);
        palette->entry[fgIndex].b = GetBValue(fg);
    }
    surf->Release();
    return image;
}

// OPEN: helper of FUN_00412fe0
static __inline int FindNearestColor(COLORREF color, int prefer)
{
    int best = -1;
    int bestOffset = -4;
    int bestDist = 0x30000;
    int offset = 0;
    int i;

    for (i = 0; offset < 0x400; i++, offset += 4) {
        int dr = DAT_00a4a890.entry[i].r - GetRValue(color);
        int dg = DAT_00a4a890.entry[i].g - GetGValue(color);
        int db = DAT_00a4a890.entry[i].b - GetBValue(color);
        int dist = dr * dr + dg * dg + db * db;

        if (dist < bestDist) {
            bestDist = dist;
            best = i;
            bestOffset = offset;
        } else if (dist == bestDist) {
            if (prefer == 0) {
                if (DAT_00a4a890.used[bestOffset / 4] == 1 && DAT_00a4a890.used[i] == 0) {
                    best = i;
                    bestOffset = offset;
                }
            } else if (prefer == 1) {
                if (DAT_00a4a890.used[bestOffset / 4] == 0 && DAT_00a4a890.used[i] == 1) {
                    best = i;
                    bestOffset = offset;
                }
            }
        }
        if (bestDist == 0) {
            if (prefer == 0) {
                if (DAT_00a4a890.used[bestOffset / 4] == 0)
                    break;
            } else if (prefer == 1) {
                if (DAT_00a4a890.used[bestOffset / 4] == 1)
                    break;
            }
        }
    }
    return best;
}

// OPEN: original keeps best index and byte offset in two variables; loops not reproduced (810 vs 671)
void FUN_00412fe0(COLORREF color1, COLORREF color2, int prefer, COLORREF *rgb1, COLORREF *rgb2, int *index1, int *index2)
{
    int best;

    best = FindNearestColor(color1, prefer);
    *rgb1 = RGB(DAT_00a4a890.entry[best].r, DAT_00a4a890.entry[best].g, DAT_00a4a890.entry[best].b);
    *index1 = best;
    best = FindNearestColor(color2, prefer);
    *rgb2 = RGB(DAT_00a4a890.entry[best].r, DAT_00a4a890.entry[best].g, DAT_00a4a890.entry[best].b);
    *index2 = best;
}

// OPEN: wall: loop inversion duplicates the Lock call (207 vs 193); inner fill matches
int FUN_00413280(IDirectDrawSurface *surf, unsigned char value)
{
    DDSURFACEDESC desc;
    HRESULT hr;
    unsigned char *line;
    int y;

    desc.dwSize = sizeof(desc);
    do {
        hr = surf->Lock(NULL, &desc, 0, NULL);
        if (hr == DD_OK) {
            line = (unsigned char *)desc.lpSurface;
            for (y = desc.dwHeight; y > 0; y--) {
                memset(line, value, desc.dwWidth);
                line += desc.lPitch;
            }
            surf->Unlock(NULL);
            return 1;
        }
    } while (hr != DDERR_SURFACELOST && hr == DDERR_WASSTILLDRAWING);
    return 0;
}

// OPEN: same Lock-loop wall as FUN_00413280
int FUN_00413350(IDirectDrawSurface *surf, unsigned char *dst, unsigned char fgIndex, unsigned char bgIndex, unsigned int fgPixel, unsigned int bgPixel)
{
    DDSURFACEDESC desc;
    HRESULT hr;
    unsigned char *src;
    unsigned char *end;
    unsigned char pixel;
    int y;

    desc.dwSize = sizeof(desc);
    do {
        hr = surf->Lock(NULL, &desc, 0, NULL);
        if (hr == DD_OK) {
            src = (unsigned char *)desc.lpSurface;
            for (y = desc.dwHeight; y > 0; y--) {
                end = src + desc.dwWidth;
                while (src < end) {
                    pixel = *src++;
                    if (pixel == fgPixel)
                        pixel = fgIndex;
                    if (pixel == bgPixel)
                        pixel = bgIndex;
                    *dst++ = pixel;
                }
                src += desc.lPitch - desc.dwWidth;
            }
            surf->Unlock(NULL);
            return 1;
        }
    } while (hr != DDERR_SURFACELOST && hr == DDERR_WASSTILLDRAWING);
    return 0;
}

// OPEN: original spills the byte to the stack and widens it into edi; 107 vs 133 bytes
int FUN_004136f0(const unsigned char *ch)
{
    unsigned char c = *ch;
    int base = 0;
    unsigned int i;

    if ((c & 0xe0) == 0x80 || (c & 0xe0) == 0xe0)
        return -1;
    for (i = 0; i < 4; i++) {
        if ((i & 1) == 0) {
            if (c < DAT_004ebb2c[i])
                return -1;
        } else {
            if (c < DAT_004ebb2c[i])
                return base + (c - DAT_004ebb2b[i]);
            base += DAT_004ebb2c[i] - DAT_004ebb2b[i];
        }
    }
    return base;
}

// OPEN: same shape as FUN_004136f0 (122 vs 130)
int FUN_00413870(const unsigned char *ch)
{
    unsigned short code;
    int base;
    unsigned int i;

    if ((ch[0] & 0xe0) != 0x80)
        return -1;
    code = (ch[0] << 8) | ch[1];
    base = 0;
    i = 0;
    do {
        if ((i & 1) == 0) {
            if (code < DAT_004ebb34[i])
                return -1;
        } else {
            if (code < DAT_004ebb34[i])
                return base + (code - DAT_004ebb32[i]);
            base += DAT_004ebb34[i] - DAT_004ebb32[i];
        }
        i++;
    } while (i < 0x48);
    return base;
}

// OPEN: original walks byte pointers into both palettes (dst+3, src+1); indexed version differs (100 vs 110)
void FUN_004147c0(PaletteTable *dst, const PaletteTable *src)
{
    int i;

    for (i = 0; i < 256; i++) {
        if (src->used[i] == 1) {
            if (dst == &DAT_00a4a890 &&
                (dst->entry[i].r != src->entry[i].r || dst->entry[i].g != src->entry[i].g || dst->entry[i].b != src->entry[i].b))
                DAT_00a4a88c = 1;
            dst->entry[i] = src->entry[i];
            dst->used[i] = 1;
        }
    }
}

// OPEN: size 960 vs 843: original caches mciSendString in a register (call ebp) and keeps name/channel in different registers; structure otherwise matches
/* Opens a wave file on an MCI channel and starts playing it; 1 on success. */
int FUN_004185a0(char *file, int channel, int from, int to, int repeat)
{
    char stackName[1024];
    char status[128];
    char command[128];
    char *heapName;
    size_t len;
    char *name;
    int tries;
    int attempt;
    int *state;

    len = strlen(file);
    heapName = NULL;
    if (*file == '"') {
        name = file;
    } else {
        if (len + 3 <= 1024) {
            name = stackName;
        } else {
            name = (char *)malloc(len + 3);
            if (name == NULL)
                return 0;
            heapName = name;
        }
        strcpy(name, "\"");
        strcat(name, file);
        strcat(name, "\"");
    }
    tries = 0;
    if (channel < 0)
        channel = 0;
    else if (channel >= 10)
        channel = 9;
    if (from >= 0) {
        if (from == 0)
            from = 1;
        if (to < 0)
            to = 0;
        if (repeat < 0)
            repeat = 0;
        state = &mciChannelOpen[channel];
        if (mciChannelOpen[channel] & 1)
            FUN_004188f0(channel);
        *state = 1;
        if (repeat > 0)
            *state = 3;
        sprintf(command, "Open %s Type WaveAudio Alias %s%d", name, "awf", channel);
        if (heapName != NULL)
            free(heapName);
        do {
            if (mciSendString(command, NULL, 0, DAT_00a4b0b0) == 0)
                break;
            tries++;
        } while (tries < 3);
        if (tries != 3) {
            sprintf(command, "Set %s%d Time Format Samples", "awf", channel);
            tries = 0;
            do {
                if (mciSendString(command, NULL, 0, DAT_00a4b0b0) == 0)
                    break;
                tries++;
            } while (tries < 3);
            if (tries != 3) {
                tries = 0;
                do {
                    if (to == 0)
                        sprintf(command, "Play %s%d From %d Notify", "awf", channel, from);
                    else
                        sprintf(command, "Play %s%d From %d To %d Notify", "awf", channel, from, to);
                    attempt = 0;
                    do {
                        if (mciSendString(command, NULL, 0, DAT_00a4b0b0) == 0)
                            break;
                        attempt++;
                    } while (attempt < 3);
                    if (attempt != 3) {
                        sprintf(command, "Status %s%d Mode", "awf", channel);
                        attempt = 0;
                        do {
                            if (mciSendString(command, status, 128, DAT_00a4b0b0) == 0)
                                break;
                            attempt++;
                        } while (attempt < 3);
                        if (attempt != 3 && strcmp(status, "playing") == 0)
                            break;
                    }
                    tries++;
                } while (tries < 3);
                if (tries == 3) {
                    FUN_004188f0(channel);
                    *state = 0;
                    return 0;
                }
                DAT_00a4b0e4[channel] = repeat;
                DAT_00a4b10c[channel] = to;
                return 1;
            }
        }
        FUN_004188f0(channel);
        *state = 0;
    }
    return 0;
}

// OPEN: size 372 vs 416: register allocation of the copy loops differs (width/keyTable spilled differently); 4 loop variants otherwise as in the original
/* Unscaled 8 bpp copy; flags 0x40/0x80 mirror horizontally/vertically, 0x400 skips pixels flagged in the key table. */
void FUN_00414830(BlitJob *job)
{
    unsigned int width = job->width;
    int *keyTable = job->keyTable;
    unsigned int flags = job->flags;
    unsigned char *src = job->src;
    unsigned char *dst = job->dst;
    unsigned int srcPitch = job->srcPitch;
    unsigned int rows = job->height;
    int dstSkip = job->dstPitch - width;
    int srcStep;
    int srcSkip;
    unsigned char *lineEnd;

    if ((flags & 0x40) == 0) {
        srcStep = -(int)width;
    } else {
        src += width - 1;
        srcStep = width;
    }
    if ((flags & 0x80) != 0) {
        src += (rows - 1) * srcPitch;
        srcPitch = -(int)srcPitch;
    }
    srcSkip = srcStep + srcPitch;
    if ((flags & 0x40) == 0) {
        if ((flags & 0x400) == 0) {
            if (rows != 0) {
                do {
                    lineEnd = src + width;
                    for (; src < lineEnd - 3; src += 4) {
                        *(unsigned int *)dst = *(unsigned int *)src;
                        dst += 4;
                    }
                    for (; src < lineEnd; src++) {
                        *dst = *src;
                        dst++;
                    }
                    src += srcSkip;
                    dst += dstSkip;
                    rows--;
                } while (rows != 0);
            }
        } else if (rows != 0) {
            do {
                lineEnd = src + width;
                for (; src < lineEnd; src++) {
                    if (keyTable[*src] != 1)
                        *dst = *src;
                    dst++;
                }
                src += srcSkip;
                dst += dstSkip;
                rows--;
            } while (rows != 0);
        }
    } else if ((flags & 0x400) == 0) {
        if (rows != 0) {
            do {
                lineEnd = src - width;
                for (; lineEnd < src; src--) {
                    *dst = *src;
                    dst++;
                }
                src += srcSkip;
                dst += dstSkip;
                rows--;
            } while (rows != 0);
        }
    } else if (rows != 0) {
        do {
            lineEnd = src - width;
            for (; lineEnd < src; src--) {
                if (keyTable[*src] != 1)
                    *dst = *src;
                dst++;
            }
            src += srcSkip;
            dst += dstSkip;
            rows--;
        } while (rows != 0);
    }
}

// OPEN: size 460 vs 511, frame 0x14 vs 0x18: the original keeps a second row-counter copy (local_4) in the unflipped path; loops otherwise as transcribed
/* Unscaled 8 bpp blend: dst = blend(src, dst, blendArg); flags as in FUN_00414830, 0x400 blends only where the key table is 0. */
void FUN_004149d0(BlitJob *job)
{
    unsigned int srcPitch = job->srcPitch;
    unsigned int flags = job->flags;
    unsigned char *src = job->src;
    unsigned int width = job->width;
    unsigned int rows = job->height;
    int *keyTable = job->keyTable;
    unsigned char *dst = job->dst;
    unsigned int arg = job->blendArg;
    unsigned char (*blend)(unsigned int, unsigned int, unsigned int) = job->blend;
    int dstSkip = job->dstPitch - width;
    int srcStep;
    int srcSkip;
    unsigned char *lineEnd;

    if ((flags & 0x40) == 0) {
        srcStep = -(int)width;
    } else {
        src += width - 1;
        srcStep = width;
    }
    if ((flags & 0x80) != 0) {
        src += (rows - 1) * srcPitch;
        srcPitch = -(int)srcPitch;
    }
    srcSkip = srcStep + srcPitch;
    if ((flags & 0x40) == 0) {
        if ((flags & 0x400) == 0) {
            if (rows != 0) {
                do {
                    lineEnd = src + width;
                    for (; src < lineEnd; src++) {
                        *dst = blend(*src, *dst, arg);
                        dst++;
                    }
                    src += srcSkip;
                    dst += dstSkip;
                    rows--;
                } while (rows != 0);
            }
        } else if (rows != 0) {
            do {
                lineEnd = src + width;
                for (; src < lineEnd; src++) {
                    if (keyTable[*src] == 0)
                        *dst = blend(*src, *dst, arg);
                    dst++;
                }
                src += srcSkip;
                dst += dstSkip;
                rows--;
            } while (rows != 0);
        }
    } else if ((flags & 0x400) == 0) {
        if (rows != 0) {
            do {
                lineEnd = src - width;
                for (; lineEnd < src; src--) {
                    *dst = blend(*src, *dst, arg);
                    dst++;
                }
                src += srcSkip;
                dst += dstSkip;
                rows--;
            } while (rows != 0);
        }
    } else if (rows != 0) {
        do {
            lineEnd = src - width;
            for (; lineEnd < src; src--) {
                if (keyTable[*src] == 0)
                    *dst = blend(*src, *dst, arg);
                dst++;
            }
            src += srcSkip;
            dst += dstSkip;
            rows--;
        } while (rows != 0);
    }
}

// OPEN: size 309 vs 252: the original keeps the pointer/int mix of the clip arithmetic in fewer temporaries (frame 0x14); my transcription of the clip tests allocates more
/* Copies the clipped rectangle of an 8 bpp surface into buf. */
void FUN_00416100(ClipJob *job)
{
    int baseW = job->baseW;
    int bufPitch = job->bufPitch;
    int *width = &job->width;
    unsigned char *base = job->base;
    int basePitch = job->basePitch;
    int baseH = job->baseH;
    int x = job->x;
    unsigned char *buf = job->buf;
    int y = job->y;
    unsigned char *bufEnd = buf + bufPitch * job->height;
    int w;
    unsigned int rowOffset;
    unsigned char *dstRow;
    unsigned char *s;
    unsigned char *d;

    if (y < 0) {
        if (job->height + y < 0)
            return;
        buf -= y * bufPitch;
        y = 0;
    }
    if (y < baseH) {
        w = *width;
        if (x < 0) {
            w = *width + x;
            if (w < 0)
                return;
            buf -= x;
            x = 0;
        }
        if (x < baseW) {
            rowOffset = (unsigned int)(base + y * basePitch);
            if (buf < bufEnd) {
                dstRow = (unsigned char *)(rowOffset + x);
                do {
                    if ((unsigned int)(base + basePitch * baseH) <= rowOffset)
                        return;
                    if (buf < buf + w) {
                        s = buf;
                        d = dstRow;
                        do {
                            if (dstRow + (baseW - x) <= d)
                                break;
                            *s = *d;
                            s++;
                            d++;
                        } while (s < buf + w);
                    }
                    buf += bufPitch;
                    rowOffset += basePitch;
                    dstRow += basePitch;
                } while (buf < bufEnd);
            }
        }
    }
}

// OPEN: size 221 vs 226: init loop matches; in the lookup part the original keeps the register for the index differently (esi) and does not merge the two return-0 tails
/* Single-byte character number index of the sheet as a string; also (first call) counts the single-byte glyphs. */
int FUN_00413780(int index, char *out, GlyphSheet *sheet)
{
    unsigned int i;
    unsigned int k;
    int *run;
    int base;
    int total;
    int sum;
    int next;
    char c;

    if (DAT_004ebb30 == 1) {
        sum = 0;
        k = 0;
        DAT_004ebb30 = 0;
        k = 0;
        do {
            if (k + 1 < 4) {
                DAT_00a4a868[k] = DAT_004ebb2d[k] - DAT_004ebb2c[k];
                if ((k & 1) == 0)
                    sum += DAT_00a4a868[k];
            } else {
                DAT_00a4a868[k] = 0;
            }
            k++;
        } while (k < 4);
        DAT_00a4a828 = sum;
        sheet->singleCount = sum;
    }
    sheet->singleCount = DAT_00a4a828;
    if (out == 0)
        return 0;
    i = 0;
    base = 0;
    run = DAT_00a4a868;
    total = 0;
    do {
        if ((i & 1) == 0) {
            next = total + *run;
            base = total;
        } else {
            next = total;
            if (index < total) {
                c = DAT_004ebb2b[i] + (char)(index - base);
                if (c == 0)
                    return 0;
                out[0] = c;
                out[1] = 0;
                return 1;
            }
        }
        run++;
        i++;
        total = next;
    } while (run < DAT_00a4a868 + 4);
    return 0;
}

// OPEN: size 234 vs 229: same shape as FUN_00413780; ushort range loop
/* Same for the double-byte characters. */
int FUN_00413900(int index, char *out, GlyphSheet *sheet)
{
    unsigned int i;
    int *run;
    const unsigned short *range;
    int base;
    int total;
    int next;
    short code;

    if (DAT_004ebbc4 == 1) {
        DAT_004ebbc4 = 0;
        DAT_00a4b09c = 0;
        run = DAT_00a4a700;
        range = DAT_004ebb34;
        i = 0;
        do {
            if (i + 1 < 0x48) {
                unsigned short hi = range[1];
                unsigned short lo = range[0];
                *run = hi - lo;
                if ((i & 1) == 0)
                    DAT_00a4b09c = DAT_00a4b09c + (hi - lo);
            } else {
                *run = 0;
            }
            range++;
            run++;
            i++;
        } while (run < DAT_00a4a700 + 0x48);
        sheet->doubleCount = DAT_00a4b09c;
    }
    sheet->doubleCount = DAT_00a4b09c;
    if (out == 0)
        return 0;
    base = 0;
    i = 0;
    run = DAT_00a4a700;
    total = 0;
    do {
        if ((i & 1) == 0) {
            next = total + *run;
            base = total;
        } else {
            next = total;
            if (index < total) {
                code = DAT_004ebb32[i] + ((short)index - (short)base);
                if (code == 0)
                    return 0;
                out[0] = (char)((unsigned short)code >> 8);
                out[1] = (char)code;
                out[2] = 0;
                return 1;
            }
        }
        run++;
        i++;
        total = next;
    } while (run < DAT_00a4a700 + 0x48);
    return 0;
}

// OPEN: size 460 vs 471, first diff at +0x3d: register allocation of count/cellH/font in the setup; declaration permutations of the locals gave no better score
GlyphSheet *FUN_00413430(unsigned int width, unsigned int height, LOGFONTA *font, COLORREF fg, COLORREF bg, int prefer, int fillBg, void *buf, DWORD *limit)
{
    GlyphSheet *sheet;
    int count;
    unsigned int cellH;
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
    cellH = font->lfHeight;
    count = sheet->doubleCount + sheet->singleCount;
    sheet->cellH = cellH;
    sheet->cellW = font->lfHeight;
    sheet->halfCellW = (sheet->cellW + 1) >> 1;
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
        if (i < sheet->singleCount)
            FUN_00413780(i, text, sheet);
        else
            FUN_00413900(i - sheet->singleCount, text, sheet);
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

// OPEN: size matches (1197); one scheduling wall (register/schedule class): in the 'Play %s%d From 1 Notify' branch the original hoists the hwnd load into edi before the retry loop (as in the Close branch); mine reloads it per iteration. for/do-while variants tried
/* Window procedure of the MCI notification window: restarts or closes finished devices, closes everything on destroy. */
LRESULT CALLBACK FUN_004180f0(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    char command[128];
    char status[128];
    int channel;
    int tries;
    int *state;
    long length;
    long position;

    switch (msg) {
    case MM_MCINOTIFY:
        if (mciCdOpen & 1) {
            for (tries = 0; tries < 3; tries++) {
                if (mciSendString("Status CDAudio Mode", status, 128, hwnd) == 0)
                    break;
            }
            if (strcmp(status, "stopped") == 0) {
                if (mciCdOpen & 2) {
                    if (DAT_00a4b160 == 0) {
                        sprintf(command, "Play CDAudio From %d Notify", DAT_00a4b15c);
                        for (tries = 0; tries < 3; tries++) {
                            if (mciSendString(command, NULL, 0, hwnd) == 0)
                                break;
                        }
                    } else {
                        sprintf(command, "Play CDAudio From %d To %d Notify", DAT_00a4b15c, DAT_00a4b160);
                        for (tries = 0; tries < 3; tries++) {
                            if (mciSendString(command, NULL, 0, hwnd) == 0)
                                break;
                        }
                    }
                } else {
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString("Close CDAudio", NULL, 0, hwnd) == 0)
                            break;
                    }
                    mciCdOpen = 0;
                }
            }
        }
        channel = 0;
        for (state = mciChannelOpen; state < mciChannelOpen + 10; channel++, state++) {
            if (*state & 1) {
                sprintf(command, "Status %s%d Mode", "awf", channel);
                for (tries = 0; tries < 3; tries++) {
                    if (mciSendString(command, status, 128, hwnd) == 0)
                        break;
                }
                if (strcmp(status, "stopped") != 0) {
                    sprintf(command, "Status %s%d Length", "awf", channel);
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString(command, status, 128, hwnd) == 0)
                            break;
                    }
                    length = strtol(status, NULL, 10);
                    sprintf(command, "Status %s%d Position", "awf", channel);
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString(command, status, 128, hwnd) == 0)
                            break;
                    }
                    position = strtol(status, NULL, 10);
                    if ((unsigned long)length > (unsigned long)position)
                        continue;
                    sprintf(command, "Stop %s%d", "awf", channel);
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString(command, NULL, 0, DAT_00a4b0b0) == 0)
                            break;
                    }
                    sprintf(command, "Close %s%d", "awf", channel);
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString(command, NULL, 0, DAT_00a4b0b0) == 0)
                            break;
                    }
                }
                if (*state & 2) {
                    sprintf(command, "Play %s%d From 1 Notify", "awf", channel);
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString(command, NULL, 0, hwnd) == 0)
                            break;
                    }
                } else {
                    sprintf(command, "Close %s%d", "awf", channel);
                    for (tries = 0; tries < 3; tries++) {
                        if (mciSendString(command, NULL, 0, hwnd) == 0)
                            break;
                    }
                    *state = 0;
                }
            }
        }
        break;
    case WM_DESTROY:
        if (mciCdOpen & 1) {
            for (tries = 0; tries < 3; tries++) {
                if (mciSendString("Stop CDAudio", NULL, 0, hwnd) == 0)
                    break;
            }
            for (tries = 0; tries < 3; tries++) {
                if (mciSendString("Close CDAudio", NULL, 0, hwnd) == 0)
                    break;
            }
            mciCdOpen = 0;
        }
        channel = 0;
        for (state = mciChannelOpen; state < mciChannelOpen + 10; channel++, state++) {
            if (*state & 1) {
                sprintf(command, "Stop %s%d", "awf", channel);
                for (tries = 0; tries < 3; tries++) {
                    if (mciSendString(command, NULL, 0, hwnd) == 0)
                        break;
                }
                sprintf(command, "Close %s%d", "awf", channel);
                for (tries = 0; tries < 3; tries++) {
                    if (mciSendString(command, NULL, 0, hwnd) == 0)
                        break;
                }
                *state = 0;
            }
        }
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}
