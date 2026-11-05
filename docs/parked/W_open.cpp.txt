#include <stdio.h>
#include <string.h>
#include "game.h"

// OPEN: 0x40dd60 (speech balloon setup, 1329 B): 88.6% instruction similarity, same size, frame 0x24. The frame only fits when
// the box metrics live in one int[7] (box[0]=textW, [1]=textH, [2..3]=option sizes, [4..6] unused slots): as separate scalars VC6 puts
// textW/textH into the dead reading/weight parameter slots and the frame is 0x10. The 3 unused slots are real in the original (nothing
// reads them), so the true source has a larger local array or struct that this draft only imitates. Residue: the original keeps the zero in ebx
// and reading in esi; mine swaps them (register/schedule). Needs int M_00410440 (game_methods_C.txt edited). Permuter 4 min: best 54.

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
    int box[7];
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
    if (textRows >= 6)
        textRows = 6;
    box[0] = textCols * 10;
    if (textRows > 1)
        box[1] = textRows * 24 - 4;
    else
        box[1] = 20;
    box[2] = M_00402a30();
    box[3] = M_00402a50();
    if (!star) {
        if (m_8c4->m_18 != 0) {
            for (k = 0; k < 2; k++) {
                if (box[0] < box[2 + k])
                    box[0] = box[2 + k];
            }
        } else if (box[0] < box[2]) {
            box[0] = box[2];
        }
    }
    boxW = box[0] + 8;
    if (star)
        boxH = box[1] + 8;
    else
        boxH = box[1] + 40;
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
    m_8c4->m_3c = cx - box[0] / 2;
    m_8c4->m_40 = top + 4;
    m_8c4->m_2c = cx;
    m_8c4->m_30 = m_8c4->m_40 + box[1] + 4;
    m_8c4->m_34 = cx - box[2] / 2;
    m_8c4->m_38 = m_8c4->m_30 + 8;
    if (m_8c4->m_18 == 0 && !star) {
        m_8c4->m_28 = boxH - 12;
        m_8c4->m_38 = m_8c4->m_40 + box[1] + 4;
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


// OPEN: 0x409700 (stage-object spawn, 1435 B, ret 0xc): 71.8% similar, same 1438 vs 1438 size, frame 0x138. All control flow is
// transcribed (buf[0x40] of M_0040bbe0 ids, flag==0 short path, flag!=0 path with the 1/1024 fixed-point fan, table DAT_004e246c). Residue: stack
// slot layout of the scalars (original: x/y outs at 0x30/0x34, w/h at 0x14/0x18, a copy of x in 0x2c later reused for delta*4) and the
// register that holds x. Needs int return (game_methods_B.txt edited). Permuter 5 min: best 69.

int Game::M_00409700(int a, int b, int flag)
{
    int buf[0x40];
    int count;
    int x;
    int y;
    int cx;
    int w;
    int h;
    int len;
    int tick;
    int delta;
    int step;
    int sx;
    int sw;
    int sh;
    int rows;
    int saved;
    int i;
    int j;
    int span;
    int t;
    int v;
    int *table;

    M_0040af40(0, a);
    M_0040af40(3, b);
    if (m_87cbc8->m_10 != 0)
        return 1;
    M_0040b4d0(a, 1, &x, &y);
    cx = x;
    len = M_00409300(a);
    M_0040b4d0(a, 2, &w, &h);
    sx = x - 320;
    w *= 2;
    h *= 2;
    if (len <= 0x146)
        len = 0x14a;
    tick = M_004096b0(len);
    if (tick < 0)
        return DAT_00430658;
    delta = 0x14 - tick;
    if (flag != 0) {
        count = delta * 2 + 4;
        if (count > 0x40)
            count = 0x40;
    } else
        count = 4;
    for (i = 0; i < count; i++) {
        buf[i] = M_0040bbe0();
        if (buf[i] == 0)
            return DAT_00430658;
    }
    M_0040af40(0, a);
    M_0040af40(3, b);
    if (flag == 0) {
        m_87cbc8->m_4 = 0x36;
        m_87cbc8->m_8 = 0x36;
        M_0040b0e0(buf[0], 1, x, y, 0, 1);
        M_0040b0e0(buf[1], 1, x, y, 1, 0);
        M_0040b0e0(buf[2], 1, w, h, 0, 1);
        M_0040b0e0(buf[3], 1, w, h, 1, 0);
        for (i = 0; i < 25; i++) {
            if (m_87cbd4->m_2c[i] > 0)
                (&m_87cbc8->m_2c)[i] = m_87cbd4->m_2c[i];
        }
        m_87cbc8->m_64 = buf[0];
        m_87cbc8->m_6c = buf[2];
        m_87cbc8->m_18 = 0;
    } else {
        span = len - 0x146;
        if (span <= 0) {
            len = 0x14a;
            span = 4;
        }
        sx = (sx << 10) / span;
        sw = (w << 10) / span;
        sh = (h << 10) / span;
        M_0040b0e0(buf[0], 1, x, y, 0, 1);
        rows = (len - y) / 32 + 1;
        M_0040b0e0(buf[1], 1, cx, len, rows, 1);
        step = delta * 4;
        saved = buf[2 + delta];
        M_0040b0e0(saved, 1, w, h, 0, 1);
        M_0040b0e0(buf[3 + delta], 1, w, h, rows, 1);
        if (delta > 0) {
            table = &DAT_004e246c[tick];
            for (j = 0; j < delta; j++, table++) {
                t = *table;
                v = t - 0x146;
                M_0040b0e0(buf[2 + j], 1, v * sx / 1024 + 0x140, t, 2, 1);
                M_0040b0e0(buf[4 + delta + j], 1, sw * v / 1024, sh * v / 1024, 2, 1);
            }
        }
        for (i = 0; i < delta + 2; i++) {
            for (j = 0; j < 2; j++) {
                v = buf[i + j * delta];
                M_0040aff0(5, v);
                if (i == delta + 1)
                    m_87cbec->m_10 = 0;
                else
                    m_87cbec->m_10 = buf[i + j * delta + 1] - v;
            }
        }
        for (i = 0; i < 25; i++) {
            if (m_87cbd4->m_2c[i] > 0) {
                (&m_87cbc8->m_2c)[i] = m_87cbd4->m_2c[i];
                m_87cbc8->m_18 = 0;
            }
        }
        m_87cbc8->m_64 = buf[0];
        m_87cbc8->m_6c = saved;
    }
    M_0040ba00(m_87cbc8->m_80);
    M_0040ba00(m_87cbc8->m_84);
    M_0040ba00(m_87cbc8->m_78);
    M_0040ba00(m_87cbc8->m_7c);
    M_0040ba00(m_87cbc8->m_60);
    M_0040ba00(m_87cbc8->m_68);
    M_0040ba00(m_87cbc8->m_70);
    m_87cbc8->m_10 = 1;
    for (i = 0; i < 0x40; i++) {
        if (i < count)
            (&m_87cbc8->m_0)[0x24 + i] = buf[i];
        else
            (&m_87cbc8->m_0)[0x24 + i] = 0;
    }
    m_a7978++;
    if (m_a7978 >= 3)
        m_a7978 = 0;
    M_00408650(m_a7978 + 0xb);
    return 0;
}

// OPEN: 0x4189d0 (key-outline loader, 1353 B): 49.9% similar, same size. Typed from DAT_00fb1d70 StageTable (168-byte StageRec), key tables
// (config/game_prelude_W.h). Original: i in edi, i<<4 in ebp, length reloaded into ebx per iteration, pos and slot cursor in stack slots,
// limit in the dead path parameter slot (that part already matches), and ecx holds the constant 1 across the final 18-way switch. Register
// allocation differs from the first instruction.

void FUN_004189d0(char *path)
{
    FILE *file;
    int length;
    unsigned int size;
    unsigned int pos;
    unsigned int limit;
    int i;
    int j;
    int part;
    int optional;
    StageRec *rec;
    int *slot;

    file = fopen(path, DAT_004eba40);
    if (file == 0)
        return;
    length = fread(DAT_00fb9e48, 1, 0x64000, file);
    fclose(file);
    size = 0x64000;
    FUN_00411910();
    limit = (unsigned int)&DAT_00f4c324;
    if (FUN_00411e80(0, DAT_00fb9e48, length, DAT_00f4dd70, &size, DAT_004ebe88) == 0)
        goto done;
    DAT_00a4b168.m_200[0] = FUN_004129a0(DAT_00f4dd70, (unsigned int)DAT_00a4c324, &limit);
    pos = limit;
    limit = (unsigned int)&DAT_00f4c324;
    part = 0;
    for (i = 0; i < 18; i++) {
        size = 0x64000;
        if (FUN_00411e80(0, DAT_00fb9e48, length, DAT_00f4dd70, &size, DAT_004ebd68[i].main) == 0)
            goto done;
        DAT_00a4c294[i] = FUN_004129a0(DAT_00f4dd70, pos, &limit);
        pos = limit;
        limit = (unsigned int)&DAT_00f4c324;
        size = 0x64000;
        if (FUN_00411e80(0, DAT_00fb9e48, length, DAT_00f4dd70, &size, DAT_004ebd68[i].shadow) == 0)
            goto done;
        DAT_00a4c2dc[i] = FUN_004129a0(DAT_00f4dd70, pos, &limit);
        pos = limit;
        limit = (unsigned int)&DAT_00f4c324;
        size = 0x64000;
        if (FUN_00411e80(0, DAT_00fb9e48, length, DAT_00f4dd70, &size, DAT_004ebd68[i].overlay) == 0)
            goto done;
        DAT_00f4dce0[i] = FUN_004129a0(DAT_00f4dd70, pos, &limit);
        pos = limit;
        limit = (unsigned int)&DAT_00f4c324;
        optional = DAT_004ebd68[i].optional;
        rec = &DAT_00fb1d70.rec[i];
        rec->m_0 = 1;
        rec->first = (ImgRec *)DAT_00a4b168.m_200[0];
        rec->fourth = (ImgRec *)DAT_00a4c294[i];
        rec->fifth = (ImgRec *)DAT_00a4c2dc[i];
        rec->third = (ImgRec *)DAT_00f4dce0[i];
        rec->optional = 0;
        DAT_00f4dd28[i] = 0;
        if (optional >= 0) {
            size = 0x64000;
            if (FUN_00411e80(0, DAT_00fb9e48, length, DAT_00f4dd70, &size, optional) == 0)
                goto done;
            rec->optional = (ImgRec *)FUN_004129a0(DAT_00f4dd70, pos, &limit);
            pos = limit;
            DAT_00f4dd28[i] = (int)rec->optional;
            limit = (unsigned int)&DAT_00f4c324;
            rec->m_0 = 2;
        }
        rec->layered = DAT_004ec118[i];
        slot = &DAT_00f4dc08[part];
        for (j = 0; j < DAT_004ebe90[i].count; j++) {
            size = 0x64000;
            if (FUN_00411e80(0, DAT_00fb9e48, length, DAT_00f4dd70, &size, DAT_004ebe90[i].id[j]) == 0)
                goto done;
            *slot = FUN_004129a0(DAT_00f4dd70, pos, &limit);
            pos = limit;
            limit = (unsigned int)&DAT_00f4c324;
            rec->extra[j] = (ImgRec *)*slot;
            rec->extra[j + 8] = (ImgRec *)DAT_004ec310[part].a;
            rec->extra[j + 16] = (ImgRec *)DAT_004ec310[part].b;
            rec->extra[j + 24] = (ImgRec *)DAT_004ec310[part].c;
            slot++;
            part++;
        }
        rec->extraCount = DAT_004ebe90[i].count;
        switch (i) {
        case 0:
            rec->m_1c[0] = (int)&DAT_004ec160[0];
            rec->m_1c[1] = 1;
            break;
        case 1:
            rec->m_1c[0] = (int)&DAT_004ec160[4];
            rec->m_1c[1] = 1;
            break;
        case 2:
            rec->m_1c[0] = (int)&DAT_004ec160[8];
            rec->m_1c[1] = 1;
            break;
        case 3:
            rec->m_1c[0] = (int)&DAT_004ec160[12];
            rec->m_1c[1] = 6;
            break;
        case 4:
            rec->m_1c[0] = (int)&DAT_004ec160[36];
            rec->m_1c[1] = 2;
            break;
        case 5:
            rec->m_1c[0] = (int)&DAT_004ec160[44];
            rec->m_1c[1] = 1;
            break;
        case 6:
            rec->m_1c[0] = (int)&DAT_004ec160[48];
            rec->m_1c[1] = 1;
            break;
        case 7:
            rec->m_1c[0] = (int)&DAT_004ec160[52];
            rec->m_1c[1] = 1;
            break;
        case 8:
            rec->m_1c[0] = (int)&DAT_004ec160[56];
            rec->m_1c[1] = 4;
            break;
        case 9:
            rec->m_1c[0] = (int)&DAT_004ec160[72];
            rec->m_1c[1] = 1;
            break;
        case 10:
            rec->m_1c[0] = (int)&DAT_004ec160[76];
            rec->m_1c[1] = 1;
            break;
        case 11:
            rec->m_1c[0] = (int)&DAT_004ec160[80];
            rec->m_1c[1] = 2;
            break;
        case 12:
            rec->m_1c[0] = (int)&DAT_004ec160[88];
            rec->m_1c[1] = 1;
            break;
        case 13:
            rec->m_1c[0] = (int)&DAT_004ec160[92];
            rec->m_1c[1] = 5;
            break;
        case 14:
            rec->m_1c[0] = (int)&DAT_004ec160[112];
            rec->m_1c[1] = 1;
            break;
        case 15:
            rec->m_1c[0] = (int)&DAT_004ec160[116];
            rec->m_1c[1] = 1;
            break;
        case 16:
            rec->m_1c[0] = (int)&DAT_004ec160[120];
            rec->m_1c[1] = 1;
            break;
        case 17:
            rec->m_1c[0] = (int)&DAT_004ec160[124];
            rec->m_1c[1] = 5;
            break;
        }
    }
done:
    FUN_00411920();
}
