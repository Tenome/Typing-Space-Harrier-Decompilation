#include <string.h>
#include <windows.h>
#include "game.h"

void Game::M_004067b0()
{
    memset(&DAT_009a5e98, 0, 0x4404);
}

int Game::M_004067d0(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k)
{
    int slot = M_00406a50();

    if (slot >= 0) {
        DAT_00a3d6ec->m_0 = 1;
        DAT_00a3d6ec->m_8 = a;
        DAT_00a3d6ec->m_14 = b;
        DAT_00a3d6ec->m_c = b;
        DAT_00a3d6ec->m_18 = c;
        DAT_00a3d6ec->m_10 = c;
        DAT_00a3d6ec->m_1c = d;
        DAT_00a3d6ec->m_20 = e;
        DAT_00a3d6ec->m_24 = 0;
        DAT_00a3d6ec->m_28 = g;
        DAT_00a3d6ec->m_2c = h;
        DAT_00a3d6ec->m_30 = i;
        DAT_00a3d6ec->m_34 = f;
        DAT_00a3d6ec->m_38 = j;
        DAT_00a3d6ec->m_3c = k;
    }
    return slot;
}

void Game::M_004068e0(int n)
{
    if (n < 0 || n >= 10)
        M_00406910(n, 4);
    else
        M_00406910(n, 8);
}

void Game::M_00406c40()
{
    int i;

    DAT_008f1000 = &DAT_004efa40;
    for (i = 0; i < 4; i++)
        M_00406cb0(i);
    DAT_008f1000->m_188 = 0;
    DAT_008f1000->m_184 = 0;
    DAT_008f1000->m_180 = 0;
}

void Game::M_00406c90(int n)
{
    DAT_00a232a0 = &DAT_008f1000->m_players[n];
}

void Game::M_00406cb0(int n)
{
    int i;

    M_00406c90(n);
    memset(DAT_00a232a0, 0, sizeof(Player));
    for (i = 0; i < 16; i++)
        DAT_00a232a0->m_8[i] = 0;
}

void Game::M_00406cf0()
{
    if (*m_3d8 + 1 <= 0x12) {
        M_00407230();
        M_00406d20();
    }
}

void Game::M_00406d20()
{
    int redraw = 0;
    int secs, nextSecs;
    int i, x, before;
    char text[0x20];

    if (*m_3d8 + 1 <= 0x12 && DAT_008f1000->m_180 > 0) {
        DAT_008f1000->m_180--;
        m_87fcc4 = 0x32;
        if (DAT_008f1000->m_180 <= 0 && m_a795c != 3) {
            DAT_008f1000->m_180 = 0;
            m_87fcc4 = 0;
            DAT_00a460c8 = 0;
        }
        secs = DAT_008f1000->m_180 / DAT_00a460bc;
        nextSecs = (DAT_008f1000->m_180 + 1) / DAT_00a460bc;
        wsprintfA(text, DAT_004e2198[secs < 5], secs);
        M_0040bfa0(0x120, 0x20, text);
        M_0040bfa0(0x120, 0x40, "@3AUTO");
        if (secs != nextSecs) {
            M_00408650(m_a797c + 0xe);
            m_a797c = m_a797c == 0;
        }
    }
    if (DAT_008f1000->m_188 > 0) {
        DAT_008f1000->m_188--;
        M_0040c250(0xa0, DAT_004e2144[DAT_008f1000->m_184]);
    }
    i = 0;
    x = 0x18;
    do {
        M_00406c90(i);
        M_0040bea0(x - 0x16, 0, M_0040bf80(0x7b, i + 4));
        before = DAT_00a232a0->m_4c;
        M_00406ef0(x, 0, i);
        if (before != DAT_00a232a0->m_4c)
            redraw = 1;
        x += 0x38;
        i++;
    } while (x < 0xf8);
    if (redraw) {
        M_00408650(m_a797c + 0xe);
        m_a797c = m_a797c == 0;
    }
}
