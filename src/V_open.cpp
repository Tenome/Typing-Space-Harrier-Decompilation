#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include "game.h"

// OPEN: 00413ad0 (BlitRequest clip-and-dispatch, 3267 B): complete transcription from the disassembly (clip of source and
// destination rectangles with 4-bit fixed-point proportional scaling, colour-key table, switch over the surface kinds);
// bytes do not match. See docs/reports/V.md for the size reached.
int FUN_00413ad0(BlitCommand *request)
{
    int keys[256];
    BlitJob job;
    DDSURFACEDESC desc;
    DDBLTFX fx;
    RECT srcRect;
    RECT dstRect;
    int both;
    int srcX, srcY, srcW, srcH;
    int dstX, dstY, dstW, dstH;
    int srcBufW, srcBufH, dstBufW, dstBufH;
    int newW, newH;
    int xEnd, yEnd;
    IDirectDrawSurface *surface;
    IDirectDrawSurface *srcSurface;
    IDirectDrawSurface *dstSurface;
    PaletteTable *palette;
    HRESULT hr;

    if (DAT_00a4a824 == 0) {
        if ((request->flags & 0xc) == 4 || (request->flags & 3) == 1)
            return 0;
    }
    if ((request->flags & 0xc) != 0 && (request->flags & 3) != 0)
        both = 1;
    else
        both = 0;

    if (request->flags & 0x10) {
        srcX = request->srcX;
        srcY = request->srcY;
        srcW = request->srcW;
        srcH = request->srcH;
    } else if ((request->flags & 0xc) == 0) {
        srcX = 0;
        srcY = 0;
        srcW = request->srcImage->width;
        srcH = request->srcImage->height;
    } else if ((request->flags & 0xc) == 4) {
        srcX = 0;
        srcY = 0;
        srcW = DAT_00a4a860;
        srcH = DAT_00a4b0a0;
    } else if ((request->flags & 0xc) == 8) {
        srcX = 0;
        srcY = 0;
        srcW = DAT_00a4a6f4;
        srcH = DAT_00a4b0a4;
    } else
        return 0;
    dstX = request->dstX;
    dstY = request->dstY;
    if ((request->flags & 0x20)) {
        dstW = request->dstW;
        dstH = request->dstH;
    } else {
        dstW = srcW;
        dstH = srcH;
    }
    if ((request->flags & 0xc) == 0) {
        srcBufW = request->srcImage->width;
        srcBufH = request->srcImage->height;
    } else if ((request->flags & 0xc) == 4) {
        srcBufW = DAT_00a4a860;
        srcBufH = DAT_00a4b0a0;
    } else if ((request->flags & 0xc) == 8) {
        srcBufW = DAT_00a4a6f4;
        srcBufH = DAT_00a4b0a4;
    } else
        return 0;
    if ((request->flags & 3) == 0) {
        dstBufW = request->dstImage->width;
        dstBufH = request->dstImage->height;
    } else if ((request->flags & 3) == 1) {
        dstBufW = DAT_00a4a860;
        dstBufH = DAT_00a4b0a0;
    } else if ((request->flags & 3) == 2) {
        dstBufW = DAT_00a4a6f4;
        dstBufH = DAT_00a4b0a4;
    } else
        return 0;

    if (srcW <= 0 || srcH <= 0 || dstW <= 0 || dstH <= 0)
        return 1;
    if (srcX >= srcBufW)
        return 1;
    xEnd = srcX + srcW;
    if (xEnd <= 0)
        return 1;
    if (srcY >= srcBufH)
        return 1;
    yEnd = srcY + srcH;
    if (yEnd <= 0)
        return 1;
    if (dstX >= dstBufW || dstX + dstW <= 0)
        return 1;
    if (dstY >= dstBufH || dstY + dstH <= 0)
        return 1;

    if (srcX < 0) {
        newW = ((xEnd << 4) * (dstW << 4) / (srcW << 4) + 15) >> 4;
        dstX += dstW - newW;
        dstW = newW;
        srcX = 0;
        srcW = xEnd;
    }
    if (srcY < 0) {
        newH = ((yEnd << 4) * (dstH << 4) / (srcH << 4) + 15) >> 4;
        dstY += dstH - newH;
        dstH = newH;
        srcY = 0;
        srcH = yEnd;
    }
    if (srcX + srcW > srcBufW) {
        newW = srcBufW - srcX;
        dstW = ((newW << 4) * (dstW << 4) / (srcW << 4) + 15) >> 4;
        srcW = newW;
    }
    if (srcY + srcH > srcBufH) {
        newH = srcBufH - srcY;
        dstH = ((newH << 4) * (dstH << 4) / (srcH << 4) + 15) >> 4;
        srcH = newH;
    }
    if (dstX < 0) {
        newW = dstX + dstW;
        dstX = 0;
        newH = ((newW << 4) * (srcW << 4) / (dstW << 4) + 15) >> 4;
        if ((request->flags & 0x40) == 0 || both == 1)
            srcX += srcW - newH;
        dstW = newW;
        srcW = newH;
    }
    if (dstY < 0) {
        newW = dstY + dstH;
        dstY = 0;
        newH = ((newW << 4) * (srcH << 4) / (dstH << 4) + 15) >> 4;
        if ((request->flags & 0x80) == 0 || both == 1)
            srcY += srcH - newH;
        dstH = newW;
        srcH = newH;
    }
    if (dstX + dstW > dstBufW) {
        newW = dstBufW - dstX;
        newH = ((newW << 4) * (srcW << 4) / (dstW << 4) + 15) >> 4;
        if ((request->flags & 0x40) && both == 0)
            srcX += srcW - newH;
        dstW = newW;
        srcW = newH;
    }
    if (dstY + dstH > dstBufH) {
        newW = dstBufH - dstY;
        newH = ((newW << 4) * (srcH << 4) / (dstH << 4) + 15) >> 4;
        if ((request->flags & 0x80) && both == 0)
            srcY += srcH - newH;
        dstH = newW;
        srcH = newH;
    }
    if (srcW <= 0 || srcH <= 0 || dstW <= 0 || dstH <= 0)
        return 1;
    if (srcX >= srcBufW)
        return 1;
    xEnd = srcX + srcW;
    if (xEnd <= 0)
        return 1;
    if (srcY >= srcBufH)
        return 1;
    yEnd = srcY + srcH;
    if (yEnd <= 0)
        return 1;
    if (dstX >= dstBufW || dstX + dstW <= 0)
        return 1;
    if (dstY >= dstBufH || dstY + dstH <= 0)
        return 1;

    if (request->flags & 0x400) {
        palette = 0;
        if ((request->flags & 0xc) == 0) {
            if (request->srcImage->bpp == 8)
                palette = request->srcImage->palette;
        } else if (((request->flags & 0xc) == 4 && DAT_00a4a820 == 8) || ((request->flags & 0xc) == 8 && DAT_00a4a6f0 == 8)) {
            palette = &DAT_00a4a890;
        }
        if (palette != 0) {
            unsigned char lowR = (unsigned char)request->keyLow;
            unsigned char lowG = (unsigned char)(request->keyLow >> 8);
            unsigned char lowB = (unsigned char)(request->keyLow >> 16);
            unsigned char highR = (unsigned char)request->keyHigh;
            unsigned char highG = (unsigned char)(request->keyHigh >> 8);
            unsigned char highB = (unsigned char)(request->keyHigh >> 16);
            int i;

            for (i = 0; i < 256; i++) {
                if (palette->used[i] == 1
                    && palette->entry[i].r >= lowR && palette->entry[i].r <= highR
                    && palette->entry[i].g >= lowG && palette->entry[i].g <= highG
                    && palette->entry[i].b >= lowB && palette->entry[i].b <= highB)
                    keys[i] = 1;
                else
                    keys[i] = 0;
            }
        }
    }

    job.flags = request->flags;
    job.keyTable = keys;
    job.blendArg = request->blendArg;
    job.blend = request->blend;
    switch (request->flags & 0xf) {
    case 0:
        if (request->srcImage->bpp == 8) {
            job.width = srcW;
            job.height = srcH;
            job.dstWidth = dstW;
            job.m_18 = dstH;
            job.srcPitch = srcBufW;
            job.dstPitch = dstBufW;
            job.src = request->srcImage->pixels + srcBufW * srcY + srcX;
            job.dst = request->dstImage->pixels + dstBufW * dstY + dstX;
            if ((request->flags & 0x20)) {
                if (request->flags & 0x200)
                    FUN_00414f00(&job);
                else
                    FUN_00414bd0(&job);
            } else {
                if (request->flags & 0x200)
                    FUN_004149d0(&job);
                else
                    FUN_00414830(&job);
            }
            FUN_004147c0(request->dstImage->palette, request->srcImage->palette);
            return 1;
        }
        if (request->srcImage->bpp == 0x10 || request->srcImage->bpp == 0x18 || request->srcImage->bpp == 0x20)
            return 1;
        return 0;
    case 1:
    case 2:
        if (request->srcImage->bpp == 8) {
            job.width = srcW;
            job.height = srcH;
            job.dstWidth = dstW;
            job.m_18 = dstH;
            job.srcPitch = srcBufW;
            job.src = request->srcImage->pixels + srcBufW * srcY + srcX;
            surface = (request->flags & 3) == 1 ? DAT_00a4a824 : DAT_00a4a864;
            desc.dwSize = sizeof(desc);
            while (1) {
                hr = surface->Lock(NULL, &desc, 0, NULL);
                if (hr == DD_OK)
                    break;
                if (hr == DDERR_SURFACELOST)
                    return 0;
                if (hr != DDERR_WASSTILLDRAWING)
                    return 0;
            }
            job.dstPitch = desc.lPitch;
            job.dst = (unsigned char *)desc.lpSurface + desc.lPitch * dstY + dstX;
            if ((request->flags & 0x20)) {
                if (request->flags & 0x200)
                    FUN_00414f00(&job);
                else
                    FUN_00414bd0(&job);
            } else {
                if (request->flags & 0x200)
                    FUN_004149d0(&job);
                else
                    FUN_00414830(&job);
            }
            surface->Unlock(NULL);
            FUN_004147c0(&DAT_00a4a890, request->srcImage->palette);
            return 1;
        }
        if (request->srcImage->bpp == 0x10 || request->srcImage->bpp == 0x18 || request->srcImage->bpp == 0x20)
            return 1;
        return 0;
    case 4:
    case 8:
        if (((request->flags & 0xc) == 4 && DAT_00a4a820 == 8) || ((request->flags & 0xc) == 8 && DAT_00a4a6f0 == 8)) {
            job.width = srcW;
            job.height = srcH;
            job.dstWidth = dstW;
            job.m_18 = dstH;
            job.dstPitch = dstBufW;
            job.dst = request->dstImage->pixels + dstBufW * dstY + dstX;
            surface = (request->flags & 0xc) == 4 ? DAT_00a4a824 : DAT_00a4a864;
            desc.dwSize = sizeof(desc);
            while (1) {
                hr = surface->Lock(NULL, &desc, 0, NULL);
                if (hr == DD_OK)
                    break;
                if (hr == DDERR_SURFACELOST)
                    return 0;
                if (hr != DDERR_WASSTILLDRAWING)
                    return 0;
            }
            job.srcPitch = desc.lPitch;
            job.src = (unsigned char *)desc.lpSurface + desc.lPitch * srcY + srcX;
            if ((request->flags & 0x20)) {
                if (request->flags & 0x200)
                    FUN_00414f00(&job);
                else
                    FUN_00414bd0(&job);
            } else {
                if (request->flags & 0x200)
                    FUN_004149d0(&job);
                else
                    FUN_00414830(&job);
            }
            surface->Unlock(NULL);
            FUN_004147c0(request->dstImage->palette, &DAT_00a4a890);
            return 1;
        }
        if (request->srcImage->bpp == 0x10 || request->srcImage->bpp == 0x18 || request->srcImage->bpp == 0x20)
            return 1;
        return 0;
    case 5:
    case 6:
    case 9:
    case 10:
        if ((request->flags & 0xc) == 4)
            srcSurface = DAT_00a4a824;
        else if ((request->flags & 0xc) == 8)
            srcSurface = DAT_00a4a864;
        if ((request->flags & 3) == 1)
            dstSurface = DAT_00a4a824;
        else if ((request->flags & 3) == 2)
            dstSurface = DAT_00a4a864;
        if (srcSurface == 0 || dstSurface == 0)
            return 0;
        srcRect.left = srcX;
        srcRect.top = srcY;
        srcRect.right = xEnd;
        srcRect.bottom = yEnd;
        if (request->flags & 0x3e0) {
            unsigned long bltFlags = DDBLT_WAIT;

            dstRect.left = dstX;
            dstRect.top = dstY;
            dstRect.right = dstX + dstW;
            dstRect.bottom = dstY + dstH;
            memset(&fx, 0, sizeof(fx));
            fx.dwSize = sizeof(fx);
            if (request->flags & 0x40) {
                fx.dwDDFX |= DDBLTFX_MIRRORLEFTRIGHT;
                bltFlags = DDBLT_WAIT | DDBLT_DDFX;
            }
            if (request->flags & 0x80) {
                fx.dwDDFX |= DDBLTFX_MIRRORUPDOWN;
                bltFlags |= DDBLT_DDFX;
            }
            if (request->flags & 0x100) {
                fx.dwRotationAngle = request->angle;
                bltFlags |= DDBLT_ROTATIONANGLE | DDBLT_DDFX;
            }
            if (request->flags & 0x400) {
                bltFlags |= DDBLT_KEYSRC;
                if (request->keyLow != DAT_00a4a888 || request->keyHigh != DAT_00a4b098) {
                    unsigned long rgbLow, rgbHigh;
                    int indexLow, indexHigh;

                    DAT_00a4a888 = request->keyLow;
                    DAT_00a4b098 = request->keyHigh;
                    FUN_00412fe0(request->keyLow, request->keyHigh, 1, &rgbLow, &rgbHigh, &indexLow, &indexHigh);
                    DAT_00a4a880 = indexLow;
                    DAT_00a4a884 = indexHigh;
                    if (DAT_00a4a824->SetColorKey(DDCKEY_SRCBLT, (DDCOLORKEY *)&DAT_00a4a880) != 0)
                        return 0;
                }
            }
            hr = dstSurface->Blt(&dstRect, srcSurface, &srcRect, bltFlags, &fx);
        } else {
            unsigned long transFlags = 0;

            if (request->flags & 0x400) {
                if (request->keyLow != DAT_00a4a888 || request->keyHigh != DAT_00a4b098) {
                    unsigned long rgbLow, rgbHigh;
                    int indexLow, indexHigh;

                    DAT_00a4a888 = request->keyLow;
                    DAT_00a4b098 = request->keyHigh;
                    FUN_00412fe0(request->keyLow, request->keyHigh, 1, &rgbLow, &rgbHigh, &indexLow, &indexHigh);
                    DAT_00a4a880 = indexLow;
                    DAT_00a4a884 = indexHigh;
                    if (DAT_00a4a824->SetColorKey(DDCKEY_SRCBLT, (DDCOLORKEY *)&DAT_00a4a880) != 0)
                        return 0;
                }
                transFlags = DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT;
            }
            hr = dstSurface->BltFast(dstX, dstY, srcSurface, &srcRect, transFlags);
        }
        if (hr != 0)
            return 0;
        return 1;
    default:
        return 0;
    }
}

