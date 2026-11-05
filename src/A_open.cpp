#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
// OPEN: argument evaluation order differs: orig loads the value argument after the two address-of leas (parameter reused as out-param)
int Game::M_00401da0(int n)
{
    int result;
    int b;

    FUN_00401d70(n, &n, &b);
    result = 0;
    if (n == 0 && b == 0)
        result = 1;
    return result;
}

// OPEN: register allocation: orig loads m_80 into edi before the y add (edi is w later); mine uses ecx
void Game::M_00402200()
{
    Slot *s = m_8c4;
    int i;
    int w;
    int color;
    int x;

    if (s->m_18 != 0 && s->m_60 == 0) {
        int y = s->m_30 + s->m_20;
        int remaining = s->m_84 - s->m_80;
        for (i = 0; i < 5; i++) {
            if (remaining <= 0)
                break;
            w = DAT_00a460bc * 5;
            if (remaining < DAT_00a460bc * 10)
                w = (DAT_00a460bc * remaining * 5) / (DAT_00a460bc * 10);
            color = DAT_004e1d94[i];
            remaining -= DAT_00a460bc * 10;
            x = (m_8c4->m_2c + m_8c4->m_1c) - w / 2;
            if (i == 0)
                M_004022e0(x + 2, y + 2, w, 4, 0);
            M_004022e0(x, y, w, 4, color);
        }
    }
}

// OPEN: frame/slot allocation: orig keeps tint and count in one stack slot (frame 0x20, mine 0x1c); register assignment differs throughout
void Game::M_00402310()
{
    Slot *s;
    int frame;
    int base;
    int tileW, tileH;
    int boxW, boxH;
    int top, bottom, edge;
    int i;
    int x;

    frame = (m_8c0 != m_8b8) ? 8 : 1;
    s = m_8c4;
    if (s->m_60 != 0)
        frame = 0;
    if (s->m_64 != 0 && s->m_80 * 2 >= s->m_84)
        frame = 3;
    M_004022e0(s->m_1c - 1, s->m_20 - 1, s->m_24 + 1, s->m_28 + 1, frame);

    s = m_8c4;
    base = m_87cbfc[11] + 0x18;
    int tint = DAT_00430658;
    if (s->m_60 == 0 && s->m_50e0 < 0) {
        int kind = DAT_004bddf8[s->m_8].m_24;
        if (kind >= 0x17 && kind <= 0x28)
            tint = 2;
        else if (kind >= 0x2a && kind <= 0x43)
            tint = 3;
    }
    if (tint != DAT_00430658) {
        tileW = strlen(s->m_94) * 10;
        tileH = 0x14;
        M_004022e0(s->m_3c + s->m_1c, s->m_40 + s->m_20, tileW, 0x14, tint);
    }

    s = m_8c4;
    boxW = s->m_24;
    boxH = s->m_28;
    bottom = s->m_20 + boxH;
    tileW = m_87fc24[0];
    top = s->m_20 - m_87fc64[0];
    if (tileW != 0) {
        int count;
        count = boxW / tileW;
        for (i = 0; i < count; i++) {
            x = i * tileW + m_8c4->m_1c;
            M_0040bea0(x, top, base);
            M_0040bea0(x, bottom, base);
        }
        if (boxW != count * tileW) {
            x = (m_8c4->m_1c - tileW) + boxW;
            M_0040bea0(x, top, base);
            M_0040bea0(x, bottom, base);
        }
        edge = m_8c4->m_1c;
        top = m_8c4->m_24 + edge;
        tileH = m_87fc64[1];
        edge = edge - m_87fc24[1];
        if (tileH != 0) {
            count = boxH / tileH;
            for (i = 0; i < count; i++) {
                x = i * tileH + m_8c4->m_20;
                M_0040bea0(edge, x, base + 1);
                M_0040bea0(top, x, base + 1);
            }
            if (boxH != count * tileH) {
                x = (m_8c4->m_20 - tileH) + boxH;
                M_0040bea0(edge, x, base + 1);
                M_0040bea0(top, x, base + 1);
            }
            s = m_8c4;
            edge = s->m_1c - m_87fc24[2];
            top = s->m_24 + s->m_1c;
            x = s->m_20 - m_87fc64[2];
            bottom = s->m_28 + s->m_20;
            M_0040bea0(edge, x, base + 2);
            M_0040bea0(top, x, base + 3);
            M_0040bea0(edge, bottom, base + 4);
            M_0040bea0(top, bottom, base + 5);
            if (m_8c0 == m_8b8) {
                int spark = m_87cbfc[12] + (DAT_00a46038 / 4) * 4;
                int inset;
                DAT_00a46038++;
                if (DAT_00a46038 > 0xf)
                    DAT_00a46038 = 0;
                M_0040be30(spark, &tileW, &tileH);
                inset = DAT_004e1dac[DAT_00a46034];
                DAT_00a46034 += DAT_004e1da8;
                if (DAT_00a46034 < 1 || DAT_00a46034 > 0xf)
                    DAT_004e1da8 = -DAT_004e1da8;
                boxW = m_87fc24[2];
                count = m_87fc64[2];
                x -= inset;
                i = edge - inset;
                M_0040bea0(i - tileW, x - tileH, spark);
                boxW = boxW + inset + top;
                M_0040bea0(boxW, x - tileH, spark + 1);
                x = inset + bottom + count;
                M_0040bea0(i - tileW, x, spark + 2);
                M_0040bea0(boxW, x, spark + 3);
            }
        }
    }
}

// OPEN: register/spill allocation: orig keeps the call's first argument in ebp and the second in a stack slot, frame 0x2808 (mine 0x2804, roles swapped)
void Game::M_00402960()
{
    char text[0x1400];
    char line[0x1400];
    int x, y;

    if (m_8c4->m_60 == 0) {
        M_00402a80(text);
        x = m_8c4->m_34 + m_8c4->m_1c;
        y = m_8c4->m_38 + m_8c4->m_20;
        M_0040e2a0(text);
        strcpy(line, "@R");
        strcat(line, text);
        M_0040bfa0(x, y, line);
    }
}

