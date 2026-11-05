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
