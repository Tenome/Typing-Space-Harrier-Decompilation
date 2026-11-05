#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"

/* Functions that are written but do not match yet. Each carries the diagnosed residue. */

// OPEN: register allocation: orig keeps zero in esi and delays the esi/edi pushes past the early return (register/schedule)
int Game::M_0040cdd0(int index, int kind, int a3, int a4, int a5, int a6)
{
    int i;

    if (FUN_0040cf20() < 0)
        return DAT_00430658;
    DAT_004f052c->m_00 = 1;
    DAT_004f052c->m_04 = 1;
    DAT_004f052c->m_08 = a4;
    DAT_004f052c->m_0c = index;
    DAT_004f052c->m_10 = a3;
    DAT_004f052c->m_14 = kind;
    DAT_004f052c->m_b0 = DAT_00430658;
    DAT_004f052c->m_28 = 0;
    DAT_004f052c->m_24 = 0;
    DAT_004f052c->m_1c = a5;
    DAT_004f052c->m_20 = a6;
    DAT_004f052c->m_18 = 0;
    DAT_004f052c->m_24 = 0;
    for (i = 0; i < 0x21; i++)
        DAT_004f052c->m_2c[i] = -1;
    switch (kind) {
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
        DAT_004f052c->m_28 = kind - 0x32;
        DAT_004f052c->m_24 = 1;
        m_8e3cc8[index] = 0;
        return 0;
    case 0x38:
    case 0x39:
    case 0x3a:
    case 0x3b:
    case 0x3c:
        DAT_004f052c->m_28 = kind - 0x37;
        DAT_004f052c->m_24 = 1;
    }
    m_8e3cc8[index] = 0;
    return 0;
}

// OPEN: register allocation: orig caches constant 1 in ebx, loop counter in ebp (register/schedule)
int Game::M_0040d160(int index, int bank, int a3)
{
    int tries;
    int pick;
    int id;
    int base;
    int total;
    char *reading;
    char *text;
    int span;
    int weight;

    FUN_0040d130(bank);
    if (DAT_00a45e34->m_0c < 1)
        return DAT_00430658;
    for (tries = 0; tries < 10; tries++) {
        pick = rand() % DAT_00a45e34->m_0c;
        if (M_0040d850(DAT_00a45e34->m_14[pick], DAT_00a45e34->m_10[pick]) == 0)
            break;
    }
    m_87fcb8++;
    reading = DAT_00a45e34->m_10[pick];
    text = DAT_00a45e34->m_14[pick];
    weight = M_0040c970(text);
    id = M_0040dd60(index, reading, text, weight, a3);
    if (id >= 0) {
        base = DAT_004bde14[index * 0x16];
        total = DAT_004bde14[index * 0x16 + 3];
        if (total < 1)
            total = 1;
        m_8a3cc8[index][m_8a2cc8[index]] = (short)id;
        m_8a2cc8[index]++;
        span = DAT_008f069c[base];
        m_8c4->m_c = span - (m_8a2cc8[index] - 1) * span / total - 1;
        if (span <= m_8c4->m_c)
            m_8c4->m_c = span - 1;
        if (m_8c4->m_c < 0)
            m_8c4->m_c = 0;
        m_8c4->m_10 = m_882cc8[index][m_8c4->m_c];
        m_8c4->m_64 = 0;
    }
    return id;
}

// OPEN: 2 bytes: orig shares eax=0 between loop init and return 0 (epilogue sharing)
int Game::M_0040d850(char *first, char *second)
{
    char key;
    int count;
    int k;
    int i;
    char keys[12];

    M_00401630(0x20);
    M_00401400(0x20);
    strcpy(m_8c4->m_1494, first);
    strcpy(m_8c4->m_94, second);
    m_8c4->m_88 = strlen(m_8c4->m_1494) >> 1;
    M_00402e90();
    M_00402f10(0);
    count = 0;
    for (i = 0; i < 12; i++) {
        key = m_8c4->m_5094[i * KEY_LEN];
        if (key == '\0')
            continue;
        for (k = 0; k < count; k++) {
            if (key == keys[k])
                goto next;
        }
        keys[count++] = key;
    next:;
    }
    for (i = 0; i < count; i++) {
        key = keys[i];
        for (k = 0; k < 0x20; k++) {
            if (M_00401a40(k, key))
                return 1;
        }
    }
    return 0;
}