// OPEN: register allocation: orig keeps out in ebp, the loop-invariant key offset in edi and spills this; mine keeps this in ebp and spills out.
int Game::M_00402a80(char *out)
{
    char *dst = out;
    int len = 0;
    int cand;
    int last;
    int pos;
    int i;
    int id;
    int sel;
    char c;
    const char *p;

    memset(dst, 0, 0x1400);
    cand = m_8c4->m_50dc;
    if (m_8c4->m_5094[cand * KEY_LEN] == 0 && m_8c4->m_5094[0] == 0)
        cand = 0;
    last = -1;
    if (m_8c4->m_5094[cand * KEY_LEN] != 0) {
        i = 0;
        do {
            c = m_8c4->m_5094[cand * KEY_LEN + i];
            if (c == 0)
                break;
            dst[i] = c;
            len++;
            i++;
            last = cand;
        } while (i < 4);
    }
    pos = m_8c4->m_50e8 + 1;
    if (last >= 5)
        pos++;
    if (len < 0x10) {
        do {
            if (m_8c4->m_50ec != 0)
                break;
            id = m_8c4->m_2894[pos];
            if (id == -1)
                break;
            if ((id >= 0x400 && id <= 0x409) || (id >= 0x40a && id <= 0x423)) {
                M_004034c0((char *)&out, id);
                dst[len] = (char)(int)out;
                len++;
            } else {
                sel = id;
                if (id >= 0xcf)
                    sel = id - 0xcf;
                if (len < 0x10) {
                    p = DAT_00429040[id].rom[m_3c[sel]];
                    do {
                        if (*p == 0)
                            break;
                        dst[len] = *p;
                        p++;
                        len++;
                    } while (len < 0x10);
                }
            }
            pos++;
        } while (len < 0x10);
    }
    return strlen(dst);
}

// OPEN: register allocation: orig caches DAT_00430658 in esi and this in ebp, result in a stack slot; mine differs (size 242 vs 219)
int Game::M_00403570()
{
    int result = DAT_00430658;
    char *text = &m_8c4->m_1494[m_a7980];
    unsigned short code = (unsigned char)text[0] << 8 | (unsigned char)text[1];
    RomajiEntry *e;
    int id;
    int len;

    if (code >= 0x8260 && code <= 0x8279)
        result = code - 0x7e56;
    else if (code >= 0x824f && code <= 0x8258)
        result = code - 0x7e4f;
    if (result != DAT_00430658) {
        m_a7980 += 2;
        return result;
    }
    id = 0x19d;
    e = &DAT_00429040[0x19d];
    do {
        len = strlen(e->kana);
        if (strncmp(e->kana, text, len) == 0) {
            m_a7980 += len;
            return id;
        }
        e--;
        id--;
    } while (e >= DAT_00429040);
    return result;
}

// OPEN: orig loads the global array base into a register (mov eax,imm; cmp [eax],0) and does not fold the first *p load; every loop spelling tried (do/while, for, while+break) folds it
void Game::M_00403650()
{
    int i = 0;
    char *p = DAT_00a461d0;

    while (*p != 0) {
        i++;
        *p = p[1];
        p++;
        if (i >= 0x1f)
            break;
    }
}

// OPEN: register allocation: orig keeps mode in memory ([esp+0x18], frame 8 bytes of locals) and pointer/counter roles in the name-count loop are swapped (eax=n, ecx=ptr); mine enregisters mode in ebp
void Game::M_00403940(int mode)
{
    int count, next;
    char **names;
    int i;
    int result;
    int held;
    int cursor, state;
    int stage;

    if (m_8ac < 0) {
        m_3cc = 0;
        if (*DAT_004bdcdc[0] != 0) {
            next = 1;
            names = DAT_004bdcdc;
            do {
                count = next;
                names++;
                next++;
            } while (**names != 0);
        } else {
            count = mode;
        }
        M_004061c0(2);
        M_00406210(0, 0, 0);
        M_00406240(count, 2);
        M_00406270(0, 0);
        M_004062e0(0x140, 0xd8, 0x80, 0x20);
        M_004061f0(7, 1);
        for (i = 0; *DAT_004bdcc8[i] != 0; i++)
            M_004062b0(i, DAT_004bdcc8[i]);
        M_0040e900(1);
    } else if (m_8ac == 0) {
        M_0040e900(0);
    }
    M_00406290(&cursor, &state);
    held = state;
    if (state == 0)
        cursor = m_3cc;
    else
        cursor = 0;
    M_00406270(cursor, state);
    M_00406600();
    result = M_00406660();
    M_00406290(&cursor, &state);
    if (state == 0 && held == 0)
        m_3cc = cursor;
    M_00403b80(mode);
    if (result == 0) {
        m_8ac++;
        if (m_8ac >= 1000000)
            m_8ac = 1000000;
        return;
    }
    if (result < 0)
        state = 1;
    switch (state) {
    case 0:
        m_a795c = 4;
        DAT_00a23308 = DAT_004bdd28[m_3cc];
        m_3c4 = m_380;
        M_00410460(m_378);
        M_004014d0(DAT_00a23308);
        M_004010d0();
        M_00403730();
        if (mode == 1) {
            stage = M_00403890(DAT_00a23308);
            M_004038f0(DAT_00a23308, stage);
            m_3d8[12] = stage;
            m_3d0 = 1;
        }
        break;
    case 1:
        m_a795c = 2;
        M_004010d0();
        return;
    }
    M_004010d0();
}

