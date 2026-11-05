#include <windows.h>
#include <string.h>
#include "game.h"

// OPEN: 0040bfa0 (466 B code + tables): body, both switch tables (byte index + dword jump table) and block layout match; remaining
// difference is the loop test. The original loads *text for the test without keeping it (cmp byte ptr [ecx],0) and reloads it as the
// glyph character at the top of the body; mine forwards the loaded value (CSE) so the top load disappears. 63% instruction similarity.
// Wall class: masked/referent (load redundancy). Unknown chars are treated as ' ' (c = ' ') to get the jump to the shared advance block.
void Game::M_0040bfa0(int x, int y, char *text)
{
    int font = 5;
    int glyph;
    int index;
    int width;
    char c;
    char *rest;
    int boxed;

    for (; *text != 0; text++) {
        c = *text;
        switch (c) {
        case '?':
            glyph = 0x24;
            break;
        case '!':
            glyph = 0x25;
            break;
        case '.':
            glyph = 0x26;
            break;
        case '<':
            glyph = 0x27;
            break;
        case ',':
            glyph = 0x28;
            break;
        case '@':
            text++;
            switch (*text) {
            case 0:
                return;
            case 'A':
            case 'a':
                font = 3;
                break;
            case 'B':
            case 'b':
                font = 4;
                break;
            case '0':
                font = 5;
                break;
            case '1':
                font = 6;
                break;
            case '2':
                font = 7;
                break;
            case '3':
                font = 8;
                break;
            case '4':
            case 'R':
            case 'r':
                font = 9;
                break;
            case 'i':
                font = 10;
                break;
            case 'K':
            case 'k':
                rest = text;
                boxed = *rest++ == 'K';
                if (boxed)
                    M_004022e0(x - 4, y - 4, strlen(rest) * 10 + 8, 0x1c, 2);
                M_0040f1c0(x, y, rest);
                return;
            }
            continue;
        default:
            if (c >= '0' && c <= '9')
                glyph = c - '0';
            else if (c >= 'a' && c <= 'z')
                glyph = c - 'a' + 10;
            else if (c >= 'A' && c <= 'Z')
                glyph = c - 'A' + 10;
            else if (c == '-')
                glyph = 0x27;
            else
                c = ' ';
            break;
        }
        if (c == ' ') {
            x += M_0040be00(M_0040bf80(font, 0));
        } else {
            index = M_0040bf80(font, glyph);
            width = M_0040be00(index);
            M_0040bea0(x, y, index);
            x += width;
        }
    }
}

// OPEN: 0040e570 (560 B): 563 vs 560 bytes. The original keeps the table row pointer (&DAT_0042e32c[group]) in a stack slot and reads
// group from its stack slot at every use; mine keeps group in a register (esi) and the loop condition re-derives the row. Also this/i
// are ebx/esi in the original, ebp/ebx in mine. Wall class: register/schedule. 57% similarity.
int Game::M_0040e570(int group, int first)
{
    char suffix[0x10];
    char name[0x108];
    int i;
    int *total;
    unsigned int limit;
    int **slot;
    int index;
    int j;
    int *image;

    i = 0;
    limit = 0x60000;
    total = &DAT_0042e32c[group];
    if (DAT_0042e32c[group] > 0) {
        slot = &m_8eced8[first];
        do {
            if (M_0040e340(first + i) < 0)
                return DAT_00430658;
            i++;
            wsprintfA(suffix, "_%02d", i);
            strcpy(name, DAT_0042e12c[group]);
            strcat(name, suffix);
            strcat(name, DAT_0042e124);
            index = DAT_00430658;
            for (j = 0; DAT_0042e52c[j][0] != 0; j++) {
                if (strcmp(name, DAT_0042e52c[j]) == 0) {
                    index = j;
                    break;
                }
            }
            if (index == DAT_00430658)
                return DAT_00430658;
            limit = 0x60000;
            if (FUN_00411e80(0, DAT_004f0658, DAT_004f0654, DAT_004efa38, &limit, index) == 0)
                return DAT_00430658;
            m_8eced4 = m_21ef798;
            image = FUN_004129a0(DAT_004efa38, m_8eced0, &m_8eced4);
            *slot = image;
            m_8ef6d8 = image;
            if (image == 0)
                break;
            slot++;
            m_8eced0 = m_8eced4;
            m_8eced4 = m_21ef798;
        } while (i < *total);
    }
    return *total;
}

