#include <string.h>
#include <stdlib.h>
#include "game.h"

// OPEN: 004096b0 registers/loop match; original pushes esi only after the early return (74 vs 75 bytes), pointer init sits after the first test
int __stdcall FUN_004096b0(int n)
{
    int *entry;
    int value;
    int i;
    int found;

    if (n <= 0x146)
        return DAT_00430658;
    found = -1;
    i = 0;
    entry = DAT_004e246c;
    value = *entry;
    while (value >= 0) {
        if (n > value)
            found = i;
        value = *++entry;
        i++;
    }
    if (found >= 0 && ++found >= 0)
        return found;
    return 0;
}

// OPEN: 00408130 exact except the order of the two return blocks (return 1 block comes first in the original)
int Game::M_00408130()
{
    int i;
    int distance;
    int stageNo;
    int active;

    for (i = 0;; i++) {
        stageNo = DAT_004bddf8[i].m_0;
        active = DAT_004bddf8[i].m_4;
        if (DAT_004bddf8[i].m_10 >= 0 || DAT_004bddf8[i].m_1c >= 0) {
            if ((stageNo <= 0 || stageNo == m_3d8[0] + 1) && active == 1 && m_87fcc8[i] == 1) {
                M_0040af40(0, m_880cc8[i]);
                distance = m_87cbc8->m_0;
                if (distance < 0)
                    distance = -distance;
                if (distance == m_881cc8[i])
                    return 1;
            }
        } else {
            return 0;
        }
    }
}

// OPEN: 004081b0 exact except the strength-reduced induction base (original &row->m_10, mine row start)
int Game::M_004081b0(int scanOnly)
{
    int i;
    int distance;
    int highest;
    int firstStage;
    int lastStage;

    highest = m_3d8[0xc];
    for (i = 0;; i++) {
        if (DAT_004bddf8[i].m_0 > 0 && DAT_004bddf8[i].m_0 != m_3d8[0] + 1)
            continue;
        firstStage = DAT_004bddf8[i].m_10;
        lastStage = DAT_004bddf8[i].m_1c;
        if (firstStage < 0 && lastStage < 0) {
            if (scanOnly)
                m_3d8[0xc] = highest;
            return highest;
        }
        if (m_87fcc8[i] == 1) {
            M_0040af40(0, m_880cc8[i]);
            distance = m_87cbc8->m_0;
            if (distance < 0)
                distance = -distance;
            if (distance != m_881cc8[i])
                m_87fcc8[i] = 2;
        }
        if (firstStage <= m_3d8[0xc] && m_87fcc8[i] == 0) {
            if (DAT_004bddf8[i].m_34 > 0)
                M_00408660(DAT_004bddf8[i].m_34, DAT_004bddf8[i].m_38);
            if (firstStage < 0) {
                m_87fcc8[i] = 2;
            } else {
                if (firstStage > highest)
                    highest = m_3d8[0xc];
                if (scanOnly == 0) {
                    M_00408ad0(i, lastStage);
                    if (lastStage >= 0x114 && lastStage <= 0x122)
                        M_00410af0();
                }
            }
        }
    }
}

// OPEN: 0040ba50 (396 B) CFG class: the original shares one 'm_c = 0; return 0' block between the b==0 and c==0 paths and keeps a zero register in edx; draft is 393 B
int Game::M_0040ba50(int mode, int a, int b, int c)
{
    int duration;
    int step;
    int span;
    int k;
    int value;

    M_0040aff0(1, a);
    M_0040aff0(2, b);
    M_0040aff0(3, c);
    m_87cbe0->m_c++;
    if (b == 0) {
        m_87cbe0->m_8 = 0;
        m_87cbe0->m_4 = 0;
    }
    if (c == 0) {
        if (b != 0) {
            m_87cbe0->m_4 = m_87cbe4->m_4;
            m_87cbe0->m_8 = m_87cbe4->m_8;
        }
        m_87cbe0->m_c = 0;
        return 0;
    }
    if (b == 0) {
        span = m_87cbe8->m_c;
    } else {
        duration = m_87cbe8->m_c;
        step = m_87cbe0->m_c;
        span = duration;
        if (span < 0)
            span = -span;
        if (step > span)
            step = span;
        if (duration <= 0) {
            if (duration == 0) {
                m_87cbe0->m_4 = m_87cbe8->m_4;
                m_87cbe0->m_8 = m_87cbe8->m_8;
            }
        } else if (mode == 1) {
            for (k = 4; k < 0xc; k += 4)
                *(int *)((char *)m_87cbe0 + k) = (*(int *)((char *)m_87cbe8 + k) - *(int *)((char *)m_87cbe4 + k)) * step / span + *(int *)((char *)m_87cbe4 + k);
        }
    }
    if (m_87cbe0->m_c < span)
        return c;
    m_87cbe0->m_c = 0;
    m_87cbe0->m_4 = m_87cbe8->m_4;
    m_87cbe0->m_8 = m_87cbe8->m_8;
    value = m_87cbe8->m_10;
    if (value == 0)
        return 0;
    if (value == 10000 || (value += c) < 0 || value >= 0x10000)
        return 10000;
    return value;
}