// OPEN: CFG layout: orig places the reset block first and the counter block last (tests: jne reset; test expired; je counter); every if/else, early-return and negated spelling tried gives counter-first
void Game::M_00404040()
{
    int tick;
    int expired;
    int done;

    DAT_00a460c8 = 0;
    expired = 0;
    tick = m_8ac;
    if (tick < 0)
        M_004014d0(DAT_009016fc);
    if (tick >= DAT_00a460bc * 30)
        expired = 1;
    done = M_00406660();
    if (done != 0 || expired) {
        DAT_009016fc++;
        if (DAT_009016fc >= 0x10)
            DAT_009016fc = 0;
        M_004014d0(DAT_00a23308);
        M_004010d0();
        M_00403730();
        m_a795c = 2;
        DAT_00a460c8 = 0;
    } else {
        m_8ac++;
        if (m_8ac >= 1000000)
            m_8ac = 1000000;
        M_00404110(0x14);
    }
}

// OPEN: register allocation: orig keeps a zero constant in ebx (xor ebx,ebx at entry) and the limit in eax, tick in edi; mine uses immediates, limit in edi
void Game::M_004042a0()
{
    int limit;
    int tick;

    DAT_00a460c8 = 0;
    M_0040a640();
    M_0040bea0(0xc0, 0xb0, m_87cbfc[0]);
    if (m_39c != 0) {
        if (m_87fcc4 == 0) {
            M_0040c250(0x170, "@aYOU GOT THE");
            M_0040c250(0x1a0, "@aTYPING MASTER POWER");
        } else {
            M_0040c250(400, "@aTHANKS FOR PLAYING");
        }
        limit = DAT_00a460bc * 30;
    } else {
        limit = DAT_00a460bc * 5;
    }
    tick = m_8ac;
    if ((M_00406660() == 0 || (tick < DAT_00a460bc * 3 && m_39c == 0)) && tick < limit) {
        m_8ac++;
        if (m_8ac >= 1000000)
            m_8ac = 1000000;
        return;
    }
    M_004010d0();
    M_00403730();
    if (m_39c == 0)
        m_a795c = 2;
    else
        m_a795c = 0xb;
    m_39c = 0;
    DAT_00a460c8 = 0;
}

// OPEN: loop induction variable: orig steps an absolute address (cmp edi,0x4e1eb0; jl) and compares m_8ac via eax; mine uses a 0..0x20 offset
void Game::M_004043d0()
{
    int i;
    int tile;

    if (m_8ac < 0) {
        M_00410460(-1);
        m_87fcc4 = 0;
        M_004013b0();
        M_00403730();
        M_004093b0(0, 2, DAT_004efbe0, DAT_008f06a4);
    }
    M_0040a640();
    M_0040a210();
    DAT_00a45e38 = 0;
    M_0040a8c0();
    tile = M_0040bf80(0x7d, 0);
    for (i = 0; i < 4; i++) {
        if (i != 2)
            M_0040bea0(DAT_004e1e90[i * 2], DAT_004e1e90[i * 2 + 1], tile);
        tile++;
    }
    M_00406660();
    m_8ac++;
    if (m_8ac >= 1000000)
        m_8ac = 1000000;
}

// OPEN: register allocation/prologue: orig pushes esi+edi at entry (edi=size) and ebx later; mine delays edi. All 24 declaration orders tried.
int Game::M_00405220(const char *path)
{
    FILE *f;
    int size, got, result;

    f = fopen(path, "rb");
    if (f == 0)
        return DAT_00430658;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    got = fread(DAT_008f144c, 1, size, f);
    fclose(f);
    result = DAT_00430658;
    if (got == size)
        result = size;
    return result;
}

// OPEN: register allocation / loop shape (size differs by a few bytes): see fd diff; walls of class register/schedule
void Game::M_00405b80()
{
    char *key;
    char *p;
    int *out;
    int n;
    char next;

    memset(DAT_00a23310, 0, sizeof DAT_00a23310);
    if (DAT_00429040[0].kana[0] != 0) {
        key = DAT_00429040[0].kana;
        out = DAT_00a23310;
        do {
            n = 0;
            p = key;
            do {
                p += 7;
                if (*p == 0)
                    break;
                n++;
            } while (n < 4);
            next = key[0x24];
            key += 0x24;
            *out = n;
            out++;
        } while (next != 0);
    }
}

// OPEN: register allocation / loop shape (size differs by a few bytes): see fd diff; walls of class register/schedule
void Game::M_00405bd0()
{
    int i = 0;
    int last;
    char **names;

    DAT_00a45e30 = 0;
    DAT_00a45e2c = 0;
    DAT_00a45e3c = 0;
    last = 0;
    if (*DAT_004bd85c[0] != 0) {
        names = DAT_004bd85c;
        do {
            last = i;
            names++;
            i++;
        } while (**names != 0);
    }
    DAT_00a23300 = last + 1;
    DAT_00a23304 = ((last + 4) / 10) * 5;
    if (DAT_00a23300 > 10)
        DAT_00a23300 = 10;
}

// OPEN: register allocation / loop shape (size differs by a few bytes): see fd diff; walls of class register/schedule
void Game::M_00405fe0(int x)
{
    int col = x / 5;
    int last;
    int ignored;

    last = DAT_00a23304 / 5 - 2;
    if (col < 2)
        col = 0;
    if (last <= col)
        col = last;
    M_004060f0(&x, &ignored, 0, col * 10);
    x = x + (-0x50 - DAT_009016f4);
    M_00406070(-x, 0, 4);
}