// OPEN: 0x417530 (447 = 447 B, 87 %) and 0x4176f0 (keyed twin): wrapped blits whose rows are shifted and stretched by a table of 12-byte records.
// Same bytes except the order of the prologue loads / spill slots and the x-wrap register choice; see docs/reports/V.md.

void FUN_00417530(DistortJob *job)
{
    int baseW = job->baseW;
    unsigned char *buf = job->buf;
    int baseH = job->baseH;
    int x = job->x;
    unsigned char *base = job->base;
    int basePitch = job->basePitch;
    int bufPitch = job->bufPitch;
    int rows = job->rows;
    DistortRow *row = job->table;
    int width = job->width;
    int y = job->y;
    int wrapW = baseW << 16;
    int widthFixed = width << 16;
    DistortRow *end;
    int step;
    unsigned char *dstRow;
    unsigned char *dst;
    unsigned char *dstEnd;
    int sy;
    int fx;
    int sx;
    unsigned char *srcRow;

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

void FUN_004176f0(DistortJob *job)
{
    int baseW = job->baseW;
    unsigned char *buf = job->buf;
    int baseH = job->baseH;
    int x = job->x;
    unsigned char *base = job->base;
    int basePitch = job->basePitch;
    int bufPitch = job->bufPitch;
    int rows = job->rows;
    int *keys = job->keys;
    DistortRow *row = job->table;
    int width = job->width;
    int y = job->y;
    int wrapW = baseW << 16;
    int widthFixed = width << 16;
    DistortRow *end;
    int step;
    unsigned char c;
    unsigned char *dstRow;
    unsigned char *dst;
    unsigned char *dstEnd;
    int sy;
    int fx;
    int sx;
    unsigned char *srcRow;

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
            c = srcRow[fx >> 16];
            if (keys[c] == 0)
                *dst = c;
            fx += step;
        }
    }
}
