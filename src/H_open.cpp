#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <string.h>
#include "game.h"

// OPEN: 0x410ea0 original builds a pointer (lea edx,[esi+0x18]) to the 5 group slots before the vtable store; mine addresses [esi+k] directly (class wall: register/schedule)
NodeList::NodeList()
{
    head = 0;
    group.Clear();
    m_2c = 0;
}

// OPEN: 0x410f40 same unrolled 5-slot clear: original advances edi by 0x18, mine uses lea eax (register/schedule)
void NodeList::FUN_00410f40()
{
    ListNode *node;
    ListNode *following;

    node = head;
    while (node) {
        following = node->next;
        if (node)
            delete node;
        node = following;
    }
    head = 0;
    group.Clear();
}

// OPEN: 0x412800 loop head is emitted once with a back jump (jne end; jmp top) in the original; every for(;;)/while/do-while/return form either duplicates the head or drops the first test eax,eax (call-set/CFG class; same wall as 413280 in lane D)
int FUN_00412800()
{
    int ok;
    HRESULT result;

    if (DAT_00a4b0a8 == 0)
        return 0;
    ok = 1;
    if (DAT_00a4a87c != 0 && DAT_00a4a88c == 1) {
        if (DAT_00a4a87c->SetEntries(0, 0, 256, (PALETTEENTRY *)&DAT_00a4a890) != 0)
            return 0;
        DAT_00a4a88c = 0;
    }
    do {
        result = DAT_00a4b094->Flip(0, 0);
        if (result == 0)
            break;
        if (result == DDERR_SURFACELOST) {
            ok = 0;
            break;
        }
    } while (result == DDERR_WASSTILLDRAWING);
    return ok;
}

// OPEN: 0x4119a0 identical except register naming in the final write-mode tail (original path in edx, tail value in ecx; mine eax/edx); goto-shared tail, param-slot reuse for 'exists' already works (register/schedule)
int FUN_004119a0(StreamRec *stream, int isFile, int reading, int append, char *source, unsigned int size)
{
    int exists;

    stream->m_00 = isFile;
    stream->m_04 = reading;
    if (reading == 1) {
        if (isFile == 1) {
            FUN_00411930(source, &exists, (unsigned long *)&size);
            if (exists == 0)
                return 0;
            stream->m_08 = fopen(source, "rb");
            if (stream->m_08 == 0)
                return 0;
            stream->m_0c = 0;
        } else {
            stream->m_0c = (unsigned char *)source;
            stream->m_08 = 0;
        }
        stream->m_14 = 0;
        stream->m_10 = 0;
        stream->m_18 = size;
        stream->m_1c = size;
        return 1;
    }
    if (append == 1) {
        if (isFile == 1) {
            FUN_00411930(source, &exists, (unsigned long *)&size);
            if (exists == 1) {
                stream->m_08 = fopen(source, "r+b");
                if (stream->m_08 == 0)
                    return 0;
                stream->m_14 = 0;
                stream->m_10 = 0;
                stream->m_0c = 0;
                stream->m_18 = size;
                stream->m_1c = size;
                return 1;
            }
            stream->m_08 = fopen(source, "wb");
            if (stream->m_08 == 0)
                return 0;
            stream->m_14 = 0;
            stream->m_10 = 0;
            stream->m_18 = 0;
            stream->m_0c = 0;
            stream->m_1c = size;
            return 1;
        }
    } else if (isFile == 1) {
        stream->m_08 = fopen(source, "wb");
        if (stream->m_08 == 0)
            return 0;
        stream->m_0c = 0;
        goto finish;
    }
    stream->m_0c = (unsigned char *)source;
    stream->m_08 = 0;
finish:
    stream->m_14 = 0;
    stream->m_10 = 0;
    stream->m_18 = 0;
    stream->m_1c = size;
    return 1;
}

