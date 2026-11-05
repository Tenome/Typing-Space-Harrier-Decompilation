#include <windows.h>
#include <string.h>
#include "game.h"

void Game::M_00405f70()
{
    if (DAT_008f100c != 0) {
        DAT_00951fc8++;
        DAT_009016f4 = ((DAT_008f0ff0 - DAT_004efbd0) * DAT_00951fc8) / DAT_008f100c + DAT_004efbd0;
        DAT_009016f0 = ((DAT_008f0ff8 - DAT_004efbcc) * DAT_00951fc8) / DAT_008f100c + DAT_004efbcc;
        if (DAT_00951fc8 >= DAT_008f100c) {
            DAT_008f100c = 0;
            DAT_00951fc8 = 0;
        }
    }
}

void Game::M_00406070(int x, int y, int frames)
{
    int dx = DAT_008f0ff0 - x;
    int dy = DAT_008f0ff8 - y;

    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;
    if (dx >= 0x40 || dy >= 0x40)
        M_00408650(0x20);
    DAT_008f0ff8 = y;
    DAT_008f0ff0 = x;
    DAT_004efbd0 = DAT_009016f4;
    DAT_004efbcc = DAT_009016f0;
    DAT_00951fc8 = 0;
    DAT_008f100c = frames;
}

void Game::M_004060f0(int *x, int *y, int a, int b)
{
    *x = DAT_009016f4 + (a + 2 + (b / 10) * 6) * 0x28;
    *y = DAT_009016f0 + 0x70 + (b % 10) * 0x24;
}

void Game::M_00406140()
{
    int i;

    for (i = 0; i < 8; i++)
        M_00406160(i);
}

void Game::M_00406160(int n)
{
    int i;

    M_004061c0(n);
    M_00406210(0, 0, 1);
    M_00406240(0, 0);
    M_00406270(0, 0);
    for (i = 0; i < 0x40; i++)
        M_004062b0(i, DAT_004e1fd8);
    M_004061f0(-1, -1);
}

void Game::M_004061c0(int n)
{
    if (n < 0 || n >= 8)
        n = 0;
    DAT_00a3d700 = n;
    DAT_00951fcc = &DAT_0099cfd8[n];
}

void Game::M_004061f0(int a, int b)
{
    DAT_00951fcc->m_34 = a;
    DAT_00951fcc->m_38 = b;
}

void Game::M_00406210(int a, int b, int c)
{
    DAT_00951fcc->m_8[0] = a;
    DAT_00951fcc->m_8[1] = b;
    DAT_00951fcc->m_4 = c;
}

void Game::M_00406240(int a, int b)
{
    if (a <= 0)
        a = 1;
    if (b <= 0)
        b = 1;
    DAT_00951fcc->m_1c[0] = a;
    DAT_00951fcc->m_1c[1] = b;
}

void Game::M_00406270(int a, int b)
{
    DAT_00951fcc->m_14[0] = a;
    DAT_00951fcc->m_14[1] = b;
}

void Game::M_00406290(int *a, int *b)
{
    *a = DAT_00951fcc->m_14[0];
    *b = DAT_00951fcc->m_14[1];
}

void Game::M_004062b0(int n, char *text)
{
    if (n >= 0 && DAT_00951fcc->m_1c[1] * 2 > n)
        DAT_00951fcc->m_3c[n] = text;
}

int Game::M_00406350(int a, int b)
{
    Menu *m = DAT_00951fcc;

    /* RULE-EXCEPTION: the original null-tests an interior pointer (lea ecx,[eax+8]; test ecx,ecx) before indexing */
    if (&m->m_8 != 0)
        return m->m_1c[1] * a + b;
    return m->m_14[1] * b + a;
}

void Game::M_00406380()
{
    Menu *m = DAT_00951fcc;
    int count, sel, i, idx;
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
                    M_0040bfa0(m->m_24, m->m_30 * i + m->m_28, text);
                    m = DAT_00951fcc;
                }
                i++;
            } while (i < count);
        }
    }
}

int Game::M_00406600()
{
    int dx = DAT_00a46180 - DAT_00a4617c;
    int dy = DAT_00a46178 - DAT_00a46174;
    int pressed = 0;

    if (DAT_00a4625c != 0) {
        pressed = 1;
        M_00408650(DAT_00951fcc->m_34);
    }
    M_00406540(dx, dy);
    return pressed;
}

int Game::M_00406660()
{
    int r = 0;

    if (DAT_00a4625c != 0)
        r = 1;
    if (DAT_00a46260 != 0)
        r = -1;
    return r;
}

void Game::M_00406680(int a)
{
    if (a < 0)
        m_3a4 = 100000;
    M_004066c0(0);
    m_3a8 = 0;
    m_3ac = 300000;
}

void Game::M_004066c0(int a)
{
    m_3a0 = a;
}

void Game::M_004063e0()
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
                    n = strlen(text) - 2;
                    x = m->m_24;
                    y = m->m_30 * i + m->m_28;
                    if (text[1] >= 'a' && text[1] <= 'b') {
                        x -= n * 16;
                        y -= 16;
                    } else {
                        x -= n * 8;
                        y -= 8;
                    }
                    M_0040bfa0(x, y, text);
                    m = DAT_00951fcc;
                }
                i++;
            } while (i < count);
        }
    }
}

