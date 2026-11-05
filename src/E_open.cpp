#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
// OPEN: wall: movsx+and 1 kept by original, VC6 narrows to mov al (type of flags unknown)
int FUN_004189a0(int n)
{
    if (n < 0)
        n = 0;
    if (n >= 10)
        n = 9;
    return DAT_00a4b134[n].flags & 1;
}

// OPEN: inc edi after mov edi,eax vs lea edi,[eax+1]
void FUN_0041a820(char *dir)
{
    char *src;
    char *p;

    src = GetCommandLineA();
    src++;
    strcpy(dir, src);
    p = dir + strlen(dir) - 2;
    while (p > dir) {
        if (*p == 0x5c) {
            p[1] = 0;
            return;
        }
        p--;
    }
    *p = 0;
}

// OPEN: store through shl eax,3 + abs address not reproduced
int FUN_0041ad10(int on, int col, int row, unsigned char key, const char *name, int kind)
{
    int slot;

    if (on == 1 && (kind < 0 || kind > 4))
        return 0;
    if (col >= 0 && col < 13 && row >= 0 && row < 4 && !(col == 11 && row == 3) && !(col == 12 && row != 0)) {
        slot = col + row * 13;
    } else {
        if (!FUN_0041adc0(key, name, &col, &row))
            return 0;
        slot = col + row * 13;
    }
    DAT_0101e350[0][slot].on = on;
    DAT_0101e350[0][slot].kind = kind;
    return 1;
}

// OPEN: register allocation (key in al, name in ebx)
int FUN_0041adc0(unsigned char key, const char *name, int *col, int *row)
{
    int i;
    const char *entryName;

    for (i = 0; i < 48; i++) {
        entryName = DAT_004ec3d8[i].name;
        if (key != 0 && DAT_004ec3d8[i].key != 0 && DAT_004ec3d8[i].key == key) {
            *col = DAT_004ec3d8[i].col;
            *row = DAT_004ec3d8[i].row;
            return 1;
        }
        if (name != 0 && entryName != 0 && strcmp(entryName, name) == 0) {
            *col = DAT_004ec3d8[i].col;
            *row = DAT_004ec3d8[i].row;
            return 1;
        }
    }
    return 0;
}

// OPEN: tail register rotation ecx/edx/eax, 2 bytes larger
int FUN_0041c8a0(int key)
{
    int result = 4;
    int sel;
    int i;
    int step;

    DAT_0101fdc0 = 400;
    DAT_0101e554 = 0;
    switch (key) {
    case 0x28:
        FUN_0041d950(0);
        DAT_0101fdc4 = (DAT_0101fdc4 + 1) % 4;
        break;
    case 0x26:
        FUN_0041d950(0);
        DAT_0101fdc4 = (DAT_0101fdc4 + 3) % 4;
        break;
    case 0xd:
        DAT_0101e554 = 1;
        sel = DAT_0101fdc4;
        DAT_0101fdb4 = sel;
        DAT_0101fdbc = DAT_004ec798[sel];
        result = sel;
        if (sel == 3) {
            DAT_0101e5d0[2] = 0;
            DAT_0101e5d0[1] = 0;
            DAT_0101e5d0[0] = 0;
            DAT_0101fdc4 = 0;
        } else {
            FUN_0041d950(0);
            sel = DAT_0101fdc4;
            DAT_0101fdc4 = 3;
            DAT_0101e5d0[sel] = 1;
            for (i = 0; i < 3; i++) {
                if (DAT_0101e5d0[i] == 0) {
                    DAT_0101fdc4 = i;
                    break;
                }
            }
        }
        break;
    }
    if (DAT_0101e554 == 0) {
        step = DAT_004ec798[DAT_0101fdc4] - DAT_0101fdbc;
        DAT_0101fdbc += (step < 0 ? step - 1 : step + 1) / 2;
    }
    FUN_0041cb30(DAT_004ec7b8, 0x50, 0x32, 0);
    FUN_0041cb30(DAT_004ec7a8[0], 0x196, 0x38, 0);
    FUN_0041cb30(DAT_004ec7a8[1], 0x196, 0x5c, 0);
    FUN_0041cb30(DAT_004ec7a8[2], 0x196, 0x80, 0);
    FUN_0041cb30(DAT_004ec7a8[3], 0x196, 0xa4, 0);
    FUN_0041c7d0(0xfa, 0x34);
    FUN_0041cac0(0, 400, 0x32);
    FUN_0041cac0(1, 400, 0x56);
    FUN_0041cac0(2, 400, 0x7a);
    FUN_0041cac0(3, 400, 0x9e);
    FUN_0041caf0(DAT_0101fdc0, DAT_0101fdbc);
    FUN_0041cb10(DAT_0101fdc0 - 0xc, DAT_004ec798[DAT_0101fdc4] + 6);
    return result;
}