// OPEN: 004090a0 exact size (234); only the register assignment differs (original keeps this in edi and total in ebx, mine ebx/edi): register/schedule class
void Game::M_004090a0(int stage)
{
    int mode;
    int previous;
    int total;
    short current;
    int percent;
    int shown;
    int level;

    mode = m_8ebcc8[stage];
    if (mode == 1 || mode == 2) {
        previous = m_8e9cc8[stage];
        total = m_8eacc8[stage];
        if (DAT_004bddf8[stage].m_28 < 1)
            percent = 100;
        else
            percent = m_8e3cc8[stage] / DAT_004bddf8[stage].m_28;
        if (mode == 1)
            current = total - ((total - 1) * percent) / 100 - 1;
        else
            current = total - (percent * total) / 100;
        m_8e9cc8[stage] = current;
        level = current;
        if (level != previous) {
            if (mode == 1) {
                shown = total - level - 1;
                if (level == 0)
                    shown = total - 1;
                M_00409210(stage, shown);
                if (total - 1 <= shown) {
                    M_00409190(stage, 0);
                    return;
                }
            } else {
                M_00409190(stage, level);
            }
        }
    }
}

// OPEN: 00408660 (sound id remap switch, 788 B with tables): switch/jump-table shape reproduced (byte index + dword table) but the original keeps this in esi and the param in eax until the switch writes esi, and every case jumps to one shared tail; mine inlines the tail after case 1 and keeps the param in esi (register/CFG class)
void Game::M_00408660(int sound, int fade)
{
    int id;

    if (sound > 0 && (DAT_00a45e28 == 0 || sound == 0x1a || sound == 0x4f)) {
        switch (sound) {
        case 0x1:
            id = 0xa;
            break;
        case 0x2:
            id = 0xb;
            break;
        case 0x3:
            id = 0xc;
            break;
        case 0x4:
            id = 0xd;
            break;
        case 0x5:
            id = 0xe;
            break;
        case 0x6:
            id = 0xf;
            break;
        case 0x7:
            id = 0x3;
            break;
        case 0x8:
            id = 0x4;
            break;
        case 0x9:
            id = 0x13;
            break;
        case 0xa:
            id = 0x14;
            break;
        case 0xb:
            id = 0x10;
            break;
        case 0xc:
            id = 0x11;
            break;
        case 0xd:
            id = 0x12;
            break;
        case 0xe:
            id = 0x5;
            break;
        case 0xf:
            id = 0x6;
            break;
        case 0x10:
            id = 0x7;
            break;
        case 0x11:
            id = 0x8;
            break;
        case 0x12:
            id = 0x15;
            break;
        case 0x13:
            id = 0x16;
            break;
        case 0x14:
            id = 0x17;
            break;
        case 0x15:
            id = 0x18;
            break;
        case 0x16:
            id = 0x19;
            break;
        case 0x17:
            id = 0x1a;
            break;
        case 0x18:
            id = 0x1b;
            break;
        case 0x19:
            id = 0x1c;
            break;
        case 0x1a:
            id = 0x1d;
            break;
        case 0x1b:
            id = 0x1e;
            break;
        case 0x1c:
            id = 0x20;
            break;
        case 0x1d:
            id = 0x21;
            break;
        case 0x1e:
            id = 0x22;
            break;
        case 0x1f:
            id = 0x23;
            break;
        case 0x20:
            id = 0x24;
            break;
        case 0x21:
            id = 0x25;
            break;
        case 0x22:
            id = 0x26;
            break;
        case 0x23:
            id = 0x27;
            break;
        case 0x24:
            id = 0x28;
            break;
        case 0x25:
            id = 0x29;
            break;
        case 0x26:
            id = 0x2a;
            break;
        case 0x27:
            id = 0x2b;
            break;
        case 0x28:
            id = 0x2c;
            break;
        case 0x29:
            id = 0x2d;
            break;
        case 0x2a:
            id = 0x2e;
            break;
        case 0x2b:
        case 0x2c:
        case 0x2d:
        case 0x2e:
        case 0x2f:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x3a:
        case 0x3b:
        case 0x3c:
        case 0x3d:
        case 0x3e:
        case 0x3f:
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x4b:
        case 0x4c:
        case 0x4d:
        case 0x4e:
        case 0x4f:
        case 0x50:
            M_00408980(sound - 0x2b);
            return;
        default:
            id = sound;
            break;
        }
        if (fade != 0) {
            FUN_0041e2a0(id, FUN_0041e2c0() / 100);
            FUN_0041e240(id);
            return;
        }
        FUN_0041e2c0();
        FUN_0041e290(id);
        FUN_0041e220(id);
    }
}

