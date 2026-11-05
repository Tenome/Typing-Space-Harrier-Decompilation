#include <string.h>
#include <windows.h>
#include "game.h"

void Game::M_004082f0(int n)
{
    if (n >= 0 && n < 0x40)
        m_8a4 = m_3d8 + (n * 3 + 0xf);
}

int Game::M_00408460()
{
    int count;
    int next;
    int expected;
    int ready;
    int *stage;

    ready = 0;
    m_3c8 = m_3d8[0];
    count = M_004084d0();
    m_3d8[0xd0 + m_3c8 * 2] = count;
    stage = m_3d8;
    next = m_3c8 + 1;
    expected = stage[0xcd + next * 2];
    if (next == 0x12 && expected == count + 2)
        m_87fcb4 = 1;
    if (expected <= count && stage[0xc] >= 0x40)
        ready = 1;
    return ready;
}

int Game::M_004084d0()
{
    int count;
    int i;
    int total;
    int *rec;

    count = 0;
    i = 0;
    m_3c8 = m_3d8[0];
    total = m_3d8[0xe];
    if (total > 0) {
        do {
            M_004082f0(i);
            rec = m_8a4;
            if (m_3c8 + 1 == rec[1]) {
                if (rec[0] == 0 && m_87fcc8[rec[2]] == 2)
                    rec[0] = 1;
                if (m_8a4[0] != 0)
                    count++;
            }
            i++;
        } while (i < total);
    }
    return count;
}

int Game::M_00408540()
{
    int *stage;
    int next;
    int last;

    next = m_3d8[0] + 1;
    M_00410460(m_378);
    M_0040f380();
    stage = m_3d8;
    if (next <= stage[2]) {
        stage[0]++;
        m_3d8[3] = 0;
        stage = m_3d8;
        next = stage[0] + 1;
    }
    stage[0xc] = -0x40;
    m_3d8[0xd] = 1;
    last = m_3d8[2];
    if (last < next)
        return 2;
    if (next == last) {
        DAT_008f1000->m_180 = 0;
        m_87fcc4 = 0;
        m_398 = 1;
    }
    return 1;
}

void Game::M_004085d0()
{
    int ready;

    ready = M_00408460();
    if (DAT_00a45e28 == 0 && ready != 0)
        M_00408540();
}

void Game::M_004085f0(int n)
{
    int now;

    if (DAT_004bddf8[n].m_3c != 0) {
        now = m_3d8[0xc];
        if (now >= DAT_004bddf8[n].m_10 && DAT_004bddf8[n].m_40 + DAT_004bddf8[n].m_10 > now)
            M_004093b0(n, DAT_004bddf8[n].m_1c, DAT_004efbd8[DAT_004bddf8[n].m_1c], -1);
    }
}

void Game::M_00408980(int n)
{
    if (n >= 0 && DAT_004e2328[n] != 0)
        M_004089a0(DAT_004e2328[n]);
}

void Game::M_00408cb0(int n)
{
    int i;

    m_8e3cc8[n] = 0;
    m_8a2cc8[n] = 0;
    for (i = 0; i < 0x20; i++) {
        m_8a3cc8[n][i] = DAT_00430658;
        m_882cc8[n][i] = DAT_00430658;
        m_8c3cc8[n][i] = 0;
    }
}

void Game::M_00409210(int n, int value)
{
    int i;
    int *id;
    Rec_87cbc8 *rec;

    M_00409270(n);
    i = 0;
    if (m_8ecec8 > 0) {
        id = m_8eccc8;
        do {
            M_0040af40(0, *id);
            rec = m_87cbc8;
            if (rec->m_0 != 0 && rec->m_c == n)
                rec->m_18 = value;
            i++;
            id++;
        } while (i < m_8ecec8);
    }
}