// OPEN: 0x411e80 443 vs 435 bytes. Everything up to the final read check matches; the original lays out [close B][fail: close A; return 0][success] with early failures jumping to the middle return, VC6 here inlines the success block first (CFG/layout class). Tried goto/if inversions.
int FUN_00411e80(unsigned int flags, char *source, unsigned int size, char *dest, unsigned int *destSize, int skip)
{
    StreamHeader header;
    unsigned int limit;
    int start;
    int i;

    if ((flags & 2) == 0) {
        if (FUN_004119a0(&DAT_00a4a680, 0, 1, 0, source, size) == 0)
            goto done;
    } else {
        if (FUN_004119a0(&DAT_00a4a680, 1, 1, 0, source, size) == 0)
            goto done;
    }
    for (i = 0; i < skip; i++) {
        if (FUN_00411b50(&DAT_00a4a680) == 0)
            goto fail;
    }
    start = FUN_00411cd0(&DAT_00a4a680);
    if (FUN_00411b50(&DAT_00a4a680) == 0)
        goto fail;
    if (FUN_00412040(FUN_00411cd0(&DAT_00a4a680), &DAT_00a4a680) == 0)
        goto fail;
    if (FUN_00411bf0(&DAT_00a4a680, start, 1) != (unsigned int)start)
        goto fail;
    if ((flags & 1) == 0) {
        if (destSize == 0)
            goto fail;
        if (FUN_004119a0(&DAT_00a4a6a0, 0, 0, 0, dest, *destSize) == 0)
            goto fail;
        limit = *destSize;
    } else {
        limit = destSize == 0 ? 0 : *destSize;
        if (FUN_004119a0(&DAT_00a4a6a0, 1, 0, 0, dest, limit) == 0)
            goto fail;
    }
    if (FUN_00411ce0((unsigned char *)&header, sizeof(header), &DAT_00a4a680) == sizeof(header)
        && ((flags & 1) != 0 || (unsigned int)header.length <= limit))
        goto finished;
    FUN_00411b20(&DAT_00a4a6a0);
fail:
    FUN_00411b20(&DAT_00a4a680);
done:
    return 0;
finished:
    FUN_00412060();
    if (destSize != 0)
        *destSize = FUN_00411da0(&DAT_00a4a6a0);
    FUN_00411b20(&DAT_00a4a680);
    FUN_00411b20(&DAT_00a4a6a0);
    return 1;
}