// OPEN: 00407310 (611 B): wave spawner. Shuffle loop and switch match up to the roll; remaining: row*3 is precomputed before rand() in the original and the mode expression compiles to setge/dec/and 2 (mine setl/add), table accessed as [edi*4+base] (register/schedule class)
void Game::M_00407310(int group)
{
    int order[4];
    int i;
    int a;
    int b;
    int swap;
    int row;
    int mode;
    int kind;
    int k;
    int count;
    int value;
    int roll;

    for (i = 0; i < 4; i++)
        order[i] = i;
    for (i = 0x28; i != 0; i--) {
        a = rand() % 4;
        b = rand() % 4;
        swap = order[a];
        order[a] = order[b];
        order[b] = swap;
    }
    switch (group) {
    case 1:
        row = 0;
        break;
    case 2:
        row = 1;
        break;
    case 3:
        row = 2;
        break;
    case 4:
        row = 3;
        break;
    default:
        return;
    }
    roll = rand() % 100;
    value = DAT_004e22e0[row].value;
    count = DAT_004e22e0[row].count;
    mode = (roll >= DAT_004e22e0[row].chance) ? 0 : 2;
    for (i = 0; i < count; i++) {
        kind = DAT_004e21a0[row * 16 + rand() % 16];
        M_00406c90(order[i]);
        DAT_00a232a0->m_0 = 1;
        DAT_00a232a0->m_4 = kind;
        DAT_00a232a0->m_58 = 0;
        DAT_00a232a0->m_4c = 0;
        DAT_00a232a0->m_54 = 0;
        DAT_00a232a0->m_5c = value;
        switch (mode) {
        case 2:
            DAT_00a232a0->m_50 = 1;
            for (k = 0; DAT_004e22d8[k] >= 0; k++) {
                DAT_00a232a0->m_8[k] = DAT_004e22d8[k];
                DAT_00a232a0->m_48 = k + 1;
            }
            break;
        case 1:
            DAT_00a232a0->m_50 = 2;
            for (k = 0; DAT_004e22a4[k] >= 0; k++) {
                DAT_00a232a0->m_8[k] = DAT_004e22a4[k];
                DAT_00a232a0->m_48 = k + 1;
            }
            break;
        case 0:
            DAT_00a232a0->m_50 = 4;
            if (kind == 1) {
                DAT_00a232a0->m_8[0] = 1;
                DAT_00a232a0->m_8[1] = 2;
                DAT_00a232a0->m_48 = 2;
            } else {
                DAT_00a232a0->m_8[0] = kind;
                DAT_00a232a0->m_8[1] = kind;
                DAT_00a232a0->m_48 = 2;
            }
            break;
        }
    }
}

