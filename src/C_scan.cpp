#include <string.h>
#include "game.h"

void Game::M_004104e0(int id, int mode)
{
    Rec_87cbd4 *rec;

    M_0040af40(3, id);
    rec = m_87cbd4;
    if (rec->m_0 != 0) {
        switch (mode) {
        case 0:
            rec->m_4 = rec->m_8;
            break;
        case 1:
            rec->m_8 = rec->m_4;
            m_87cbd4->m_4 = 0xffff;
            return;
        case 2:
            if (id != m_87cbc4->m_7d5228) {
                rec->m_8 = rec->m_4;
                m_87cbd4->m_4 = 0xffff;
                if (DAT_004bde00[m_87cbd4->m_c * 0x16] != 0) {
                    m_87cbd4->m_4 = -8;
                    return;
                }
            }
            break;
        }
    }
}

void Game::M_00410580(int mode, int all)
{
    int limit = M_0040bf80(0x26, 0);
    int top = limit + 0x1f;
    int i;
    int v;
    int inRange;

    for (i = m_87cbc4->m_7d5214; i < 0x4000; i++) {
        M_0040af40(3, i);
        if (m_87cbd4->m_0 != 0) {
            v = M_0040b410(i);
            if (limit > v || v >= top)
                inRange = 0;
            else
                inRange = 1;
            if (all != 0 || !inRange)
                M_004104e0(i, mode);
        }
    }
}

void Game::M_00410280(int ch, int hit)
{
    int code;
    int slot;
    int idx;
    int *count;

    if (ch >= 'a' && ch <= 'z') {
        code = ch - 0x57;
    } else if (ch >= 'A' && ch <= 'Z') {
        code = ch - 0x37;
    } else if (ch >= '0' && ch <= '9') {
        code = ch - '0';
    } else if (ch == '-') {
        code = 0x27;
    } else {
        return;
    }
    slot = hit != 0 ? 1 : 2;
    M_00410260();
    DAT_0099d6bc->m_00[10 + slot] = DAT_0099d6bc->m_00[10 + slot] + 1;
    idx = slot + code * 3;
    count = &DAT_0099d6bc->m_00[0x8e + idx];
    *count = *count + 1;
    if (M_004100c0() != 0 && slot == 1)
        M_0040fd10();
}

void Game::M_0040fa00()
{
    int i = 0;

    do {
        M_00401440(i);
        if (m_8c8->m_0 != 0) {
            M_0040cf50(m_8c8->m_8, 1);
            M_00401440(i);
            if (m_8c8->m_0 != 0) {
                if (--m_3b8 < 0)
                    m_3b8 = 0;
                M_00401630(i);
            }
        }
        i++;
    } while (i < 0x20);
    M_00401480(-1);
}

void Game::M_0040fc50()
{
    int *cur = m_3d8;
    int x;
    int y;

    if (*DAT_0043065c[*cur] == '*')
        m_a7964 = 1;
    if (cur[1] == cur[0]) {
        y = 0x103;
        x = 0x186;
    } else {
        y = m_87cbc4->m_7d522c;
        x = m_87cbc4->m_7d5230;
    }
    M_0040b940(m_87cbc4->m_7d5228);
    M_0040af40(3, DAT_004f021c);
    M_0040aff0(5, m_87cbd4->m_2c[14]);
    m_87cbec->m_4 = y;
    m_87cbec->m_8 = x;
    m_87cbc4->m_7d5228 = M_00409330(2, 0x191);
}

void Game::M_00410b00()
{
    if (DAT_008f544c->m_08 != 0 && DAT_008f544c->m_04 == 0) {
        DAT_008f544c->m_04 = DAT_00430658;
        if (M_00410ba0() != 0) {
            DAT_008f544c->m_04 = 1;
            M_004066d0(DAT_008f544c->m_0c * 100000 / 10);
            M_00407310(1);
            DAT_008f544c->m_9c = 0x14;
            DAT_008f544c->m_98 = 0;
        }
    }
}