// OPEN: 0x412400 818 vs 816 bytes. All calls/fields match; residue: (1) original hoists mov eax,1 before the prologue pushes and compares ecx,eax; (2) palette clear loop: original induction variable starts at the table address (eax=0xa4a890, cmp 0xa4ac90, signed jl), mine uses index with absolute displacements; (3) final colour-key part uses edi/esi where mine uses ebp (register/schedule)
int FUN_00412400(void *window, int width, int height, int bpp, int coopFlags, unsigned int offWidth, unsigned int offHeight, unsigned long keyHigh, unsigned long keyLow)
{
    DDSURFACEDESC desc;
    DDSCAPS caps;
    DDCOLORKEY key;
    unsigned long highValue;
    unsigned long lowValue;
    int ok = 1;
    int i;

    if (DAT_00a4b0a8 != ok) {
        DAT_00a4a6fc = 0;
        DAT_00a4b094 = 0;
        DAT_00a4a864 = 0;
        DAT_00a4a824 = 0;
        DAT_00a4a87c = 0;
        PaletteChangeHook = 0;
        if (DirectDrawCreate(0, &DAT_00a4a6fc, 0) != 0) {
            FUN_00412730();
            return 0;
        }
        DAT_00a4a6f8 = (int)window;
        if (DAT_00a4a6fc->SetCooperativeLevel((HWND)window, coopFlags) != 0) {
            FUN_00412730();
            return 0;
        }
        DAT_00a4b090 = coopFlags;
        if (DAT_00a4a6fc->SetDisplayMode(width, height, bpp) != 0) {
            FUN_00412730();
            return 0;
        }
        memset(&desc, 0, sizeof(desc));
        desc.dwSize = sizeof(desc);
        desc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
        desc.dwBackBufferCount = 1;
        if (DAT_00a4a6fc->CreateSurface(&desc, &DAT_00a4b094, 0) != 0) {
            FUN_00412730();
            return 0;
        }
        caps.dwCaps = DDSCAPS_BACKBUFFER;
        if (DAT_00a4b094->GetAttachedSurface(&caps, &DAT_00a4a864) != 0) {
            FUN_00412730();
            return 0;
        }
        DAT_00a4a6f4 = width;
        DAT_00a4b0a4 = height;
        DAT_00a4a6f0 = bpp;
        if (bpp != 8) {
            DAT_00a4a87c = 0;
        } else {
            DAT_00a4a88c = 0;
            for (i = 0; i < 256; i++) {
                DAT_00a4a890.used[i] = 0;
                DAT_00a4a890.entry[i].r = 0;
                DAT_00a4a890.entry[i].g = 0;
                DAT_00a4a890.entry[i].b = 0;
                DAT_00a4a890.entry[i].flags = 0;
            }
            if (DAT_00a4a6fc->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, (PALETTEENTRY *)DAT_00a4a890.entry, &DAT_00a4a87c, 0) != 0) {
                FUN_00412730();
                return 0;
            }
            if (DAT_00a4a87c != 0 && DAT_00a4b094->SetPalette(DAT_00a4a87c) != 0) {
                FUN_00412730();
                return 0;
            }
        }
        if (offWidth > 0 && offHeight > 0) {
            memset(&desc, 0, sizeof(desc));
            desc.dwSize = sizeof(desc);
            desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
            desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
            desc.dwWidth = offWidth;
            desc.dwHeight = offHeight;
            if (DAT_00a4a6fc->CreateSurface(&desc, &DAT_00a4a824, 0) != 0) {
                FUN_00412730();
                return 0;
            }
            DAT_00a4a860 = offWidth;
            DAT_00a4a820 = bpp;
            DAT_00a4b0a0 = offHeight;
        } else {
            DAT_00a4a824 = 0;
        }
        DAT_00a4a888 = keyLow;
        DAT_00a4b098 = keyHigh;
        if (DAT_00a4a824 != 0) {
            highValue = FUN_00412880(DAT_00a4a824, keyHigh);
            lowValue = highValue;
            if (keyHigh != keyLow)
                lowValue = FUN_00412880(DAT_00a4a824, keyLow);
            key.dwColorSpaceLowValue = lowValue;
            key.dwColorSpaceHighValue = highValue;
            if (DAT_00a4a824->SetColorKey(DDCKEY_SRCBLT, &key) != 0) {
                FUN_00412730();
                return 0;
            }
            DAT_00a4a880 = key.dwColorSpaceLowValue;
            DAT_00a4a884 = key.dwColorSpaceHighValue;
        }
        DAT_00a4b0a8 = ok;
    }
    return ok;
}

// OPEN: 0x4129a0 (BMP -> SheetImage loader) 497 vs 489 bytes. Original keeps the image pointer in ebp, reuses the dead 'bufferStart' parameter slot for the palette-entry count and a separate 0x30 frame, reads biBitCount as a dword masked with 0xffff (not cx); structure otherwise matches (register/schedule + param-slot reuse). Note lane J declares this function as int(char*,uint,uint*).
/* Builds a SheetImage (header, palette, bottom-up flipped pixels) from a BMP file image inside a caller-supplied buffer. */
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