// OPEN: 0040a270 depth bucket sort: 539 vs 538 B; differences: stack slot assignment (orig 0x10 frame, keeps outer index and bucket pointer on stack) and interleaving of placed++ with the bucket count increment (register/schedule class). Needs Field members (see report requests)
void Game::M_0040a270()
{
    int placed;
    int obj;
    int k;
    int depth;
    int bucket;
    int b;
    int n;
    int j;
    int a;
    int c;
    int swap;

    placed = 0;
    obj = m_87cbc4->m_7d5214;
    for (k = 0; k < 0x81; k++)
        m_87cbc4->m_7b5004[k] = 0;
    m_87cbc4->m_7c520c = 0;
    for (; obj < 0x4000; obj++) {
        M_0040af40(0, obj);
        if (m_87cbc8->m_0 > 0) {
            depth = M_0040b350(obj);
            if (depth == 0)
                depth = M_0040b350(obj);
            if (depth != 0x8002) {
                bucket = depth >> 8;
                if (bucket >= 0x81)
                    bucket = 0x80;
                else if (bucket < 0)
                    bucket = 0;
                m_87cbc4->m_785004[placed] = depth;
                m_87cbc4->m_7a5004[placed] = obj;
                m_87cbc4->m_795004[placed] = bucket;
                placed++;
                m_87cbc4->m_7b5004[bucket] = m_87cbc4->m_7b5004[bucket] + 1;
            }
        }
    }
    m_87cbc4->m_785000 = placed;
    for (b = 0; b < 0x81; b++) {
        if (m_87cbc4->m_7b5004[b] != 0) {
            n = 0;
            for (j = 0; j < m_87cbc4->m_785000; j++) {
                if (b == m_87cbc4->m_795004[j]) {
                    m_87cbc4->m_7b520c[n] = j;
                    n++;
                }
            }
            if (n > 0) {
                for (a = 0; a < n; a++) {
                    for (c = a + 1; c < n; c++) {
                        if (m_87cbc4->m_785004[m_87cbc4->m_7b520c[c]] < m_87cbc4->m_785004[m_87cbc4->m_7b520c[a]]) {
                            swap = m_87cbc4->m_7b520c[a];
                            m_87cbc4->m_7b520c[a] = m_87cbc4->m_7b520c[c];
                            m_87cbc4->m_7b520c[c] = swap;
                        }
                    }
                }
                for (k = 0; k < n; k++) {
                    m_87cbc4->m_7c5210[m_87cbc4->m_7c520c] = m_87cbc4->m_7a5004[m_87cbc4->m_7b520c[k]];
                    m_87cbc4->m_7c520c++;
                }
            }
        }
    }
}

// OPEN: 0040b140 (494 B): shape matches (478 B); original keeps count in a stack slot and the link pointer/index in ebp/ebx with a hoisted zero register, mine spills the pointer (register class). M_0040b330 must return int (request)
void Game::M_0040b140(int obj)
{
    int links[3];
    int count;
    int i;
    int link;
    int k;
    int *entry;
    int whole;
    int x;
    int y;
    float scale;

    if (obj >= 0 && obj < 0x4000) {
        M_0040af40(0, obj);
        if (m_87cbc8->m_0 > 0) {
            whole = 1;
            count = 0;
            scale = 1.0f;
            i = 0;
            entry = links;
            do {
                if (i == 0)
                    link = m_87cbc8->m_28;
                else
                    link = m_87cbd0->m_18[4];
                if (link == 0)
                    break;
                M_0040af40(2, link);
                if (m_87cbd0->m_0 == 0) {
                    whole = 0;
                    break;
                }
                *entry = link;
                count++;
                entry++;
                i++;
            } while (i < 3);
            if (count != 0) {
                if (!whole) {
                    if (count < 1)
                        return;
                    entry = links;
                    do {
                        M_0040b940(*entry);
                        entry++;
                        count--;
                    } while (count != 0);
                    return;
                }
                if (count > 0) {
                    entry = links;
                    do {
                        scale = M_0040b390(obj) * scale;
                        M_0040af40(2, *entry);
                        M_0040b800(m_87cbc8->m_30, m_87cbd0->m_2c[7]);
                        entry++;
                        count--;
                    } while (count != 0);
                }
            }
            M_0040b850(m_87cbc8->m_44, m_87cbc8->m_58, scale);
            M_0040b850(m_87cbc8->m_34, m_87cbc8->m_4c, scale);
            M_0040b8b0(m_87cbc8->m_40, m_87cbc8->m_44, m_87cbc8->m_54, scale);
            M_0040b800(m_87cbc8->m_30, m_87cbc8->m_40);
            k = M_0040b330(obj);
            if (k == 0)
                k = 0x7c01 - (int)M_0040b390(obj);
            M_0040b4d0(obj, 2, &x, &y);
            if (x == 0 || y == 0)
                k = 0x8002;
            M_0040b370(obj, k);
        }
    }
}