// OPEN: register allocation of count/pointer (ebp vs ebx), lea vs add chain, rows kept in memory+eax
void Game::M_0040db00()
{
    int rows;
    int width;
    int i;
    int j;
    int k;
    int row;
    int n;
    int total;
    int widest;
    int y;
    int x;
    int shuffles;
    int a;
    int b;
    int tmp;
    int order[17];

    DAT_00a3d358 = &DAT_00a3d488[0];
    DAT_00a3d488[0].m_00 = 1;
    DAT_00a3d488[0].m_20 = 0;
    rows = 1;
    for (i = 1; i < DAT_008f0ffc; i++) {
        DAT_00a3d358 = &DAT_00a3d488[i];
        if (DAT_00a3d358->m_00 != 0) {
            DAT_00a3d358->m_00 = 0;
            DAT_00a3d358->m_20 = rows;
            width = DAT_00a3d358->m_10 + 0x30;
            for (j = i + 1; j < DAT_008f0ffc; j++) {
                DAT_0099cfd4 = &DAT_00a3d488[j];
                if (DAT_0099cfd4->m_00 != 0) {
                    x = DAT_0099cfd4->m_10 + 0x28 + width;
                    if (x <= 0x280) {
                        DAT_0099cfd4->m_00 = 0;
                        DAT_0099cfd4->m_20 = rows;
                        width = x;
                    }
                }
            }
            rows++;
        }
    }
    y = 0x1a8;
    for (row = 0; row < rows; row++) {
        k = 0;
        total = 0;
        n = 0;
        widest = 0;
        for (i = 0; i < DAT_008f0ffc; i++) {
            DAT_008f1014 = &DAT_00a3d488[i];
            if (DAT_008f1014->m_20 == row) {
                order[k] = i;
                total += DAT_008f1014->m_10;
                n++;
                k++;
                if (widest < DAT_008f1014->m_14)
                    widest = DAT_008f1014->m_14;
            }
        }
        if (n > 1) {
            for (shuffles = n * 10; shuffles > 0; shuffles--) {
                a = rand();
                b = rand();
                tmp = order[a % n];
                order[a % n] = order[b % n];
                order[b % n] = tmp;
            }
        }
        if (row > 0)
            y -= widest;
        x = ((0x11 - n) * 0x28 - total) / 2;
        for (k = 0; k < n; k++) {
            DAT_008f1014 = &DAT_00a3d488[order[k]];
            DAT_008f1014->m_08 = x;
            if (row < 1)
                DAT_008f1014->m_0c = widest + 0x30;
            else
                DAT_008f1014->m_0c = y;
            x += 0x28 + DAT_008f1014->m_10;
            M_00401400(DAT_008f1014->m_04);
            m_8c4->m_1c = DAT_008f1014->m_08;
            m_8c4->m_20 = DAT_008f1014->m_0c;
        }
        if (row > 0)
            y -= 0x18;
    }
}

// OPEN: 1 byte: orig tests *str with cmp before loading al (loop entry guard form)
void Game::M_0040e2a0(char *str)
{
    char c;

    while (*str != 0) {
        c = *str;
        if (c >= 'a' && c <= 'z')
            *str = c - 0x20;
        str++;
    }
}

// OPEN: orig stores DAT_004f0530 with an immediate 0, not eax
void Game::M_00410340()
{
    memset(DAT_0099d5bc, 0, sizeof(DAT_0099d5bc));
    DAT_004f0530 = 0;
}

// OPEN: register swap of i/p in the counting loop (eax/ecx)
void Game::M_00410460(int mode)
{
    int n;
    int i;
    int *p;

    DAT_00a460bc = 0x1e;
    if (mode > -100) {
        if (mode < -1)
            mode = 3;
        n = 0;
        if (DAT_004e28dc[0] > 0) {
            i = 1;
            p = DAT_004e28dc;
            do {
                n = i;
                p++;
                i = n + 1;
            } while (*p > 0);
        }
        if (mode < 0 || n <= mode)
            mode = 2;
        DAT_00a461cc = 1000 / DAT_004e28dc[mode];
        if (DAT_00a461cc > 0)
            return;
    }
    DAT_00a461cc = 1;
}

// OPEN: loop rotation: orig loads next size after the name test
int Game::M_00410c20()
{
    BgmFile *file = DAT_004bdd70;
    const char *name = file->name;
    int ok = 1;

    if (*name != 0) {
        do {
            if (file->size == 0)
                break;
            if (M_00410ca0((char *)name, file->size) == DAT_00430658) {
                ok = 0;
                break;
            }
            name = file[1].name;
            file++;
        } while (*name != 0);
    }
    m_8 = 0;
    if (!ok) {
        M_00404a90();
        m_8 = 1;
        M_00404d80();
        m_a795c = 0xc;
    }
    return m_8;
}

