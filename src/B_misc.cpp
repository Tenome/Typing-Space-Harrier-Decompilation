#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"

void Game::M_0040a640()
{
    M_004022e0(0, 0, 0x280, 0x1e0, 0);
}

int Game::M_0040b330(int a)
{
    return M_0040b430(a, 0, 0);
}

int Game::M_0040b350(int a)
{
    return M_0040b430(a, 4, 0);
}

void Game::M_0040b370(int a, int b)
{
    M_0040b520(a, 4, 0, b);
}

int Game::M_0040b3f0(int a)
{
    return M_0040b430(a, 8, 0);
}

int Game::M_0040b410(int a)
{
    return M_0040b430(a, 3, 0);
}

int Game::M_0040bf80(int a, int b)
{
    return m_87cbfc[a] + b;
}

void Game::M_0040c6e0()
{
    int i;

    i = 0;
    do {
        DAT_00a45e34 = &DAT_00901708[i];
        M_0040c710(&DAT_00901708[i]);
        i++;
    } while (i < 0x50);
}

void Game::M_0040c760()
{
    int i;

    i = 0;
    do {
        M_0040c780(i);
        i++;
    } while (i < 0x50);
}

void Game::M_0040a210()
{
    int i;
    int first;

    first = m_87cbc4->m_7d5214;
    for (i = first; i < 0x4000; i++)
        M_0040b5c0(i);
    for (i = first; i < 0x4000; i++)
        M_0040b140(i);
    M_0040a270();
}

void Game::M_0040a100()
{
    int pick;

    if (M_004100c0() == 0) {
        M_0040a160(m_8c4->m_8);
        if (DAT_004f0550 > 0) {
            pick = rand();
            M_00409e00(m_87cbc4->m_7d5228, DAT_004f0554[pick % DAT_004f0550], 0);
        }
    }
}

void Game::M_0040a160(int n)
{
    int i;
    Rec_87cbd4 *rec;

    DAT_004f0550 = 0;
    for (i = m_87cbc4->m_7d5214; i < 0x4000; i++) {
        M_0040af40(2, i);
        rec = m_87cbd0;
        if (rec->m_0 > 0 && rec->m_10 == 0 && rec->m_c == n) {
            DAT_004f0554[DAT_004f0550] = i;
            DAT_004f0550++;
        }
    }
}

void Game::M_0040a1d0(int a, int b)
{
    M_0040af40(0, a);
    M_0040af40(3, b);
    memcpy(m_87cbc8, m_87cbd4, 100 * sizeof(int));
}

void Game::M_0040a660()
{
    int order[0x20];
    int i;
    int tmp;

    for (i = 0; i < 0x20; i++)
        order[i] = i;
    if (m_8b8 != -1) {
        tmp = order[0x1f];
        order[0x1f] = order[m_8b8];
        order[m_8b8] = tmp;
    }
    for (i = 0; i < 0x20; i++)
        M_00401dd0(order[i]);
}

void Game::M_0040af40(int kind, int n)
{
    if (n >= 0 && n < 0x4000 && kind >= 0 && kind < 4) {
        switch (kind) {
        case 0:
            m_87cbc8 = (Rec_87cbc8 *)(m_87cbc4->m_0 + n * 400);
            break;
        case 1:
            m_87cbcc = (Rec_87cbd4 *)(m_87cbc4->m_0 + n * 400);
            break;
        case 2:
            m_87cbd0 = (Rec_87cbd4 *)(m_87cbc4->m_0 + n * 400);
            break;
        case 3:
            m_87cbd4 = (Rec_87cbd4 *)(m_87cbc4->m_0 + n * 400);
            break;
        }
    }
}

void Game::M_0040aff0(int kind, int n)
{
    if (n >= 0 && n < 0x10000 && kind >= 0 && kind < 6) {
        switch (kind) {
        case 0:
            m_87cbd8 = (Rec_87cbd4 *)(m_87cbc4->m_640000 + n * 20);
            break;
        case 1:
            m_87cbe0 = (Rec_87cbd4 *)(m_87cbc4->m_640000 + n * 20);
            break;
        case 2:
            m_87cbe4 = (Rec_87cbd4 *)(m_87cbc4->m_640000 + n * 20);
            break;
        case 3:
            m_87cbe8 = (Rec_87cbd4 *)(m_87cbc4->m_640000 + n * 20);
            break;
        case 4:
            m_87cbdc = (Rec_87cbd4 *)(m_87cbc4->m_640000 + n * 20);
            break;
        case 5:
            m_87cbec = (Rec_87cbd4 *)(m_87cbc4->m_640000 + n * 20);
            break;
        }
    }
}