// OPEN: 004093b0 (760 B): 761-764 B draft, flow matches; original keeps the hoisted zero in edi and different stack slots (register/schedule class).
// Disabled until Rec_87cbc8 exposes int m_2c[] and m_882cc8 is declared (see report requests).
#if 0
int Game::M_004093b0(int stage, int index, int base, int count)
{
    int failed;
    int got;
    int i;
    int j;
    int item;
    int serial;
    int randomized;
    int cap;
    int limit;
    int *slot;
    int *key;
    int serials[128];
    int unit;
    int k;
    short *out;

    failed = DAT_00430658;
    got = 0;
    randomized = 0;
    if (count == -1) {
        randomized = 1;
        count = DAT_004bddf8[stage].m_44;
        cap = DAT_004bddf8[stage].m_48;
        limit = DAT_008f069c[index];
        if (cap <= 0)
            cap = 0;
        if (cap > limit)
            cap = limit;
    }
    unit = DAT_004bddf8[stage].m_20;
    if (count != 0) {
        if (count >= 0x80)
            count = 0x80;
        if (count > 0) {
            slot = serials;
            do {
                item = M_0040bc60();
                if (item == 0)
                    return 1;
                *slot = item;
                got++;
                slot++;
            } while (got < count);
        }
        for (i = 0; i < count; i++) {
            item = serials[i];
            serial = i + base;
            if (randomized) {
                if (cap < 1) {
                    if (limit < 1)
                        serial = base;
                    else
                        serial = rand() % limit + base;
                } else {
                    serial = cap - 1 + base;
                }
                if (serial < 0)
                    serial = 0;
            }
            M_0040a1d0(item, serial);
            M_0040af40(0, item);
            k = m_87cbc8->m_28;
            if (k > 0)
                m_87cbc8->m_28 = serials[k - base];
            k = m_87cbc8->m_24;
            if (k > 0)
                m_87cbc8->m_24 = serials[k - base];
            if (DAT_004e23c4[0] >= 0) {
                key = DAT_004e23c4;
                k = DAT_004e23c4[0];
                do {
                    item = M_0040bbe0();
                    if (item == 0)
                        return failed;
                    m_87cbc8->m_2c[k] = item;
                    M_0040ba00(item);
                    key++;
                    k = *key;
                } while (k >= 0);
            }
        }
        if (count > 0) {
            slot = serials;
            j = count;
            out = m_882cc8[stage];
            do {
                failed = *slot;
                *out = failed;
                M_0040af40(0, failed);
                m_87cbc8->m_0 = index;
                m_87cbc8->m_10 = 0;
                m_87cbc8->m_c = stage;
                m_87cbc8->m_14 = unit;
                if (DAT_004e23c4[0] >= 0) {
                    key = DAT_004e23c4;
                    k = DAT_004e23c4[0];
                    do {
                        M_0040aff0(0, m_87cbc8->m_2c[k]);
                        key++;
                        m_87cbd8->m_0 = 1;
                        k = *key;
                    } while (k >= 0);
                }
                m_87cbc8->m_18 = 0;
                m_87cbc8->m_1c = 0;
                if (m_87cbc8->m_2c[0] != 0) {
                    M_0040aff0(0, m_87cbc8->m_2c[0]);
                    m_87cbc8->m_1c = m_87cbd8->m_10;
                }
                out++;
                slot++;
                j--;
            } while (j != 0);
        }
        failed = serials[0];
    }
    return failed;
}
#endif
