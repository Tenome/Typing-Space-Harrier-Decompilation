#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "game.h"

// OPEN: non-switch path reloads c from the stack instead of keeping edi (register/live-range)
void Game::M_00406ef0(int a, int b, int c)
{
    int state;
    int frame;

    M_00406c90(c);
    state = DAT_00a232a0->m_0;
    if (state != 0) {
        if (state <= 0 || state > 2) {
            frame = c;
        } else {
            switch (DAT_00a232a0->m_8[DAT_00a232a0->m_4c]) {
            case 1:  frame = M_0040bf80(0x7c, 0); break;
            case 2:  frame = M_0040bf80(0x7c, 1); break;
            case 3:  frame = M_0040bf80(0x7c, 2); break;
            case 4:  frame = M_0040bf80(0x7c, 3); break;
            case 5:  frame = M_0040bf80(0x7c, 4); break;
            case 6:  frame = M_0040bf80(0x7c, 5); break;
            case 7:  frame = M_0040bf80(0x7c, 6); break;
            case 8: case 9: case 10: case 11: case 12:
                frame = M_0040bf80(0x7c, 7);
                break;
            case 13: case 14: case 15: case 16: case 17:
                frame = M_0040bf80(0x7c, 8);
                break;
            case 18: frame = M_0040bf80(0x7c, 9); break;
            case 19: frame = M_0040bf80(0x7c, 10); break;
            default:
                return;
            }
            DAT_00a232a0->m_58++;
            DAT_00a232a0->m_54++;
            if (DAT_00a232a0->m_58 >= DAT_00a232a0->m_5c) {
                M_00406cb0(c);
                return;
            }
            if (DAT_00a232a0->m_54 >= DAT_00a232a0->m_50) {
                DAT_00a232a0->m_4c++;
                if (DAT_00a232a0->m_4c >= DAT_00a232a0->m_48)
                    DAT_00a232a0->m_4c = 0;
                DAT_00a232a0->m_54 = 0;
            }
        }
        M_0040bea0(a, b, frame);
    }
}

// OPEN: zero constant hoisted into esi, original has test ebx,ebx (register/schedule)
void Game::M_004071c0()
{
    int active[4];
    int count;
    int i;
    int pick;

    count = 0;
    for (i = 0; i < 4; i++) {
        M_00406c90(i);
        if (DAT_00a232a0->m_0 == 1)
            active[count++] = i;
    }
    if (count == 0)
        return;
    pick = rand() % count;
    DAT_00a46264[0] = 0;
    DAT_00a46264[1] = 0;
    DAT_00a46264[2] = 0;
    DAT_00a46264[3] = 0;
    DAT_00a46264[active[pick]] = 1;
}

// OPEN: loop pointer/counter register roles and grade stack slot (register/schedule)
void Game::M_004076a0(int flag)
{
    int grade;
    Slot *slot;
    int len;
    int *row;
    int i;

    grade = 5;
    m_8c4->m_7c = grade;
    if (flag == 0) {
        slot = m_8c4;
        if (slot->m_84 != 0) {
            len = strlen(slot->m_1494);
            if (len >= 0x14)
                len = 0x14;
            row = &DAT_004e1fdc[len * 4];
            for (i = 0; i < 4; i++, row++) {
                if (slot->m_80 * 100 / slot->m_84 <= *row) {
                    grade = DAT_004e2314[i];
                    break;
                }
            }
            slot->m_7c = grade;
        }
    }
}

// OPEN: SIB operand order [eax+ecx-4] vs [ecx+eax-4] in byte-offset clear loop (register)
void Game::M_00407cd0(int a, int b, int c, int d, int e, int f, int g, int h)
{
    int off;

    if (DAT_0099cfd0 < 0x4000) {
        M_0040af40(0, DAT_0099cfd0);
        for (off = 0x2c; off < 0x90; off += 4)
            *(int *)((char *)m_87cbc8 + off) = 0;
        if (a < 0)
            a = 0;
        if (b < 0)
            b = 0;
        if (c < 0)
            c = 0;
        if (d < 0)
            d = 0;
        if (e < 0)
            e = 0;
        if (f < 0)
            f = 0;
        if (g < 0)
            g = 0;
        if (h < 0)
            h = 0;
        m_87cbc8->m_0 = 0;
        m_87cbc8->m_c = 0;
        m_87cbc8->m_28 = h;
        m_87cbc8->m_24 = 0;
        m_87cbc8->m_20 = DAT_008f0ff4;
        m_87cbc8->m_2c = a;
        m_87cbc8->m_64 = b;
        m_87cbc8->m_6c = c;
        m_87cbc8->m_74 = d;
        m_87cbc8->m_7c = e;
        m_87cbc8->m_84 = f;
        m_87cbc8->m_8c = g;
        m_87cbc4->m_7d5224 = DAT_0099cfd0;
        m_87cbc4->m_7d5214 = DAT_0099cfd0;
        if (DAT_004efa14 > 0) {
            M_0040af40(0, DAT_004efa14);
            m_87cbc8->m_24 = DAT_0099cfd0;
        }
        DAT_004efa14 = DAT_0099cfd0;
        DAT_0099cfd0++;
    }
}