// OPEN: register allocation (ebp/esi/edi), x/y locals
int FUN_0041cc80()
{
    int dx;
    int dy;
    int x;
    int y;
    int sel;
    int moving;

    sel = DAT_0101fdb4;
    x = DAT_0102230c;
    y = DAT_01022314;
    if (x == 0 && y == 0) {
        y = DAT_004ec798[sel];
        x = 400;
    }
    dx = 0x50 - x;
    dy = 0x32 - y;
    if (dx < 0)
        dx--;
    else
        dx++;
    x += dx / 2;
    y += (dy < 0 ? dy - 1 : dy + 1) / 2;
    DAT_0102230c = x;
    DAT_01022314 = y;
    if (x == 0x50 && y == 0x32)
        moving = 0;
    else
        moving = 1;
    FUN_0041cb30(DAT_004ec7a8[sel], x + 6, y + 6, 0);
    if (moving == 0) {
        FUN_0041c860(DAT_0101fdb4, 0);
        FUN_0041c7b0(1);
        FUN_0041c7d0(0xd0, 0x3a);
        return 0;
    }
    FUN_0041cac0(DAT_0101fdb4, DAT_0102230c, DAT_01022314);
    FUN_0041c7b0(0);
    return moving;
}

// OPEN: param i used as slot copy; register allocation, 311 vs 304 bytes
void FUN_0041ce30(int i, int on, int col, int row)
{
    int slot = i;
    unsigned int label = i;
    int advance;
    const KeyDef *def;

    if (slot >= 0 && slot < 12) {
        DAT_01121560[slot] = on;
        if (on == 1) {
            for (def = DAT_004ec3d8; def < &DAT_004ec3d8[48]; def++) {
                if (def->col == col && def->row == row) {
                    for (label = 0; label < 48; label++) {
                        if (strcmp(def->name, DAT_004ec978[label].text) == 0)
                            break;
                    }
                    if (label < 48)
                        break;
                }
            }
            if (label >= 0x30)
                return;
            if (FUN_00413610((const unsigned char *)DAT_004ec978[label].text, DAT_0101fdd0->m_34, &i, &col, &row, &advance) == 0)
                return;
            DAT_0101fdd8[slot][0].m_0c = i;
            DAT_0101fdd8[slot][0].m_10 = col;
            DAT_0101fdd8[slot][0].m_14 = row;
            DAT_0101fdd8[slot][0].m_18 = advance;
        }
        FUN_0041cf60(slot, on);
    }
}

// OPEN: original loop is not peeled; mine is (constant initial slot)
int FUN_0041d950(int n)
{
    char path[2048];
    int slot;
    int next;
    int i;

    if (n >= 0 && n <= 0x30) {
        sprintf(path, "%s%s", DAT_0101eb04, DAT_004ec3cc[n]);
        slot = 0;
        for (;;) {
            next = slot + 1;
            if (FUN_004189a0(next) == 0)
                break;
            slot = next;
            if (slot >= 3)
                break;
        }
        if (slot == 3)
            slot = 0;
        for (i = 0; i < 3; i++) {
            if (FUN_004185a0(path, slot + 1, 1, 0, 0) != 0)
                return 1;
        }
    }
    return 0;
}