// OPEN: tail layout of the two GlobalUnlock exits (107 vs 94 bytes)
int SoundMgr::FUN_004111f0(int index, int freq)
{
    LPDIRECTSOUNDBUFFER *buffers;
    LPDIRECTSOUNDBUFFER *slot;
    LPDIRECTSOUNDBUFFER buf;

    if (m_00 == 0 || m_08 == 0 || index < 0 || index >= m_04)
        return 0;
    buffers = (LPDIRECTSOUNDBUFFER *)GlobalLock(m_08);
    slot = buffers + index;
    buf = *slot;
    if (buf != 0 && buf->SetFrequency(freq) != 0) {
        GlobalUnlock(m_08);
        return 0;
    }
    GlobalUnlock(m_08);
    return freq;
}


// OPEN: one 170-byte function (Ghidra split it at 0x412118): register choice eax vs ecx for the running remainder
int FUN_004120e0()
{
    int bit;
    int dist;
    int len;
    int copied;
    int i;
    int rest;
    int byte;

    bit = FUN_00412190();
    if (bit == -1)
        return -1;
    if (bit == 0) {
        byte = FUN_004122a0();
        if (byte == -1)
            return -1;
        FUN_004122d0(byte);
        FUN_00412350(1);
        return 1;
    }
    dist = FUN_004121f0(FUN_004123f0());
    len = FUN_00412250();
    copied = 0;
    i = 0;
    rest = len;
    if (0 < len) {
        do {
            if (FUN_004122d0(FUN_00412300(i + dist)) == 0) {
                FUN_00412350(copied);
                rest -= copied;
                dist -= copied;
                copied = 0;
            }
            copied++;
            i++;
        } while (i < len);
    }
    FUN_00412350(rest);
    return 1;
}

// OPEN: orig keeps mask 0x80 in ecx (and/neg/sbb), mine folds to shr/and (masked/referent class)
int FUN_004121a0(StreamRec *stream)
{
    if (DAT_004ebb1c == 1) {
        DAT_00a4a6e0 = FUN_00411d20(stream);
        if (DAT_00a4a6e0 == 0xffffffff)
            return -1;
        DAT_004ebb1c = 0x80;
        return (DAT_00a4a6e0 & DAT_004ebb1c) != 0;
    }
    DAT_004ebb1c = DAT_004ebb1c >> 1;
    return (DAT_00a4a6e0 & DAT_004ebb1c) != 0;
}

// OPEN: register allocation: top/next kept in esi/eax, result via ecx
int FUN_004121f0(int bits)
{
    int value = 0;
    int top;
    int bit;

    for (top = 1; top * 2 < bits; top *= 2)
        value = FUN_004121a0(&DAT_00a4a680) + value * 2;
    if (value < top * 2 - bits)
        return value;
    bit = FUN_004121a0(&DAT_00a4a680);
    return value * 2 + top * -2 + bits + bit;
}

// OPEN: orig does not thread the zero-count branch past the loop test (CFG shape)
int FUN_00412250()
{
    int zeros = 0;
    int value = 1;
    int bit;

    bit = FUN_004121a0(&DAT_00a4a680);
    while (bit == 0) {
        zeros++;
        bit = FUN_004121a0(&DAT_00a4a680);
    }
    if (0 < zeros) {
        do {
            value = FUN_004121a0(&DAT_00a4a680) + value * 2;
            zeros--;
        } while (zeros != 0);
    }
    return value + 3;
}

// OPEN: putc inline expansion differs (194 vs 180 bytes)
unsigned int FUN_00411db0(unsigned int c, StreamRec *stream)
{
    unsigned int r;
    unsigned int pos;

    if (stream != 0 && stream->m_04 != 1) {
        if (stream->m_00 == 1) {
            r = putc(c, (FILE *)stream->m_08);
            if (r != c)
                return r;
            pos = stream->m_14 + 1;
            stream->m_14 = pos;
            if (stream->m_18 < pos)
                stream->m_18 = pos;
            if (stream->m_1c < stream->m_18) {
                stream->m_1c = stream->m_18;
                return r;
            }
            return r;
        }
        if (stream->m_14 < stream->m_1c && (int)c >= 0 && (int)c < 0x100) {
            stream->m_0c[stream->m_14] = (unsigned char)c;
            pos = stream->m_14 + 1;
            stream->m_14 = pos;
            if (stream->m_18 < pos) {
                stream->m_18 = pos;
                return c;
            }
            return c;
        }
    }
    return 0xffffffff;
}