// OPEN: register allocation: orig keeps this in ebx and 'before' in ebp, DAT_00951fcc in edi inside the clamp loop; mine differs. All 120 declaration orders tried.
void Game::M_00406540(int dx, int dy)
{
    int before, after;
    int i, v, limit;

    before = M_00406350(DAT_00951fcc->m_14[0], DAT_00951fcc->m_14[1]);
    DAT_00951fcc->m_14[0] += dx;
    DAT_00951fcc->m_14[1] += dy;
    for (i = 0; i < 2; i++) {
        v = DAT_00951fcc->m_14[i];
        limit = DAT_00951fcc->m_1c[i];
        if (DAT_00951fcc->m_8[i] == 1) {
            if (v >= limit)
                v = limit - 1;
            if (v < 0)
                v = 0;
        } else {
            if (v >= limit)
                v -= limit;
            if (v < 0)
                v += limit;
        }
        DAT_00951fcc->m_14[i] = v;
    }
    after = M_00406350(DAT_00951fcc->m_14[0], DAT_00951fcc->m_14[1]);
    if (before != after) {
        m_8ac = 0;
        M_00408650(DAT_00951fcc->m_38);
    }
}

// OPEN: loop shape / ordering: orig is a non-peeled do-while with absolute-address induction variable and DAT_00430658 cached; see fd diff
void Game::M_004066d0(int n)
{
    if (m_a795c == 4 && m_390 == 0 && DAT_00a45e28 == 0 && *m_3d8 + 1 <= 0x12 && m_87fcb4 == 0) {
        n = m_3a0 + n;
        m_3a0 = n;
        if (n < 0)
            m_3a0 = 0;
        if (m_3a0 >= 0x98967f)
            m_3a0 = 0x98967f;
        n = m_3a0;
        if (m_3a4 < n)
            m_3a4 = n;
        if (m_10[m_378] < n)
            m_10[m_378] = n;
        n = m_3ac + m_3a8;
        if (n <= m_3a0) {
            m_3a8 = n;
            m_3c0++;
            M_00408650(8);
        }
    }
}

// OPEN: loop shape / ordering: orig is a non-peeled do-while with absolute-address induction variable and DAT_00430658 cached; see fd diff
int Game::M_004068a0(int n)
{
    int none = DAT_00430658;
    int i = 0;

    while (DAT_00a3d6ec = &DAT_009a629c[i], DAT_009a629c[i].m_0 == 0 || DAT_009a629c[i].m_8 != n) {
        i++;
        if (i >= 0x100)
            return none;
    }
    return i;
}

// OPEN: loop shape / ordering: orig is a non-peeled do-while with absolute-address induction variable and DAT_00430658 cached; see fd diff
int Game::M_00406a50()
{
    int none = DAT_00430658;
    int i = 0;

    do {
        DAT_00a3d6ec = &DAT_009a629c[i];
        if (DAT_009a629c[i].m_0 == 0)
            return i;
        i++;
    } while (i < 0x100);
    return none;
}

// OPEN: register allocation/loop inversion: orig run=edi k=esi with the count cached in ecx and a single test-at-bottom loop; all 120 local orders tried
int Game::M_00406be0()
{
    int idx, k, run, value;
    int *p;

    if (DAT_009a5e98.m_0 < 3)
        return 0;
    idx = DAT_009a5e98.m_0 - 1;
    k = 0;
    value = DAT_009a5e98.m_4[idx];
    p = &DAT_009a5e98.m_4[idx];
    run = 1;
    if (DAT_009a5e98.m_0 > 0) {
        do {
            idx--;
            p--;
            if (idx < 0 || *p != value)
                break;
            run++;
            k++;
        } while (k < DAT_009a5e98.m_0);
        if (run > 2) {
            M_00407590(run);
            M_00407310(value);
        }
    }
    return value;
}

// OPEN: register allocation: orig keeps the Menu pointer in eax and the count spilled; mine differs; 150 declaration orders tried
void Game::M_00406490()
{
    Menu *m = DAT_00951fcc;
    int count, sel, i, idx;
    int x, y, n;
    char *text;

    if (m->m_10 != 0) {
        count = m->m_1c[1];
        sel = m->m_14[1];
        i = 0;
        if (count > 0) {
            do {
                idx = i;
                if (sel == i)
                    idx = m->m_1c[1] + i;
                text = m->m_3c[idx];
                if (text != 0 && *text != 0) {
                    y = m->m_30 * i + m->m_28;
                    n = strlen(text) - 2;
                    x = m->m_24;
                    if (text[1] >= 'a' && text[1] <= 'b') {
                        x -= n * 16;
                        y -= 8;
                    } else {
                        x -= n * 8;
                    }
                    M_0040bfa0(x, y, text);
                    m = DAT_00951fcc;
                }
                i++;
            } while (i < count);
        }
    }
}

// OPEN: induction-variable choice: orig strength-reduces on a different member of DAT_009a629c[i] (m_8 / m_28) than mine; first draft per Ghidra
void Game::M_00406910(int n, int arg)
{
    int i = 0;
    int k;
    int count;

    if (n < 0 || DAT_009a5e98.m_0 < n)
        n = DAT_009a5e98.m_0;
    count = DAT_009a5e98.m_0;
    for (k = 0; k < 0x100; k++) {
        DAT_00a3d6ec = &DAT_009a629c[k];
        if (DAT_00a3d6ec->m_0 != 0 && DAT_009a629c[k].m_8 > 0) {
            DAT_009a629c[k].m_8 -= n;
            DAT_00a3d6ec->m_1c = (0x14 - DAT_00a3d6ec->m_8) * 0x20;
            DAT_00a3d6ec->m_20 = 0;
            if (DAT_00a3d6ec->m_8 < 1) {
                DAT_00a3d6ec->m_30 = 1;
                DAT_00a3d6ec->m_3c = 0;
                DAT_00a3d6ec->m_14 = DAT_00a3d6ec->m_c;
                DAT_00a3d6ec->m_18 = DAT_00a3d6ec->m_10;
            }
            if (DAT_00a3d6ec->m_0 == 3) {
                DAT_00a3d6ec->m_3c = 0;
                DAT_00a3d6ec->m_0 = 2;
                DAT_00a3d6ec->m_24 = 0;
                DAT_00a3d6ec->m_28 = arg;
                DAT_00a3d6ec->m_14 = DAT_00a3d6ec->m_c;
                DAT_00a3d6ec->m_18 = DAT_00a3d6ec->m_10;
            }
            count = DAT_009a5e98.m_0;
            if (DAT_00a3d6ec->m_8 < 1) {
                count = DAT_009a5e98.m_0 - 1;
                DAT_009a5e98.m_0 = count;
            }
        }
    }
    if (count > 0) {
        do {
            DAT_009a5e98.m_4[i] = DAT_009a5e98.m_4[n + i];
            i++;
            count = DAT_009a5e98.m_0;
        } while (i < DAT_009a5e98.m_0);
    }
    if (count < 0x100)
        memset(&DAT_009a5e98.m_4[count], 0, (0x100 - count) * sizeof(int));
}