// OPEN: 0x4114d0 (WAV loader, SoundLoader = SoundMgr + method) 898 vs 914 bytes. Flow, DS/mmio calls and param-slot reuse (buffer in the index slot, len1 in the path slot) are right. Original keeps the slot pointers (edi=&buffers[index], ebp=&loops[index]) and GlobalLock import in ebp, has no hoisted zero register, and duplicates the close/unlock/unlock/ret 8 tails (register/schedule + tail duplication).
int SoundLoader::M_004114d0(const char *path, int index)
{
    LPDIRECTSOUNDBUFFER *buffers;
    LPDIRECTSOUNDBUFFER buffer;
    void **loops;
    HMMIO file;
    MMCKINFO riff;
    MMCKINFO chunk;
    int format[4];
    DSBUFFERDESC desc;
    void *ptr1;
    void *ptr2;
    DWORD len1;
    DWORD len2;
    char *data;

    if (m_00 == 0 || m_08 == 0 || index < 0 || index >= m_04)
        return 0;
    buffers = (LPDIRECTSOUNDBUFFER *)GlobalLock(m_08);
    buffer = buffers[index];
    if (buffer != 0) {
        buffer->Stop();
        buffer->Release();
        buffers[index] = 0;
    }
    loops = (void **)GlobalLock(m_14);
    if (loops[index] != 0) {
        GlobalFree(loops[index]);
        loops[index] = 0;
    }
    file = mmioOpenA((LPSTR)path, 0, 0);
    if (file != 0) {
        riff.ckid = 0;
        riff.cksize = 0;
        riff.fccType = 0x45564157;
        riff.dwDataOffset = 0;
        riff.dwFlags = 0;
        if (mmioDescend(file, &riff, 0, MMIO_FINDRIFF) != 0)
            goto fail;
        chunk.cksize = 0;
        chunk.fccType = 0;
        chunk.dwDataOffset = 0;
        chunk.dwFlags = 0;
        chunk.ckid = 0x20746d66;
        if (mmioDescend(file, &chunk, &riff, MMIO_FINDCHUNK) == 0) {
            format[0] = 0;
            format[1] = 0;
            format[2] = 0;
            format[3] = 0;
            if (mmioRead(file, (HPSTR)format, 0x10) != 0x10)
                goto fail;
            mmioAscend(file, &chunk, 0);
            chunk.cksize = 0;
            chunk.fccType = 0;
            chunk.dwDataOffset = 0;
            chunk.dwFlags = 0;
            chunk.ckid = 0x61746164;
            if (mmioDescend(file, &chunk, &riff, MMIO_FINDCHUNK) != 0)
                goto fail;
            desc.dwReserved = 0;
            desc.dwBufferBytes = chunk.cksize;
            desc.dwSize = 0x14;
            desc.dwFlags = 0xa2;
            desc.lpwfxFormat = (LPWAVEFORMATEX)format;
            if (m_00->CreateSoundBuffer(&desc, &buffer, 0) == 0) {
                if (buffer->Lock(0, chunk.cksize, &ptr1, &len1, &ptr2, &len2, 0) != 0) {
                    mmioClose(file, 0);
                    GlobalUnlock(m_14);
                    GlobalUnlock(m_08);
                    buffer->Release();
                    buffers[index] = 0;
                    return 0;
                }
                if (chunk.cksize > len1) {
                    mmioRead(file, (HPSTR)ptr1, len1);
                    mmioRead(file, (HPSTR)ptr2, chunk.cksize - len1);
                } else {
                    mmioRead(file, (HPSTR)ptr1, chunk.cksize);
                }
                buffer->Unlock(ptr1, len1, ptr2, len2);
                buffers[index] = buffer;
                mmioAscend(file, &chunk, 0);
                chunk.cksize = 0;
                chunk.fccType = 0;
                chunk.dwDataOffset = 0;
                chunk.dwFlags = 0;
                chunk.ckid = 0x2070696c;
                if (mmioDescend(file, &chunk, &riff, MMIO_FINDCHUNK) == 0) {
                    data = (char *)GlobalAlloc(GPTR, chunk.cksize);
                    if (data == 0)
                        goto fail;
                    memset(data, 0, chunk.cksize);
                    mmioRead(file, data, chunk.cksize);
                    loops[index] = data;
                }
                mmioAscend(file, &chunk, 0);
                mmioClose(file, 0);
                GlobalUnlock(m_14);
                GlobalUnlock(m_08);
                return 1;
            }
        }
        mmioClose(file, 0);
    }
    GlobalUnlock(m_14);
    GlobalUnlock(m_08);
    return 0;
fail:
    mmioClose(file, 0);
    GlobalUnlock(m_14);
    GlobalUnlock(m_08);
    return 0;
}