// OPEN: 0040dd60 (speech balloon setup, 1329 B): 81% instruction similarity, flow identical. The original spills the box width, box height and
// the two size values to stack slots (frame 0x24, mine 0x10), keeps the zero register in ebx (mine esi), and stores the default centre
// (0x140, 0xf0) into the dead parameter slot of 'text' for M_00401d70's out-pointer (parameter-slot reuse, as seen in lane E's 0x41ce30).
// Needs request: game_methods_C.txt 'void M_00410440(int index)' must return int. Wall class: register/schedule plus stack layout.
int Game::M_0040dd60(int index, char *reading, char *text, int weight, int lines)
{
    int star;
    int limit;
    int cells;
    int length;
    int rows;
    int textCols;
    int textRows;
    int textW;
    int textH;
    int sizes[2];
    int boxW;
    int boxH;
    int cx;
    int cy;
    int left;
    int top;
    int k;

    if (*text == 0)
        return DAT_00430658;
    star = 0;
    if (*text == '*')
        star = 1;
    k = M_00401930();
    if (k < 0)
        return k;
    limit = M_0040f720(index);
    m_8c4->m_0 = 1;
    m_8c4->m_60 = star;
    strcpy(m_8c4->m_1494, text);
    strcpy(m_8c4->m_94, reading);
    m_8c4->m_88 = strlen(m_8c4->m_1494) / 2;
    m_8c4->m_8 = index;
    m_8c4->m_84 = weight;
    if (m_8c4->m_84 > limit)
        m_8c4->m_84 = limit;
    m_8c4->m_7c = 5;
    m_8c4->m_68 = 1;
    m_8c4->m_6c = 0;
    m_8c4->m_78 = lines;
    m_8c4->m_70 = 0;
    m_8c4->m_74 = 0;
    M_00402e90();
    M_00402f10(0);
    cells = strlen(reading) / 2;
    rows = (cells - 1) / 25 + 1;
    m_8c4->m_8c = 25;
    m_8c4->m_90 = 6;
    if (cells < 25)
        m_8c4->m_8c = cells;
    if (rows < 6)
        m_8c4->m_90 = rows;
    m_8c4->m_50e0 = -1;
    if (M_00410440(m_8c4->m_8)) {
        m_8c4->m_18 = 1;
        m_8c4->m_14 = 1;
    } else {
        m_8c4->m_18 = 0;
        m_8c4->m_14 = 0;
    }
    if (m_398 != 0) {
        m_8c4->m_18 = 0;
        m_8c4->m_14 = 0;
    }
    length = strlen(reading);
    textCols = length;
    if (length >= 50)
        textCols = 50;
    textRows = (length - 1) / 50 + 1;
    if (textRows > 6)
        textRows = 6;
    textW = textCols * 10;
    if (textRows > 1)
        textH = textRows * 24 - 4;
    else
        textH = 20;
    sizes[0] = M_00402a30();
    sizes[1] = M_00402a50();
    if (!star) {
        if (m_8c4->m_18 != 0) {
            for (k = 0; k < 2; k++) {
                if (textW < sizes[k])
                    textW = sizes[k];
            }
        } else if (textW < sizes[0]) {
            textW = sizes[0];
        }
    }
    boxW = textW + 8;
    if (star)
        boxH = textH + 8;
    else
        boxH = textH + 40;
    cx = 0x140;
    cy = 0xf0;
    if (M_00401da0(m_8c4->m_8) == 0)
        M_00401d70(index, &cx, &cy);
    left = cx - boxW / 2;
    top = cy - boxH / 2;
    if (left < 0x10) {
        cx += 0x10 - left;
        left = 0x10;
    }
    if (left + boxW >= 0x260) {
        cx += 0x260 - left - boxW;
        left = 0x260 - boxW;
    }
    if (top < 0) {
        cy -= top;
        top = 0;
    }
    if (top + boxH >= 0x1b0) {
        cy += 0x1b0 - top - boxH;
        top = 0x1b0 - boxH;
    }
    m_8c4->m_1c = left;
    m_8c4->m_20 = top;
    m_8c4->m_24 = boxW;
    m_8c4->m_28 = boxH;
    m_8c4->m_3c = cx - textW / 2;
    m_8c4->m_40 = top + 4;
    m_8c4->m_2c = cx;
    m_8c4->m_30 = m_8c4->m_40 + textH + 4;
    m_8c4->m_34 = cx - sizes[0] / 2;
    m_8c4->m_38 = m_8c4->m_30 + 8;
    if (m_8c4->m_18 == 0 && !star) {
        m_8c4->m_28 = boxH - 12;
        m_8c4->m_38 = m_8c4->m_40 + textH + 4;
    }
    m_8c4->m_3c -= m_8c4->m_1c;
    m_8c4->m_40 -= m_8c4->m_20;
    m_8c4->m_2c -= m_8c4->m_1c;
    m_8c4->m_30 -= m_8c4->m_20;
    m_8c4->m_34 -= m_8c4->m_1c;
    m_8c4->m_38 -= m_8c4->m_20;
    m_8c4->m_4c = 0;
    m_8c4->m_48 = 0;
    m_8c4->m_44 = 0;
    m_8c4->m_58 = m_8c4->m_1c;
    m_8c4->m_50 = m_8c4->m_58;
    m_8c4->m_5c = m_8c4->m_20;
    m_8c4->m_54 = m_8c4->m_5c;
    return m_8c0;
}