void Game::M_0040b0e0(int n, int a, int b, int c, int d, int e)
{
    M_0040aff0(5, n);
    m_87cbec->m_0 = a;
    m_87cbec->m_4 = b;
    m_87cbec->m_8 = c;
    m_87cbec->m_c = d;
    m_87cbec->m_10 = e;
}

void Game::M_0040a5c0()
{
    M_0040a640();
    M_0040a490();
    M_0040a6d0();
    M_0040a7b0();
    M_0040a8c0();
    M_0040a660();
    M_00406a80();
    M_00406cf0();
    M_0040c340();
    M_004100e0();
    M_00410800();
    if (m_a795c == 3)
        M_004048f0(0x10, 200, 0xc0, "@0PRESS ENTER KEY");
}

void Game::M_0040a490()
{
    int x1;
    int y1;
    int x2;
    int y2;
    int id;

    m_a7950 = 0x146;
    id = m_87cbc4->m_7d5228;
    M_0040b4d0(id, 1, &x1, &y1);
    M_0040b4d0(id, 2, &x2, &y2);
    m_87cbc4->m_7d522c = x2 / 2 + x1;
    m_87cbc4->m_7d5230 = y2 / 2 + y1;
}

void Game::M_0040a520(int *x, int *y, int flag)
{
    int cur;
    int scale;

    if (m_a795c == 4 || m_a795c == 3 || m_a795c == 8) {
        cur = *y;
        if (flag != 0) {
            *y = (m_87cbc4->m_7d523c - 0x146) * 100 / 100 + cur;
        } else {
            scale = m_87cbc4->m_7d523c;
            *y = (0x1cc - scale) * (cur - 0x146) / 0x86 + scale;
        }
    }
}

int Game::M_0040b430(int id, int kind, int field)
{
    int result;

    result = 0;
    M_0040af40(3, id);
    M_0040aff0(5, m_87cbd4->m_2c[kind]);
    switch (field) {
    case 0:
        result = m_87cbec->m_4;
        break;
    case 1:
        result = m_87cbec->m_8;
        break;
    case 2:
        result = m_87cbec->m_0;
        break;
    case 3:
        result = m_87cbec->m_c;
        break;
    case 4:
        result = m_87cbec->m_10;
        break;
    }
    return result;
}

void Game::M_0040b4d0(int id, int kind, int *x, int *y)
{
    M_0040af40(3, id);
    M_0040aff0(5, m_87cbd4->m_2c[kind]);
    *x = m_87cbec->m_4;
    *y = m_87cbec->m_8;
}

void Game::M_0040b520(int id, int kind, int field, int value)
{
    M_0040af40(3, id);
    M_0040aff0(5, m_87cbd4->m_2c[kind]);
    switch (field) {
    case 0:
        m_87cbec->m_4 = value;
        break;
    case 1:
        m_87cbec->m_8 = value;
        break;
    case 2:
        m_87cbec->m_0 = value;
        break;
    case 3:
        m_87cbec->m_c = value;
        break;
    case 4:
        m_87cbec->m_10 = value;
        break;
    }
}

void Game::M_0040b7b0(int a, int b, int value)
{
    M_0040aff0(4, a);
    M_0040aff0(5, b);
    memcpy(m_87cbdc, m_87cbec, 5 * sizeof(int));
    if (value > 0)
        m_87cbdc->m_0 = value;
}

void Game::M_0040b800(int a, int b)
{
    M_0040aff0(4, a);
    M_0040aff0(5, b);
    m_87cbdc->m_4 = m_87cbdc->m_4 + m_87cbec->m_4;
    m_87cbdc->m_8 = m_87cbdc->m_8 + m_87cbec->m_8;
}

