#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

// OPEN: 782 vs 780 bytes. Flow and strings match; the original keeps the READY-lines loop on a char** that starts at the
// table (mine is strength-reduced to table+4), and edi (row) is saved/restored around the middle section only, mine saves it
// in the prologue because a constant 1 gets a register for the later "== 1" test (register/schedule).
void Game::M_0040c340()
{
    char text[256];
    char tens[64];
    char ones[16];
    int stage;
    int fill;
    char *name;
    int row;
    char **line;

    M_0040bfa0(0x2c, 0x1c, "@i0");
    M_0040bfa0(0x17c, 0x1c, "@i1");
    wsprintfA(text, "@0%7d0", m_3a4);
    M_0040bfa0(0x70, 0x20, text);
    wsprintfA(text, "@1%7d0", m_3a0);
    M_0040bfa0(0x1e0, 0x20, text);
    stage = *m_3d8 + 1;
    if (m_398 != 0)
        stage = 0x12;
    wsprintfA(text, "@2STAGE%3d", stage);
    M_0040bfa0(0x1c0, 0x1c0, text);
    if (m_3c0 > 0xa0)
        m_3c0 = 0xa0;
    if (m_3c0 < 0)
        m_3c0 = 0;
    fill = m_3c0 - 1;
    if (fill < 0)
        fill = 0;
    strcpy(tens, DAT_004e267c);
    strcpy(ones, DAT_004e2680);
    tens[fill / 10] = 0;
    ones[fill % 10] = 0;
    wsprintfA(text, "@i%s%s", tens, ones);
    M_0040bfa0(0x30, 0x1b0, text);
    if (m_a795c != 3 && DAT_008f1004 != 0) {
        row = 0xb0;
        if (*DAT_004e2684[0] != 0) {
            line = DAT_004e2684;
            name = *line;
            do {
                M_0040bfa0(0x80, row, name);
                name = line[1];
                row += 0x20;
                line++;
            } while (*name != 0);
        }
    }
    if (m_a795c != 3 && DAT_00a45e28 == 1)
        M_0040bfa0(0xa0, 0xc0, "@aGAME OVER");
    if (m_87fcac > 0) {
        name = DAT_0043065c[*m_3d8];
        if (*name == '*') {
            M_0040c250(0xa0, DAT_004e26a0);
            M_0040c250(0xd0, DAT_004e26a4);
            return;
        }
        if (m_87fcb0 < 1) {
            if (m_398 == 0) {
                wsprintfA(text, DAT_004e2694, *m_3d8 + 1);
                M_0040c250(0xa0, text);
            }
            wsprintfA(text, DAT_004e2698, name);
            M_0040c250(0xd0, text);
        }
        if (m_87fcb0 >= 0 && m_398 == 0) {
            if (*DAT_004306c0[m_87fcb0] != '*' && *DAT_004306c0[m_87fcb0] != 0) {
                wsprintfA(text, DAT_004e269c, DAT_004306c0[m_87fcb0]);
                M_0040c250(0x1b0, text);
            }
        }
    }
}

// OPEN: same 312 bytes; only the load schedule differs: the original loads bank->m_10 before the index expression and bank->m_14
// after DAT_004f052c->m_b0 (operand evaluation order of the two list reads), mine hoists them differently (register/schedule).
int Game::M_0040d2c0(int index, int bank, int n, int a4)
{
    int per;
    int start;
    int pick;
    int i;
    int id;
    char *reading;
    char *text;

    FUN_0040d130(bank);
    if (n != 0 && DAT_00a45e34->m_0c != 0) {
        per = DAT_00a45e34->m_0c / n;
        if (DAT_004f052c->m_b0 == DAT_00430658) {
            start = rand() % per * n;
            for (i = 0; i < n; i++)
                DAT_004f052c->m_b4[i] = i;
            DAT_004f052c->m_b0 = start;
        }
        pick = DAT_004f052c->m_b4[n - DAT_004f052c->m_08] + DAT_004f052c->m_b0;
        reading = DAT_00a45e34->m_10[pick];
        text = DAT_00a45e34->m_14[pick];
        id = M_0040dd60(index, reading, text, M_0040c970(text), a4);
        m_8a3cc8[index][m_8a2cc8[index]] = (short)id;
        m_8a2cc8[index]++;
        m_8c4->m_c = m_8a2cc8[index] - 1;
        m_8c4->m_10 = m_882cc8[index][m_8c4->m_c];
        m_8c4->m_64 = 0;
        return id;
    }
    return M_0040d160(index, bank, a4);
}