// OPEN: induction-variable choice: orig strength-reduces on a different member of DAT_009a629c[i] (m_8 / m_28) than mine; first draft per Ghidra
void Game::M_00406a80()
{
    int i;

    if (*m_3d8 + 1 <= 0x12) {
        for (i = 0; i < 0x100; i++) {
            DAT_00a3d6ec = &DAT_009a629c[i];
            if (DAT_00a3d6ec->m_0 != 0 && DAT_009a629c[i].m_28 > 0) {
                if (DAT_00a3d6ec->m_0 == 1)
                    DAT_009a629c[i].m_10 = ((DAT_009a629c[i].m_20 - DAT_009a629c[i].m_18) * DAT_009a629c[i].m_24) / DAT_009a629c[i].m_28 + DAT_009a629c[i].m_18;
                if (DAT_00a3d6ec->m_0 == 2)
                    DAT_00a3d6ec->m_c = ((DAT_00a3d6ec->m_1c - DAT_00a3d6ec->m_14) * DAT_00a3d6ec->m_24) / DAT_00a3d6ec->m_28 + DAT_00a3d6ec->m_14;
                M_0040bea0(DAT_00a3d6ec->m_c, DAT_00a3d6ec->m_10, DAT_00a3d6ec->m_34);
                if (DAT_00a3d6ec->m_0 < 3) {
                    if (DAT_00a3d6ec->m_0 == 1 && DAT_00a3d6ec->m_24 == 0 && DAT_00a3d6ec->m_38 != 0) {
                        M_00408650(DAT_00a3d6ec->m_38);
                        DAT_00a3d6ec->m_38 = 0;
                    }
                    if (DAT_00a3d6ec->m_2c < 1) {
                        if (DAT_00a3d6ec->m_24 < DAT_00a3d6ec->m_28) {
                            DAT_00a3d6ec->m_24++;
                        } else {
                            DAT_00a3d6ec->m_0++;
                            DAT_00a3d6ec->m_24 = 0;
                            if (DAT_00a3d6ec->m_0 == 3 && DAT_00a3d6ec->m_4 == 1) {
                                DAT_00a3d6ec->m_4 = 0;
                                M_00406be0();
                                if (DAT_00a3d6ec->m_3c != 0)
                                    M_00408650(DAT_00a3d6ec->m_3c);
                            }
                        }
                    } else {
                        DAT_00a3d6ec->m_2c--;
                    }
                } else if (DAT_00a3d6ec->m_30 > 0 && --DAT_00a3d6ec->m_30 == 0) {
                    DAT_00a3d6ec->m_0 = 0;
                }
            }
        }
    }
}

// OPEN: register/schedule: orig reuses dead parameter slots (set/frame) for count and bottom and keeps tileW in ebp; mine is 10 bytes longer
void Game::M_00402730(int x, int y, int w, int h, int set, int frame)
{
    int tileW, tileH;
    int top, leftPad;
    int base, bottom;
    int count, n;
    int px, py;

    if (frame >= 0)
        M_004022e0(x - 1, y - 1, w + 1, h + 1, frame);
    base = m_87cbfc[11] + set * 6;
    tileW = m_87fc24[0];
    top = y - m_87fc64[0];
    bottom = y + h;
    if (tileW != 0) {
        count = w / tileW;
        if (count > 0) {
            px = x;
            n = count;
            do {
                M_0040bea0(px, top, base);
                M_0040bea0(px, bottom, base);
                px += tileW;
                n--;
            } while (n != 0);
        }
        if (w != count * tileW) {
            tileW = (x - tileW) + w;
            M_0040bea0(tileW, top, base);
            M_0040bea0(tileW, bottom, base);
        }
        leftPad = m_87fc24[1];
        tileH = m_87fc64[1];
        w = x + w;
        if (tileH != 0) {
            count = h / tileH;
            if (count > 0) {
                py = y;
                n = count;
                do {
                    M_0040bea0(x - leftPad, py, base + 1);
                    M_0040bea0(w, py, base + 1);
                    py += tileH;
                    n--;
                } while (n != 0);
            }
            if (h != tileH * count) {
                h = (y - tileH) + h;
                M_0040bea0(x - leftPad, h, base + 1);
                M_0040bea0(w, h, base + 1);
            }
            leftPad = m_87fc24[2];
            tileH = m_87fc64[2];
            M_0040bea0(x - leftPad, y - tileH, base + 2);
            M_0040bea0(w, y - tileH, base + 3);
            M_0040bea0(x - leftPad, bottom, base + 4);
            M_0040bea0(w, bottom, base + 5);
        }
    }
}