// OPEN: lea edi for rep stosd scheduled after neighbouring store (schedule)
void Game::M_00407ed0()
{
    int i;

    for (i = 0; i < 0x800; i++) {
        m_8ebcc8[i] = 0;
        m_8eacc8[i] = 0;
        m_8e9cc8[i] = 0;
        m_8e8cc8[i] = 0;
        m_8e7cc8[i] = 0;
        m_8e6cc8[i] = 0;
        m_8e5cc8[i] = 0;
        m_8e4cc8[i] = 0;
        m_881cc8[i] = 0;
        m_880cc8[i] = 0;
        m_87fcc8[i] = 0;
        M_00408cb0(i);
    }
    memset(m_8eccc8, 0, sizeof(m_8eccc8));
    m_8ecec8 = 0;
    m_87fcac = 0;
    m_87fcb0 = -1;
}

// OPEN: whole function incl. old 4089c8; original has extra interleaved arg pushes and add esp,0x24 (inline/call set)
void Game::M_004089a0(int n)
{
    char number[0x20];
    char path[0x108];

    if (n < 0) {
        FUN_004188f0(0);
        FUN_004188f0(1);
        return;
    }
    wsprintfA(number, "%02d", n);
    strcpy(path, DAT_011215bc);
    strcat(path, "BGM\\");
    strcat(path, number);
    strcat(path, ".WAV");
    FUN_004185a0(path, DAT_00a4604c);
    DAT_00a4604c = DAT_00a4604c == 0;
    FUN_004188f0(DAT_00a4604c);
}

// OPEN: loop entry: tries=0 stack store and limit spill (register/schedule)
int Game::M_00409270(int n)
{
    int found;
    int cur;
    int tries;
    int limit;
    int *out;

    found = 0;
    cur = m_880cc8[n];
    tries = 0;
    limit = DAT_008f069c[m_881cc8[n]];
    out = m_8eccc8;
    for (; tries < limit; tries++) {
        if (cur <= 0)
            break;
        M_0040af40(0, cur);
        if (m_87cbc8->m_0 != 0 && m_87cbc8->m_c == n) {
            *out = cur;
            found++;
            out++;
            cur = m_87cbc8->m_24;
        }
    }
    m_8ecec8 = found;
    return found;
}

// OPEN: record pointer in edx vs eax (register)
void Game::M_00409ce0(int n)
{
    Timer *timer;

    M_00409ca0(n);
    timer = DAT_004efa18;
    timer->m_0 = 0;
    timer->m_4 = 0;
    timer->m_8 = 0;
    timer->m_c = 0;
    timer->m_10 = 0;
}

// OPEN: esi/edi init order (register)
int Game::M_00409d10()
{
    int none;
    int i;

    none = DAT_00430658;
    i = 0;
    do {
        M_00409ca0(i);
        if (DAT_004efa18->m_0 == 0)
            return i;
        i++;
    } while (i < 0x40);
    return none;
}

// OPEN: register/schedule
void Game::M_00409db0()
{
    int i;

    i = 0;
    do {
        M_00409ca0(i);
        if (DAT_004efa18->m_0 != 0) {
            DAT_004efa18->m_10--;
            if (DAT_004efa18->m_10 < 1) {
                M_00409700(DAT_004efa18->m_4, DAT_004efa18->m_8, DAT_004efa18->m_c);
                M_00409ce0(i);
            }
        }
        i++;
    } while (i < 0x40);
}

// OPEN: zero constant hoisted into edi, original test eax,eax + xor edx (register/schedule)
void Game::M_0040ba00(int n)
{
    Rec_87cbd4 *rec;

    if (-1 < n && n < 0x10000 && n >= m_87cbc4->m_7d5210) {
        M_0040aff0(5, n);
        rec = m_87cbec;
        rec->m_0 = 0;
        rec->m_4 = 0;
        rec->m_8 = 0;
        rec->m_c = 0;
        rec->m_10 = 0;
    }
}