// OPEN: block layout: orig emits the -1 and -2 branches after the switch (CFG order), mine in source order
void Game::M_0040f8a0()
{
    if (DAT_00a3d6f0 != -2) {
        if (DAT_00a3d6f0 == -1) {
            M_0040fa00();
            M_0040fa70(0x50);
            M_0040fab0();
            M_0040fa80();
            M_00410580(2, 0);
            m_390 = 1;
            return;
        }
        if (0 < DAT_008f1004)
            DAT_008f1004--;
        if (0 < DAT_00a3d6f0 && --DAT_00a3d6f0 == 0) {
            switch (DAT_00a45e28) {
            case 0:
                M_00410580(0, 0);
                m_390 = 0;
                M_00409330(1, 5);
                M_00408650(0x19);
                DAT_008f1004 = 0;
                if (0 < m_3c0)
                    DAT_008f1004 = DAT_00a460bc * 2;
                DAT_00a45e28 = 2;
                M_0040fa70(DAT_00a460bc * 3);
                return;
            case 1:
                M_004010d0();
                m_a795c = 0xb;
                return;
            case 2:
                M_0040f840();
                return;
            }
        }
    } else {
        DAT_00a45e28 = 1;
        M_00410460(-1);
        M_0040fa70(DAT_00a460bc * 0x1e);
        M_00408660(0x4f, 0);
        M_00410580(0, 0);
        m_390 = 0;
        M_00409330(1, 7);
    }
}

// OPEN: register/schedule: orig keeps result in a stack slot and i/span in ebp/ebx; loop peeled differently
int Game::M_004103c0(int index)
{
    int base = DAT_004efbd8[index];
    int span = DAT_008f069c[index];
    int i;
    int found;

    i = 0;
    found = 0;
    if (base <= i || span == i)
        return 0;
    if (0 < span) {
        do {
            M_0040af40(3, base);
            if (0 < m_87cbd4->m_2c[24]) {
                found = 1;
                break;
            }
            i++;
            base++;
        } while (i < span);
    }
    return found;
}

// OPEN: register choice: orig reuses eax for the loaded index, mine edx
void Game::M_00410440(int index)
{
    M_004103c0(DAT_004bde14[index * 0x16]);
}

// OPEN: orig reloads DAT_008f544c after the clearing loop (no value forwarding); mine forwards the constant
void Game::M_00410760()
{
    int *word;
    int n;

    DAT_008f544c = &DAT_0099d6c0;
    word = (int *)&DAT_0099d6c0;
    for (n = 0x2c; n != 0; n--)
        *word++ = 0;
    DAT_008f544c->m_00 = DAT_00430658;
    DAT_008f544c->m_10[0] = 0;
    DAT_008f544c->m_08 = 0;
    DAT_008f544c->m_04 = 0;
    DAT_008f544c->m_90 = 0x1c0;
    DAT_008f544c->m_94 = 0x1d0;
    DAT_008f544c->m_ac = 0;
    DAT_008f544c->m_a8 = 0;
    DAT_008f544c->m_9c = 0;
    DAT_008f544c->m_98 = 0;
}

// OPEN: size 398 vs 270: orig keeps separate epilogues per failure return (return-path layout)
int SoundMgr::FUN_00411340(int count)
{
    WAVEFORMATEX format;
    DSBUFFERDESC desc;
    LPDWORD slots;
    int i;

    if (count < 0)
        return 0;
    if (m_00 == 0)
        return 0;
    if (m_14 != 0)
        return 0;
    m_14 = 0;
    m_04 = -1;
    m_08 = GlobalAlloc(0x42, count * 4);
    if (m_08 == 0)
        return 0;
    slots = (LPDWORD)GlobalLock(m_08);
    if (0 < count) {
        for (i = count; i != 0; i--)
            *slots++ = 0;
    }
    GlobalUnlock(m_08);
    m_14 = GlobalAlloc(0x42, count * 4);
    if (m_14 == 0)
        return 0;
    slots = (LPDWORD)GlobalLock(m_14);
    if (0 < count) {
        for (i = count; i != 0; i--)
            *slots++ = 0;
    }
    GlobalUnlock(m_14);
    m_04 = count;
    desc.dwBufferBytes = 0;
    desc.dwReserved = 0;
    desc.dwSize = 0x14;
    desc.lpwfxFormat = 0;
    desc.dwFlags = 1;
    if (m_00->CreateSoundBuffer(&desc, &m_0c, 0) != 0) {
        m_0c = 0;
        return 0;
    }
    if (m_0c->SetFormat(&format) == 0) {
        m_0c->Play(0, 0, 1);
        return 1;
    }
    return 0;
}