// OPEN: 0x414bd0 873 vs 812 bytes. All four Bresenham variants transcribed (flags 0x40 mirror, 0x400 key skip); the original spills/loads the BlitJob fields in a different order (flags&0x40 evaluated once into esi, height and dst width saved to the stack) and shares more increments (register/schedule). Same family as 414830/4149d0 in lane D.
/* Scaled 8 bpp copy (Bresenham in both directions); flags 0x40/0x80 mirror, 0x400 skips pixels flagged 1 in the key table. */
void FUN_00414bd0(BlitJob *job)
{
    unsigned int srcH = job->height;
    unsigned int flags = job->flags;
    unsigned int dstW = job->dstWidth;
    unsigned char *src = job->src;
    unsigned char *dst = job->dst;
    unsigned int srcW = job->width;
    int *keyTable = job->keyTable;
    unsigned int srcPitch = job->srcPitch;
    int dstH2 = job->m_18 * 2;
    int dstSkip = job->dstPitch - dstW;
    int srcStep;
    int srcSkip;
    int rowsLeft;
    int yErr;
    int xErr;
    int step;
    unsigned char *rowEnd;
    unsigned char *p;

    if ((flags & 0x40) == 0) {
        srcStep = -(int)srcW;
    } else {
        src += srcW - 1;
        srcStep = srcW;
    }
    if ((flags & 0x80) != 0) {
        src += (srcH - 1) * srcPitch;
        srcPitch = -(int)srcPitch;
    }
    srcSkip = srcStep + srcPitch;
    rowsLeft = srcH;
    yErr = dstH2;
    if ((flags & 0x40) == 0) {
        if ((flags & 0x400) == 0) {
            while (0 < rowsLeft) {
                rowEnd = dst + dstW;
                step = dstW * 2;
                p = src;
                if (yErr < (int)srcH) {
                    src += srcW + srcSkip;
                    rowsLeft--;
                    step = dstH2;
                } else {
                    while (p < src + srcW && dst < rowEnd) {
                        if (step < (int)srcW) {
                            p++;
                            xErr = dstW * 2;
                        } else {
                            *dst = *p;
                            dst++;
                            xErr = srcW * -2;
                        }
                        step += xErr;
                    }
                    dst = rowEnd + dstSkip;
                    step = srcH * -2;
                }
                yErr += step;
            }
        } else {
            while (0 < rowsLeft) {
                rowEnd = dst + dstW;
                step = dstW * 2;
                p = src;
                if (yErr < (int)srcH) {
                    src += srcW + srcSkip;
                    rowsLeft--;
                    step = dstH2;
                } else {
                    while (p < src + srcW && dst < rowEnd) {
                        if (step < (int)srcW) {
                            p++;
                            xErr = dstW * 2;
                        } else {
                            if (keyTable[*p] != 1)
                                *dst = *p;
                            dst++;
                            xErr = srcW * -2;
                        }
                        step += xErr;
                    }
                    dst = rowEnd + dstSkip;
                    step = srcH * -2;
                }
                yErr += step;
            }
        }
    } else if ((flags & 0x400) == 0) {
        while (0 < rowsLeft) {
            rowEnd = dst + dstW;
            step = dstW * 2;
            p = src;
            if (yErr < (int)srcH) {
                src += -srcW + srcSkip;
                rowsLeft--;
                step = dstH2;
            } else {
                while (src - srcW < p && dst < rowEnd) {
                    if (step < (int)srcW) {
                        p--;
                        xErr = dstW * 2;
                    } else {
                        *dst = *p;
                        dst++;
                        xErr = srcW * -2;
                    }
                    step += xErr;
                }
                dst = rowEnd + dstSkip;
                step = srcH * -2;
            }
            yErr += step;
        }
    } else {
        while (0 < rowsLeft) {
            rowEnd = dst + dstW;
            step = dstW * 2;
            p = src;
            if (yErr < (int)srcH) {
                src += -srcW + srcSkip;
                rowsLeft--;
                step = dstH2;
            } else {
                while (src - srcW < p && dst < rowEnd) {
                    if (step < (int)srcW) {
                        p--;
                        xErr = dstW * 2;
                    } else {
                        if (keyTable[*p] != 1)
                            *dst = *p;
                        dst++;
                        xErr = srcW * -2;
                    }
                    step += xErr;
                }
                dst = rowEnd + dstSkip;
                step = srcH * -2;
            }
            yErr += step;
        }
    }
}

