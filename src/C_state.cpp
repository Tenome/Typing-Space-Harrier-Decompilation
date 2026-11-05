#include <string.h>
#include "game.h"

void Game::M_0040f840()
{
    DAT_00a3d6f0 = 0;
    DAT_008f1004 = 0;
    DAT_00a45e28 = 0;
}

void Game::M_0040f860()
{
    if (DAT_00a3d6f0 == 0 && m_398 == 0)
        DAT_00a3d6f0 = -1;
}

int Game::M_0040f880()
{
    return 0 < DAT_00a3d6f0;
}

void Game::M_0040f890()
{
    DAT_00a3d6f0 = -2;
}

void Game::M_0040fa70(int v)
{
    DAT_00a3d6f0 = v;
}

void Game::M_0040fa80()
{
    if (m_a795c != 3 && m_8b0 == 0)
        m_3c0--;
    if (m_3c0 <= 0)
        M_0040f890();
}

void Game::M_0040fc40()
{
    m_a7964 = 0;
}

int Game::M_004100c0()
{
    int hit = 0;

    if (*DAT_0043065c[*m_3d8] == '*')
        hit = 1;
    return hit;
}

void Game::M_00410200(int from)
{
    int i;
    Rec418 *rec;

    if (from < 0x20) {
        for (i = from; i < 0x20; i++) {
            rec = &DAT_0099d780[i];
            DAT_0099d6bc = rec;
            memset(rec, 0, sizeof(Rec418));
        }
    }
}

void Game::M_00410240(int n)
{
    DAT_0099d6bc = &DAT_0099d780[n];
}

void Game::M_00410260()
{
    DAT_0099d6bc = &DAT_0099d780[*m_3d8 + 1];
}

void Game::M_00410320(int n)
{
    M_00410260();
    DAT_0099d6bc->m_00[0x10 + n]++;
}

int Game::M_00410360(int count)
{
    int i;

    M_00410340();
    for (i = 0; i < count; i++) {
        int id = M_0040bbe0();
        if (id == 0) {
            M_00410340();
            return DAT_00430658;
        }
        DAT_0099d5bc[i] = id;
    }
    DAT_004f0530 = count;
    return 0;
}

void Game::M_00410730()
{
    M_00402730(0xa8, 0xb8, 0x130, 0x70, 2, 1);
    M_004063e0();
}

void Game::M_00410af0()
{
    DAT_008f544c->m_08 = 1;
}

int Game::M_00410ba0()
{
    int len = DAT_008f544c->m_0c;
    int i;

    if (len == 0)
        return 0;
    for (i = 0; i < len; i++) {
        if (DAT_008f544c->m_50[i] != '@')
            return 0;
    }
    return 1;
}

void Game::M_00410bd0(char key)
{
    int len = DAT_008f544c->m_0c;
    int i;

    if (len != 0) {
        for (i = 0; i < len; i++) {
            if (DAT_008f544c->m_50[i] == key) {
                DAT_008f544c->m_50[i] = '@';
                return;
            }
        }
    }
}

void Game::M_00410c00()
{
    DAT_004efa10 = DAT_00a3d360;
    memset(DAT_00a3d360, 0, sizeof(DAT_00a3d360));
}