// OPEN: loop end compare signed (int)p < const; pointer loop form (CFG/compare)
void Game::M_0040c650()
{
    int i;

    i = 0;
    do {
        DAT_00a45e34 = &DAT_00901708[i];
        memset(&DAT_00901708[i], 0, sizeof(BankRec));
        i++;
    } while (i < 0x50);
    M_0040c760();
    M_0040c6a0();
}

// OPEN: loop end compare signed; pointer loop form (CFG/compare)
void Game::M_0040c6a0()
{
    int i;

    i = 0;
    do {
        DAT_004f052c = &DAT_008f5450[i];
        memset(&DAT_008f5450[i], 0, sizeof(GroupRec));
        i++;
    } while (i < 71);
    m_87fcb8 = 0;
    m_87fcc0 = 0;
    m_87fcc4 = 0;
}


// OPEN: this in esi/edi and x load order (register/schedule)
void Game::M_0040a6d0()
{
    int x;
    int y;
    int shift;
    int *stage;
    int next;

    if (m_39c <= 0) {
        y = (m_87cbc4->m_7d5230 - 0x1e0) * 0xcc / 0x141 + 0x1ac;
        x = m_87cbc4->m_7d522c;
        m_87cbc4->m_7d523c = y;
        shift = (x - 0x140) / 16;
        stage = m_3d8;
        if (stage[4] != stage[0] && stage[0xc] >= 0) {
            stage[4] = stage[0];
            next = m_3d8[4] + 1;
            if (next > 0x12)
                next = 0x12;
            FUN_00418f70(next);
            m_3d8[8] = 0;
            m_3d8[7] = 0;
            m_3d8[6] = 0;
            m_3d8[5] = 0;
        }
        M_0040f3a0(-shift, y);
    }
}

// OPEN: orig uses 4 callee-saved regs (first kept live, tried in ecx); loop shape differs (register/CFG)
int Game::M_0040bbe0()
{
    int tried;
    int limit;
    int cur;

    tried = 0;
    limit = 0x10000 - m_87cbc4->m_7d5210;
    cur = m_87cbc4->m_7d5220;
    if (limit <= 0)
        return 0;
    do {
        cur++;
        if (cur >= 0x10000)
            cur = m_87cbc4->m_7d5210;
        M_0040aff0(5, cur);
        if (m_87cbec->m_0 == 0) {
            if (cur != 0)
                m_87cbc4->m_7d5220 = cur;
            return cur;
        }
        tried++;
    } while (tried < limit);
    return 0;
}

// OPEN: same as 40bbe0 (register/CFG)
int Game::M_0040bc60()
{
    int tried;
    int limit;
    int cur;

    tried = 0;
    limit = 0x3fff - m_87cbc4->m_7d5214;
    cur = m_87cbc4->m_7d5224;
    if (limit <= 0)
        return 0;
    do {
        cur++;
        if (cur >= 0x4000)
            cur = m_87cbc4->m_7d5214;
        M_0040af40(3, cur);
        if (m_87cbd4->m_0 == 0) {
            if (cur != 0)
                m_87cbc4->m_7d5224 = cur;
            return cur;
        }
        tried++;
    } while (tried < limit);
    return 0;
}

// OPEN: two bool locals live in regs/stack differently; loop shape (register)
int Game::M_0040c9f0(BankRec *bank, int store)
{
    unsigned char *p;
    int count;
    int i;
    bool haveReading;
    bool atBreak;

    p = (unsigned char *)bank->m_00[1];
    count = 0;
    haveReading = false;
    i = 0;
    atBreak = true;
    if (bank->m_00[2] > 0) {
        while (*p != 0x1a) {
            if (*p < 0x20) {
                atBreak = true;
            } else if (atBreak) {
                atBreak = false;
                if (haveReading) {
                    if (store != 0)
                        bank->m_10[count] = (char *)p;
                    count++;
                    haveReading = false;
                } else {
                    if (store != 0)
                        bank->m_14[count] = (char *)p;
                    haveReading = true;
                }
            }
            i++;
            p++;
            if (bank->m_00[2] <= i) {
                bank->m_0c = count;
                return count;
            }
        }
        *p = 0;
    }
    bank->m_0c = count;
    return count;
}