// OPEN: register allocation: orig keeps the row counter i in memory and col in edi; mine enregisters i. 60 declaration orders tried
void Game::M_00403d80(int t)
{
    char line[0x100];
    int counts[6];
    int i = 0;
    int row, col, x, y, delay, w, len;
    int pct, total, best, bestIdx, k, a, b, color, blink;

    M_0040a640();
    M_0040bea0(0, 0, m_87cbfc[126]);
    M_0040c250(0x20, DAT_004e1e80);
    do {
        row = i % 10;
        col = i / 10;
        x = col * 0x140 + 4;
        delay = row * 5 + 10;
        y = row * 0x20 + 0x90;
        if (t < delay)
            x += (((delay - t) * 0x280) / 10) * (col * 2 - 1);
        i++;
        M_00410240(i);
        len = strlen(DAT_004e1e84);
        w = (len - 2) * 10;
        if (row == 0) {
            M_004022e0(x, 0x5c, w, 0x28, 0);
            M_0040bfa0(x, 0x5c, DAT_004e1e84);
            M_0040bfa0(x, 0x70, DAT_004e1e88);
        }
        pct = 0;
        blink = (m_8a8 & 7) < 4;
        total = DAT_0099d6bc->m_00[0xc] + DAT_0099d6bc->m_00[0xb];
        if (total > 0)
            pct = (DAT_0099d6bc->m_00[0xb] * 100) / total;
        color = 0;
        if (blink && pct == 100)
            color = 8;
        M_004022e0(x, y, w, 0x14, color);
        if (blink) {
            bestIdx = DAT_00430658;
            best = 0;
            memset(counts, 0, sizeof counts);
            counts[1] = DAT_0099d6bc->m_00[0x11];
            counts[2] = DAT_0099d6bc->m_00[0x12];
            counts[3] = DAT_0099d6bc->m_00[0x13];
            counts[4] = DAT_0099d6bc->m_00[0x14];
            for (k = 0; k < 6; k++) {
                if (best < counts[k]) {
                    best = counts[k];
                    bestIdx = k;
                }
            }
            if (best > 0)
                M_004022e0(x + 10 + bestIdx * 0x1e, y, 0x14, 0x14, 2);
        }
        a = DAT_0099d6bc->m_00[0xb];
        b = DAT_0099d6bc->m_00[0xc];
        if (a > 998)
            a = 999;
        if (b > 998)
            b = 999;
        wsprintfA(line, DAT_004e1e8c, i, DAT_0099d6bc->m_00[0x11], DAT_0099d6bc->m_00[0x12], DAT_0099d6bc->m_00[0x13], DAT_0099d6bc->m_00[0x14], DAT_0099d6bc->m_00[0x15], a, b, pct);
        M_0040bfa0(x, y, line);
    } while (i <= 0x11);
}

// OPEN: register/stack allocation (same size 797): orig keeps mode in esi and shifts the stack slots of the cursor/key locals; 200+ local orders tried
void Game::M_00405c50(int mode)
{
    char line[0x100];
    char **title;
    short *glyph;
    int page, row, col, kana, variant, nvar, x, y, kx, ky, len, cursorX, cursorY;

    page = mode + 5;
    M_004061c0(page);
    M_00405f70();
    M_0040a640();
    DAT_00a3d6f4 = strlen(DAT_004bd858) - 2;
    M_00402730(((0x40 - DAT_00a3d6f4) * 10) / 2 - 0x28, 10, (DAT_00a3d6f4 * 5 + 0x28) * 2, 0x28, 2, 1);
    M_0040bfa0(((0x40 - DAT_00a3d6f4) * 10) / 2, 0x14, DAT_004bd858);
    M_004061c0(5);
    M_00406380();
    len = strlen(DAT_004bd848[m_38]);
    x = DAT_00951fcc->m_2c + DAT_00951fcc->m_24 + len * -10;
    y = DAT_00951fcc->m_30 + DAT_00951fcc->m_28;
    M_0040bfa0(x, y, DAT_004bd848[m_38]);
    row = 0;
    title = DAT_004bd85c;
    glyph = DAT_004bd924;
    do {
        if (**title == '\0')
            break;
        M_004060f0(&x, &y, 0, row);
        col = 0;
        do {
            kana = *glyph;
            if (kana != -1) {
                variant = m_3c[kana];
                nvar = DAT_00a23310[kana];
                M_004060f0(&kx, &ky, col, row);
                M_004022e0(kx, ky, 0x28, 0x24, ((nvar < 2) - 1 & 2) + 1);
                kx += 0x14 - ((int)strlen(DAT_00429040[kana].rom[variant]) * 10) / 2;
                ky += 0x14;
                strcpy(line, "@R");
                strcat(line, DAT_00429040[kana].rom[variant]);
                M_0040bfa0(kx, ky, line);
            }
            col++;
            glyph++;
        } while (col < 5);
        M_0040f1c0(x, y, *title);
        title++;
        row++;
    } while ((int)glyph < 0x4bdb18);
    if (mode == 1 || mode == 2) {
        cursorX = DAT_00a45e2c % 5;
        cursorY = DAT_00a45e30 + (DAT_00a45e2c / 5) * 10;
        M_004060f0(&kx, &ky, cursorX, cursorY);
        M_00402730(kx, ky, 0x28, 0x24, (-(mode != 1) & -2) + 4, -1);
    }
    M_004061c0(page);
}