// OPEN: 303 bytes identical except one register: the original indexes DAT_004e2774 through eax, mine through ecx
// (register/schedule, one operand).
void Game::M_0040ef00()
{
    PaletteTable palette;
    int level;
    int phase;

    FUN_004139f0(&palette);
    palette.entry[255].b = 0xff;
    palette.entry[255].g = 0xff;
    palette.entry[255].r = 0xff;
    phase = DAT_004e2774[m_8a8 % 32u];
    if (m_a795c == 7) {
        level = phase / 4 + 0xbf;
        if (level < 0)
            level = 0;
        else if (level > 0xff)
            level = 0xff;
        palette.entry[8].r = level;
        palette.entry[8].g = 0x20;
        palette.entry[8].b = level;
    } else {
        level = phase / 2 + 0xbf;
        if (level < 0)
            level = 0;
        else if (level > 0xff)
            level = 0xff;
        palette.entry[8].r = level;
        palette.entry[8].b = 0;
        palette.entry[8].g = 0;
    }
    level = phase + 0xe7;
    if (level < 0)
        level = 0;
    else if (level > 0xff)
        level = 0xff;
    palette.entry[54].r = level;
    if (m_a795c == 2) {
        level = phase + 0xf7;
        if (level < 0)
            level = 0;
        else if (level > 0xff)
            level = 0xff;
        palette.entry[201].r = level;
        level = phase + 0xc6;
        if (level < 0)
            level = 0;
        else if (level > 0xff)
            level = 0xff;
        palette.entry[202].r = level;
    }
    FUN_00413a20(&palette);
    FUN_00413a50();
}

// OPEN: 284 vs 277 bytes. Loop shape right (for with break), the original keeps this in edi and run/len/sprite in
// esi/ebp/ebx with n stored back into its parameter slot; mine swaps them (register class). Random declaration permutations (290) did not hit.
int Game::M_0040f720(int n)
{
    int best;
    int pass;
    int count;
    int rec;
    int run;
    int len;
    int sprite;
    int *entry;

    rec = DAT_004bde14[n * 0x16];
    best = 0;
    pass = 0;
    n = DAT_004efbd8[rec];
    count = DAT_008f069c[rec];
    if (count > 0) {
        for (; pass < count; pass++, n++) {
            if (n >= m_87cbc4->m_7d5214)
                break;
            M_0040af40(3, n);
            for (entry = DAT_004e27f8; entry < DAT_004e27f8 + 3; entry++) {
                run = 0;
                len = 0;
                sprite = m_87cbd4->m_2c[*entry];
                do {
                    if (sprite >= m_87cbc4->m_7d5210) {
                        run = 0;
                        break;
                    }
                    M_0040aff0(5, sprite);
                    run += m_87cbec->m_c;
                    if (m_87cbec->m_10 == 10000)
                        break;
                    if (m_87cbec->m_10 <= 0) {
                        run = 0;
                        break;
                    }
                    len++;
                    sprite++;
                } while (len < 0x400);
                if (best < run)
                    best = run;
            }
        }
        if (best != 0)
            return best;
    }
    return DAT_00a460bc << 10;
}

// OPEN: 391 bytes identical size; the original holds m_7d522c in ebx until the loop and stores it to the stack just before ebx is reused
// (register/schedule); `y > top` and the if/else table copy loop already match.
int Game::M_0040fab0()
{
    int x;
    int top;
    int scale;
    int slot;
    int span;
    int elapsed;
    int i;
    int y;

    m_8b8 = -1;
    x = m_87cbc4->m_7d522c;
    scale = m_87cbc4->m_7d5230;
    M_0040b940(m_87cbc4->m_7d5228);
    slot = M_00409330(2, 6);
    m_87cbc4->m_7d5228 = slot;
    if (slot <= 0)
        return DAT_00430658;
    if (M_00410360(0x17) == DAT_00430658)
        return DAT_00430658;
    top = DAT_004e28b4;
    span = top - DAT_004e2860[0];
    elapsed = top - scale;
    for (i = 0; i < 0x16; i++) {
        y = top - (top - DAT_004e2860[i]) * elapsed / span;
        if (y > top)
            y = top;
        if (i == 0x15)
            x = 0x140;
        M_0040b0e0(DAT_0099d5bc[i], 1, x, y, DAT_004e2804[i], DAT_0099d5bc[i + 1] - DAT_0099d5bc[i]);
    }
    M_0040b0e0(DAT_0099d5bc[22], 1, 0x140, DAT_004e28b8, DAT_004e285c, 10000);
    M_0040af40(3, slot);
    m_87cbd4->m_2c[14] = DAT_0099d5bc[0];
    m_87cbc4->m_7d5228 = slot;
    for (i = 25; i < 0x59; i++) {
        if (i < 0x30)
            m_87cbd4->m_2c[i] = DAT_0099d5bc[i - 25];
        else
            m_87cbd4->m_2c[i] = 0;
    }
    M_00408650(0x1a);
    return 0;
}

// OPEN: 295 vs 303 bytes. Original keeps `base` (cond ? 4 : 6, compared with the register that holds 10) in ebx and the fill
// bytes are built in edx; this version lets the compiler fold row into the fill byte (register/schedule + cast shape).
void Game::M_0040ff90(int points)
{
    int counts[2];
    char digits[16];
    char text[32];
    int *count;
    char base;
    char fill;
    char row;
    int y;

    counts[0] = points / 10;
    counts[1] = points % 10;
    base = *m_3d8 + 1 < 10 ? 6 : 4;
    row = 0;
    count = counts;
    for (y = 0xcd; y < 0x10d; y += 0x20) {
        if (*count > 0) {
            fill = base - row + '1';
            memset(digits, fill, *count);
        }
        digits[*count] = 0;
        strcpy(text, DAT_004ebb00);
        strcat(text, digits);
        M_0040bfa0(0xf0, y, text);
        row++;
        count++;
    }
}