// OPEN: 0x41cb30 shl-form stores now match (direct array indexing); remaining: prologue (original has two address-taken locals, mine one), `and ecx,0xffffffe0` vs `and cl,0xe0`, loop-top count compare
int FUN_0041cb30(const unsigned char *text, int x, int y, int fade)
{
    const unsigned char *p = text;
    int alpha = fade;
    int height;
    int extra;

    if (*p != 0) {
        do {
            if (DAT_0101e56c >= 0x200)
                return 0;
            if (FUN_00413610(p, DAT_0101fdd0->m_34, &fade, &height, (int *)&text, &extra) == 0)
                return 0;
            DAT_01022320[DAT_0101e56c].m_0c = fade;
            DAT_01022320[DAT_0101e56c].m_10 = height;
            DAT_01022320[DAT_0101e56c].m_14 = (int)text;
            DAT_01022320[DAT_0101e56c].m_18 = extra;
            DAT_01022320[DAT_0101e56c].m_1c = x + alpha * 12 / 255;
            DAT_01022320[DAT_0101e56c].m_20 = y;
            if ((*p & 0xe0) == 0x80)
                p++;
            p++;
            x += (int)text;
            DAT_0101e56c++;
        } while (*p != 0);
    }
    return 1;
}

// OPEN: 0x41d460 first attempt

bool FUN_0041d460()
{
    char label[8];
    char line[100];
    int cur = DAT_0101e570;
    int row = cur;
    int tab = cur;
    bool inRange = cur < 0xb0;
    int counter = DAT_0101fda8;
    int state;
    int tens;
    int hundreds;
    int i;
    int y;
    int labelX = 0x32;
    int labelY = 0xd4;
    const char *head;

    if (cur >= 0xe) {
        row = 0xd;
        tab = 0xe;
    }
    DAT_011215a0++;
    state = DAT_0101e270[row].state;
    if (state == 0) {
        if (DAT_004ec888[tab].wait0 < DAT_011215a0) {
            DAT_0101e270[row].state = 1;
            DAT_011215a0 = 0;
        }
    } else if (state == DAT_0101e270[row].len) {
        if (DAT_004ec888[tab].wait2 < DAT_011215a0) {
            if (inRange) {
                DAT_0101e570++;
                cur = DAT_0101e570;
            }
            DAT_011215a0 = 0;
        }
    } else if (DAT_004ec888[tab].wait1 < DAT_011215a0) {
        if (DAT_004ec888[tab].slow == 0 || state < 0xf) {
            DAT_0101e270[row].state = state + 1;
        } else {
            if (state == 0xf) {
                counter = 0;
                DAT_0101e270[row].state = 0x12;
                DAT_0101fda8 = 0;
            } else if (DAT_0101fda8 < 100) {
                counter = DAT_0101fda8 + 1;
                DAT_0101fda8 = counter;
            } else {
                DAT_0101e270[row].state = state + 1;
            }
            hundreds = counter / 100;
            head = DAT_004e2d68;
            if (hundreds == 0)
                head = DAT_004ecf78;
            strcpy(label, head);
            tens = counter % 100 / 10;
            if (tens <= 0 && hundreds == 0)
                strcat(label, DAT_004ecf78);
            else
                strcat(label, DAT_004ecaf8[tens]);
            strcat(label, DAT_004ecaf8[counter % 10]);
            strncpy(DAT_004ec850[row] + 0x1c, label, 6);
            cur = DAT_0101e570;
        }
        DAT_011215a0 = 0;
    }
    if (cur - 9 < 0)
        y = 0x32;
    else
        y = (cur - 9) * -0x12 + 0x32;
    for (i = 0; i < 14; i++) {
        if (y >= 0x32 && y <= 0xd4) {
            if (DAT_0101e270[i].state != 0) {
                strncpy(line, DAT_004ec850[i], DAT_0101e270[i].state * 2);
                line[DAT_0101e270[i].state * 2] = 0;
                FUN_0041cb30((unsigned char *)line, 0x32, y, 0);
            }
            if (i == DAT_0101e570) {
                labelX = DAT_0101e270[i].state * 12 + 0x32;
                labelY = y;
            }
        }
        y += 0x12;
    }
    FUN_0041c7d0(labelX, labelY);
    return inRange;
}