// OPEN: near match (one extra mov esi,edi in the cursor update; rest of the 1035-byte body aligned, jump tables included). Needs DAT_004bd608 as an int array in lane B's prelude (currently scalar, indexed through (&DAT_004bd608)[sel])
void Game::M_004052e0()
{
    char line[0x100];
    int sel, cursorX, done, cur, limit, idx, selected, x, y;
    char *text;

    if (m_8ac < 0) {
        m_388 = 0;
        m_384 = 0;
        m_38c = 0;
        M_00403730();
        M_004061c0(1);
        M_00406210(0, 0, 0);
        M_00406240(3, 7);
        M_00406270(1, 0);
        M_004062e0(0x40, 0xa0, 0x200, 0x20);
        M_004061f0(0, 1);
        for (idx = 0; *DAT_004bd5cc[idx] != 0; idx++)
            M_004062b0(idx, DAT_004bd5cc[idx]);
    }
    M_00406600();
    M_0040a640();
    M_0040bfa0(0xc0, 0x50, "@aOPTIONS");
    cur = m_3c4;
    if (cur == 0)
        cur = m_380;
    wsprintfA(line, "@2CREDIT %3d", cur);
    M_0040bfa0(0x20, 0x1c0, line);
    M_0040bfa0(0xf0, 0x1c0, "@i8@2 SEGA CORPORATION,2002");
    M_0040bfa0(0x226, 4, "@3V1.0B");
    M_00406380();
    done = M_00406660();
    M_00406290(&cursorX, &sel);
    M_00406270(1, sel);
    /* OPEN(types): lane B declares DAT_004bd608 as a scalar; it is an int array of option counts */
    limit = (&DAT_004bd608)[sel];
    if (m_c == 0 && sel == 0)
        limit--;
    switch (sel) {
    case 0: cur = m_378; break;
    case 1: cur = m_37c; break;
    case 2: cur = m_380; break;
    case 4: cur = m_384; break;
    case 5: cur = m_388; break;
    default: cur = 0; break;
    }
    cursorX--;
    if (limit > 0) {
        cur += cursorX;
        limit--;
        if (cur > limit)
            cur = 0;
        else if (cur < 0)
            cur = limit;
    }
    switch (sel) {
    case 0: m_378 = cur; break;
    case 1: m_37c = cur; break;
    case 2: m_380 = cur; break;
    case 4: m_384 = cur; break;
    case 5: m_388 = cur; break;
    }
    idx = 0;
    do {
        text = DAT_00a46054;
        switch (idx) {
        case 0: text = DAT_004bd628[m_378]; break;
        case 1: text = DAT_004bd640[m_37c]; break;
        case 2: text = DAT_004bd670[m_380]; break;
        case 4: text = DAT_004bd6a8[m_384]; break;
        case 5: text = DAT_004bd724[m_388]; break;
        }
        selected = 0;
        if (idx == sel)
            selected = 1;
        x = DAT_00951fcc->m_2c - (int)strlen(text) * (selected + 1) * 16 + DAT_00951fcc->m_24;
        y = DAT_00951fcc->m_30 * idx + DAT_00951fcc->m_28;
        if (selected)
            y -= 8;
        if (*text != 0) {
            strcpy(line, DAT_004bd5c4[selected]);
            strcat(line, text);
            M_0040bfa0(x, y, line);
        }
        idx++;
    } while (idx < 7);
    if (done < 0)
        sel = 6;
    if (done != 0) {
        if (done < 0)
            sel = 6;
        switch (sel) {
        case 3:
            DAT_00a46044 = 0;
            m_a795c = 7;
            break;
        case 4:
            DAT_00a46044 = 1;
            M_004089a0(DAT_004bd6e8[m_384]);
            break;
        case 5:
            M_00408650(DAT_004bd7a0[m_388]);
            break;
        case 6:
            DAT_00a46044 = 0;
            m_a795c = 2;
            break;
        }
    }
    if (DAT_00a46044 != 0 && sel != 4) {
        M_004089a0(-1);
        DAT_00a46044 = 0;
    }
    if (m_a795c != 6) {
        M_004010d0();
        M_00403730();
        return;
    }
    m_8ac++;
    if (m_8ac >= 1000000)
        m_8ac = 1000000;
}

// OPEN: first draft
void Game::M_00404490()
{
    char line[0x100];
    int items;
    int action;
    int tick;
    int count, i;
    int cursorX, sel, done, choice;
    int tile, credits;

    i = 0;
    items = 0;
    if (m_c != 0)
        items = 2;
    if (m_3c4 > 0)
        items++;
    action = DAT_00430658;
    tick = m_8ac;
    if (tick >= DAT_00a460bc * 15)
        action = 3;
    if (tick < 0) {
        M_00410460(-1);
        DAT_00a460c8 = 0;
        m_87fcc4 = 0;
        M_004013b0();
        M_00408660(0x2b, 1);
        M_00403730();
        M_004093b0(0, 1, DAT_004efbdc, DAT_008f06a0);
        M_004061c0(0);
        M_00406210(0, 0, 0);
        M_004062e0(0x140, 0xf0, 0, 0x1c);
        M_004061f0(7, 1);
        count = DAT_004e1eb0[items];
        M_00406240(0, count);
        count *= 2;
        switch (items) {
        case 0:
            for (i = 0; i < count; i++)
                M_004062b0(i, DAT_004e1ec0[i]);
            break;
        case 1:
            for (i = 0; i < count; i++)
                M_004062b0(i, DAT_004e1ef4[i]);
            break;
        case 2:
            for (i = 0; i < count; i++)
                M_004062b0(i, DAT_004e1f34[i]);
            break;
        case 3:
            for (i = 0; i < count; i++)
                M_004062b0(i, DAT_004e1f80[i]);
            break;
        }
    }
    M_00406600();
    M_0040a210();
    DAT_00a45e38 = 0;
    tile = M_0040bf80(0x14, 0);
    M_0040edc0(0, 0, 0x280, 0x1e0, tile);
    M_0040a8c0();
    M_004048f0(0x10, 200, 0xc0, "@0PRESS ENTER KEY");
    credits = m_3c4;
    if (credits == 0)
        credits = m_380;
    wsprintfA(line, "@2CREDIT %3d", credits);
    M_0040bfa0(0x20, 0x1c0, line);
    M_0040bfa0(0xf0, 0x1c0, "@i8@2 SEGA CORPORATION,2002");
    M_004063e0();
    M_0040ef00();
    if (action != DAT_00430658) {
        m_a795c = action;
        if (DAT_009016fc >= 0x10)
            DAT_009016fc = 0;
        DAT_00a23308 = DAT_009016fc;
        goto tail;
    }
    done = M_00406660();
    M_00406290(&cursorX, &sel);
    switch (items) {
    case 0: choice = DAT_004e1ee4[sel]; break;
    case 1: choice = DAT_004e1f20[sel]; break;
    case 2: choice = DAT_004e1f68[sel]; break;
    case 3: choice = DAT_004e1fbc[sel]; break;
    default: choice = DAT_00430658; break;
    }
    if (done <= 0)
        goto count;
    switch (choice) {
    case 1:
        m_a795c = 4;
        DAT_00a23308 = 0;
        m_3c4 = m_380;
        m_24[m_378]++;
        M_00410460(m_378);
        break;
    case 0:
        m_3c4--;
        if (m_3c4 < 0) {
            m_3c4 = 0;
            break;
        }
        m_a795c = 4;
        DAT_00a23308 = DAT_0099d778;
        if (DAT_0099d778 >= 0x12)
            DAT_00a23308 = 0x11;
        goto bump;
    case 4:
        m_a795c = 5;
        break;
    case 5:
        m_a795c = 6;
        break;
    case 2:
        m_a795c = 9;
        goto restart;
    case 3:
        m_a795c = 10;
restart:
        DAT_00a23308 = 0;
        m_3c4 = m_380;
bump:
        m_24[m_378]++;
        M_00410460(m_378);
        break;
    case 6:
        M_00404d80();
        FUN_00417fd0();
        DAT_00a460c8 = -10000;
        break;
    }
tail:
    if (m_a795c == 2) {
count:
        m_8ac++;
        if (m_8ac >= 1000000)
            m_8ac = 1000000;
    } else {
        M_004014d0(DAT_00a23308);
        M_004010d0();
        M_00403730();
    }
}