void Game::M_0040b940(int n)
{
    int i;

    if (n >= 0 && n < 0x4000) {
        M_0040af40(3, n);
        for (i = 1; i < 0x19; i++) {
            if (m_87cbc4->m_7d5210 <= m_87cbd4->m_2c[i]) {
                M_0040aff0(5, m_87cbd4->m_2c[i]);
                m_87cbec->m_0 = 0;
            }
        }
        for (i = 0x1a; i < 0x59; i++) {
            if (m_87cbc4->m_7d5210 <= m_87cbd4->m_2c[i]) {
                M_0040aff0(5, m_87cbd4->m_2c[i]);
                m_87cbec->m_0 = 0;
            }
        }
        memset(m_87cbd4, 0, 100 * sizeof(int));
    }
}

void Game::M_0040bea0(int x, int y, int tile)
{
    if (tile >= 0 && m_87cbf8 >= tile && x < 0x280 && y < 0x1e0)
        M_0040eb70(x, y, tile);
}

void Game::M_0040bee0(int a, int b, int tile, float scale)
{
    if (tile >= 0 && m_87cbf8 >= tile && 0.0 < scale && scale < 4.0)
        M_0040e7a0(a, b, tile, scale);
}

void Game::M_0040bf30(int a, int b, int tile, float scale)
{
    if (tile >= 0 && m_87cbf8 >= tile && 0.0 < scale && scale < 4.0)
        M_0040e9f0(a, b, tile, scale);
}

void Game::M_0040a8c0()
{
    int i;

    DAT_00a45e38 = 1;
    i = m_87cbc4->m_7c520c;
    while (--i >= 0)
        M_0040a900(i);
    DAT_00a45e38 = 0;
}

int Game::M_0040be00(int a)
{
    if (M_0040e360(a) < 0)
        return 0;
    if (m_8ef6dc == 0)
        return 0;
    return *m_8ef6dc;
}

int Game::M_0040b3d0(int a)
{
    return M_0040be00(M_0040b410(a));
}

void Game::M_0040be30(int a, int *x, int *y)
{
    if (M_0040e360(a) < 0) {
        *y = 0;
        *x = 0;
        return;
    }
    if (m_8ef6dc == 0) {
        *y = 0;
        *x = 0;
        return;
    }
    *x = m_8ef6dc[0];
    *y = m_8ef6dc[1];
}

void Game::M_0040c250(int y, const char *text)
{
    int width;

    width = M_0040be00(M_0040bf80(3, 0));
    M_0040bfa0(0x140 - ((strlen(text) - 2) * width >> 1), y, (char *)text);
}

void Game::M_0040c2a0(int progress)
{
    M_0040a640();
    M_0040c250(0xc0, "@aNOW LOADING");
    M_004022e0(0x98, 0x118, 0x150, 0x28, 7);
    M_004022e0(0x9c, 0x11c, 0x148, 0x20, 0);
    M_004022e0(0xa0, 0x120, (progress * 5 + 5) * 0x40 / 0x7f, 0x18, 2);
    if (FUN_00412800() == 0)
        FUN_004127a0();
}

void Game::M_0040c710(BankRec *bank)
{
    if (bank->m_00[1] != 0) {
        free((void *)bank->m_00[1]);
        bank->m_00[1] = 0;
    }
    if (bank->m_14 != 0) {
        free(bank->m_14);
        bank->m_14 = 0;
    }
    if (bank->m_10 != 0) {
        free(bank->m_10);
        bank->m_10 = 0;
    }
}

int Game::M_0040c970(char *text)
{
    int len;
    int row;

    len = (int)strlen(text) / 2 + 2;
    row = len;
    if (row > 0x32)
        row = 0x32;
    return DAT_004e26a8[row] * DAT_00a460bc * len / 800;
}

void Game::M_0040c9c0(BankRec *bank)
{
    int i;
    unsigned char *p;

    i = 0;
    p = (unsigned char *)bank->m_00[1];
    if (bank->m_00[2] > 0) {
        do {
            if (*p < 0x20)
                *p = 0;
            i++;
            p++;
        } while (i < bank->m_00[2]);
    }
}