// OPEN: 0x414f00 first draft, not compared line by line: blending sibling of 414bd0 (dst = blend(src, dst, arg); key table 0 = blend). Same family and expected walls as 414bd0; size differs.
/* Scaled 8 bpp blend; same loops as FUN_00414bd0. */
void FUN_00414f00(BlitJob *job)
{
    unsigned int srcH = job->height;
    unsigned int flags = job->flags;
    unsigned int dstW = job->dstWidth;
    unsigned char *src = job->src;
    unsigned char *dst = job->dst;
    unsigned int srcW = job->width;
    int *keyTable = job->keyTable;
    unsigned int arg = job->blendArg;
    unsigned char (*blend)(unsigned int, unsigned int, unsigned int) = job->blend;
    unsigned int srcPitch = job->srcPitch;
    int dstH2 = job->m_18 * 2;
    int dstSkip = job->dstPitch - dstW;
    int srcStep;
    int srcSkip;
    int rowsLeft;
    int yErr;
    int xErr;
    int step;
    unsigned char *rowEnd;
    unsigned char *p;

    if ((flags & 0x40) == 0) {
        srcStep = -(int)srcW;
    } else {
        src += srcW - 1;
        srcStep = srcW;
    }
    if ((flags & 0x80) != 0) {
        src += (srcH - 1) * srcPitch;
        srcPitch = -(int)srcPitch;
    }
    srcSkip = srcStep + srcPitch;
    rowsLeft = srcH;
    yErr = dstH2;
    if ((flags & 0x40) == 0) {
        if ((flags & 0x400) == 0) {
            while (0 < rowsLeft) {
                rowEnd = dst + dstW;
                step = dstW * 2;
                p = src;
                if (yErr < (int)srcH) {
                    src += srcW + srcSkip;
                    rowsLeft--;
                    step = dstH2;
                } else {
                    while (p < src + srcW && dst < rowEnd) {
                        if (step < (int)srcW) {
                            p++;
                            xErr = dstW * 2;
                        } else {
                            *dst = blend(*p, *dst, arg);
                            dst++;
                            xErr = srcW * -2;
                        }
                        step += xErr;
                    }
                    dst = rowEnd + dstSkip;
                    step = srcH * -2;
                }
                yErr += step;
            }
        } else {
            while (0 < rowsLeft) {
                rowEnd = dst + dstW;
                step = dstW * 2;
                p = src;
                if (yErr < (int)srcH) {
                    src += srcW + srcSkip;
                    rowsLeft--;
                    step = dstH2;
                } else {
                    while (p < src + srcW && dst < rowEnd) {
                        if (step < (int)srcW) {
                            p++;
                            xErr = dstW * 2;
                        } else {
                            if (keyTable[*p] == 0)
                                *dst = blend(*p, *dst, arg);
                            dst++;
                            xErr = srcW * -2;
                        }
                        step += xErr;
                    }
                    dst = rowEnd + dstSkip;
                    step = srcH * -2;
                }
                yErr += step;
            }
        }
    } else if ((flags & 0x400) == 0) {
        while (0 < rowsLeft) {
            rowEnd = dst + dstW;
            step = dstW * 2;
            p = src;
            if (yErr < (int)srcH) {
                src += -srcW + srcSkip;
                rowsLeft--;
                step = dstH2;
            } else {
                while (src - srcW < p && dst < rowEnd) {
                    if (step < (int)srcW) {
                        p--;
                        xErr = dstW * 2;
                    } else {
                        *dst = blend(*p, *dst, arg);
                        dst++;
                        xErr = srcW * -2;
                    }
                    step += xErr;
                }
                dst = rowEnd + dstSkip;
                step = srcH * -2;
            }
            yErr += step;
        }
    } else {
        while (0 < rowsLeft) {
            rowEnd = dst + dstW;
            step = dstW * 2;
            p = src;
            if (yErr < (int)srcH) {
                src += -srcW + srcSkip;
                rowsLeft--;
                step = dstH2;
            } else {
                while (src - srcW < p && dst < rowEnd) {
                    if (step < (int)srcW) {
                        p--;
                        xErr = dstW * 2;
                    } else {
                        if (keyTable[*p] == 0)
                            *dst = blend(*p, *dst, arg);
                        dst++;
                        xErr = srcW * -2;
                    }
                    step += xErr;
                }
                dst = rowEnd + dstSkip;
                step = srcH * -2;
            }
            yErr += step;
        }
    }
}