// OPEN: first draft
void Game::M_00402f10(int shift)
{
    int id;
    unsigned next;
    unsigned key;
    char keyTerm;
    unsigned char flags;
    int sel;
    int i, j, found, off;
    RomajiEntry *e;
    Slot *s;

    m_8c4->m_50ec = 0;
    if (m_8c4->m_60 != 0)
        return;
    m_8c4->m_50e8++;
    if (shift >= 5)
        m_8c4->m_50e8++;
    s = m_8c4;
    id = s->m_2894[s->m_50e8];
    next = s->m_2894[s->m_50e8 + 1];
    if (id == -1) {
        s->m_50ec = 1;
        m_8c4->m_50e8 = m_8c4->m_88;
        M_00401480(-1);
        return;
    }
    s->m_50dc = 0;
    for (i = 0; i < 12; i++)
        m_8c4->m_5094[i * KEY_LEN] = 0;
    if ((id >= 0x400 && id <= 0x409) || (id >= 0x40a && id <= 0x423)) {
        M_004034c0(m_8c4->m_5094, id);
        m_8c4->m_50dc = 0;
        return;
    }
    if (id < 0)
        return;
    if (id > 0x19d)
        return;
    e = &DAT_00429040[id & 0xffff];
    flags = e->pad23;
    sel = id & 0xffff;
    if (sel >= 0xcf)
        sel -= 0xcf;
    m_8c4->m_50dc = m_3c[sel];
    strcpy(m_8c4->m_5094, e->rom[0]);
    strcpy(m_8c4->m_5094 + 6, e->rom[1]);
    strcpy(m_8c4->m_5094 + 12, e->rom[2]);
    strcpy(m_8c4->m_5094 + 18, e->rom[3]);
    if (flags != 0 && next != 0xffffffff) {
        keyTerm = 0;
        switch (next) {
        case 0:
            if ((flags & 0x80) == 0)
                goto search;
            goto load;
        default:
            key &= 0xffffff00;
            goto search;
        case 2: flags &= 0x40; break;
        case 4: flags &= 0x20; break;
        case 6: flags &= 0x10; break;
        case 8: flags &= 8; break;
        case 0x42: flags &= 4; break;
        case 0x44: flags &= 2; break;
        case 0x46: flags &= 1; break;
        }
        if (flags != 0) {
load:
            key = *(unsigned *)&m_8c4->m_1494[m_8c4->m_50e8 * 2];
        }
search:
        found = 0x55;
        if (DAT_00429040[0x55].kana[0] != 0) {
            e = &DAT_00429040[0x55];
            do {
                if ((unsigned char)key == 0)
                    break;
                if (strcmp(e->kana, (char *)&key) == 0) {
                    strcpy(m_8c4->m_5094 + 0x1e, e->rom[0]);
                    strcpy(m_8c4->m_5094 + 0x24, e->rom[1]);
                    strcpy(m_8c4->m_5094 + 0x2a, e->rom[2]);
                    strcpy(m_8c4->m_5094 + 0x30, e->rom[3]);
                    break;
                }
                e++;
                found++;
            } while (e->kana[0] != 0);
        }
    }
    if ((short)id == 0x22 && next != 0xffffffff && next != 1 && next != 7 && next != 9) {
        j = 0;
        off = 0x50a6;
        do {
            switch (DAT_00429040[next & 0xffff].rom[j][0]) {
            case 0:
            case 'a':
            case 'e':
            case 'i':
            case 'n':
            case 'o':
            case 'u':
                break;
            default:
                strncpy((char *)m_8c4 + off, DAT_00429040[next & 0xffff].rom[j], 1);
                *((char *)m_8c4 + off + 1) = 0;
                if (strcmp((char *)m_8c4 + off, (char *)m_8c4 + off - 6) != 0)
                    off += 6;
                if (off > 0x50b1)
                    j = 99;
            }
            j++;
        } while (j < 4);
    }
}