void Game::M_004062e0(int a, int b, int c, int d)
{
    DAT_00951fcc->m_10 = 1;
    if (a == DAT_00430658 && b == DAT_00430658 && c == DAT_00430658 && d == DAT_00430658)
        DAT_00951fcc->m_10 = 0;
    DAT_00951fcc->m_24 = a;
    DAT_00951fcc->m_28 = b;
    DAT_00951fcc->m_2c = c;
    DAT_00951fcc->m_30 = d;
}

void Game::M_00405770()
{
    int mode, cursorX, state, firstState;
    int done, idx, kana, count, value, i;
    int *dst;

    if (m_8ac < 0) {
        M_00403730();
        M_004061c0(5);
        M_00406210(0, 0, 0);
        M_00406240(3, 2);
        M_00406270(0, 0);
        M_004062e0(0x60, 0x28, 0x180, 0x1e);
        M_004061f0(0, 1);
        for (idx = 0; *DAT_004bd834[idx] != 0; idx++)
            M_004062b0(idx, DAT_004bd834[idx]);
        M_00405bd0();
        DAT_009016f0 = 0;
        DAT_009016f4 = 0;
        DAT_008f0ff8 = 0;
        DAT_008f0ff0 = 0;
        DAT_004efbcc = 0;
        DAT_004efbd0 = 0;
        DAT_00951fc8 = 0;
        DAT_008f100c = 0;
        M_004061c0(6);
        M_00406210(0, 0, 0);
        M_00406240(DAT_00a23304, DAT_00a23300);
        M_00406270(0, 0);
        M_004062e0(DAT_00430658, DAT_00430658, DAT_00430658, DAT_00430658);
        M_004061f0(0, 1);
        M_004061c0(7);
        M_00406210(0, 0, 0);
        M_00406240(0, 0);
        M_00406270(0, 0);
        M_004062e0(DAT_00430658, DAT_00430658, DAT_00430658, DAT_00430658);
        M_004061f0(0, 1);
        M_004061c0(5);
        DAT_004ef900 = 0;
        DAT_00a3d708 = 0;
    }
    M_00406290(&cursorX, &state);
    firstState = state;
    if (DAT_0099d77c == 0) {
        switch (state) {
        case 0:
            M_00406270(0, 0);
            break;
        case 1:
            M_00406270(m_38, 1);
            break;
        }
    }
    M_004061c0(DAT_0099d77c + 5);
    M_00406600();
    M_00405c50(DAT_0099d77c);
    done = M_00406660();
    M_00406290(&cursorX, &state);
    switch (DAT_0099d77c) {
    case 0:
        switch (state) {
        case 1:
            if (firstState == 1)
                m_38 = cursorX;
            break;
        }
        break;
    case 1:
        DAT_00a45e2c = cursorX;
        DAT_00a45e30 = state;
        M_00405fe0(cursorX);
        break;
    case 2:
        m_3c[DAT_00a3d6fc] = cursorX;
        break;
    }
    if (done != 0) {
        switch (DAT_0099d77c) {
        case 0:
        {
            if (done < 0)
                state = 0;
            switch (state) {
            case 0:
                m_a795c = 6;
                break;
            case 1:
                if (m_38 == 2) {
                    DAT_0099d77c = 1;
                } else {
                    dst = m_3c;
                    for (i = 0; i < 0xcf; i++) {
                        switch (m_38) {
                        case 0:
                            *dst = DAT_004bdab8[i];
                            break;
                        case 1:
                            *dst = DAT_00a45e40[i];
                            break;
                        }
                        dst++;
                    }
                }
                break;
            }
        }
            break;
        case 1:
            if (done < 0) {
                M_00406270(0, 0);
                DAT_0099d77c = 0;
                M_00405bd0();
                DAT_009016f0 = 0;
                DAT_009016f4 = 0;
                M_00406070(0, 0, 4);
            } else {
                DAT_0099d77c = 1;
                kana = DAT_004bd924[(state + (cursorX / 5) * 10) * 5 + cursorX % 5];
                if (kana != -1) {
                    count = DAT_00a23310[kana];
                    value = m_3c[kana];
                    if (count >= 2) {
                        DAT_0099d77c = 2;
                        DAT_00a3d6fc = kana;
                        DAT_008f141c = count;
                        M_004061c0(7);
                        M_00406210(0, 0, 0);
                        M_00406240(count, 0);
                        M_00406270(value, 0);
                        M_004062e0(DAT_00430658, DAT_00430658, DAT_00430658, DAT_00430658);
                        M_004061f0(0, 1);
                    }
                }
            }
            break;
        case 2:
            DAT_004ef900 = 0;
            DAT_0099d77c = 1;
            break;
        }
    }
    M_0040ef00();
    M_004061c0(DAT_0099d77c + 5);
    if (m_a795c != 7) {
        m_a795c = 6;
        M_004010d0();
        M_00403730();
        return;
    }
    m_8ac++;
    if (m_8ac >= 1000000)
        m_8ac = 1000000;
}
