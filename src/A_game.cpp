#include <string.h>
#include "game.h"

Game::Game()
{
    if (!FUN_00412800())
        FUN_004127a0();
    M_00410460(-1);
    m_8b4 = 1;
    m_8a8 = 0;
    m_3d4 = 0;
    m_a795c = 0;
    m_a7960 = 0;
    m_3cc = 0;
    DAT_00a23308 = 0;
    DAT_0099d778 = 0;
    DAT_009016fc = 0;
    M_00406680(-1);
    M_00410200(0);
    M_00403690();
    M_004014d0(0);
    M_0040bce0();
    M_004037f0();
    M_0040c650();
    M_00401290();
    M_00405b80();
    M_00404b20();
    M_00410c00();
    M_00410c20();
    m_3c4 = 0;
}

void Game::M_004010d0()
{
    m_8ac = -1;
}

void Game::M_004010e0()
{
    if (++m_8a8 >= 0x100)
        m_8a8 = 0;
}

void Game::M_00401100()
{
    M_004010d0();
    m_a795c = 2;
}

void Game::M_00401120()
{
    switch (m_a795c) {
    case 4:  M_00407850(); break;
    case 8:  M_00404000(); break;
    case 3:  M_00403850(); break;
    case 2:  M_00404490(); break;
    case 5:  M_00404940(); break;
    case 6:  M_004052e0(); break;
    case 7:  M_00405770(); break;
    case 0:  M_004042a0(); break;
    case 9:  M_00403870(); break;
    case 10: M_00403880(); break;
    case 11: M_00403cf0(); break;
    case 12: M_004043d0(); break;
    }
    M_004010e0();
    M_0040ef00();
}

void Game::M_00401290()
{
    M_004012a0();
}

void Game::M_004012a0()
{
    char path[0x108];
    SeEntry *e;
    char c;

    DAT_00901700 = e = DAT_004306e8;
    c = *e->name;
    for (;;) {
        if (c == '\0' || c == '*') {
            DAT_00901700 = e;
            return;
        }
        strcpy(path, DAT_011215bc);
        strcat(path, "SE\\");
        strcat(path, e->name);
        strcat(path, ".WAV");
        DAT_00901700 = e;
        FUN_0041e260(path, e->id);
        e++;
        c = *e->name;
    }
}

void Game::M_004013b0()
{
    SeEntry *e;
    char c;

    DAT_00901700 = e = DAT_004306e8;
    c = *e->name;
    while (c != '\0') {
        if (e->id != 3)
            FUN_0041e290(e->id);
        e++;
        DAT_00901700 = e;
        c = *e->name;
    }
    M_004089a0(-1);
}

void Game::M_00401400(int n)
{
    if (n < 0 || n > 0x20)
        n = 0;
    m_8c0 = n;
    m_8c4 = &m_9d4[n];
}

void Game::M_00401440(int n)
{
    if (n < 0 || n > 0x20)
        n = 0;
    m_8cc = n;
    m_8c8 = &m_9d4[n];
}

void Game::M_00401480(int n)
{
    m_8b8 = n;
    if (n < 0 || n > 0x20) {
        m_8b8 = -1;
        m_8bc = DAT_00430658;
        return;
    }
    M_00401400(n);
    m_8bc = m_8c4->m_8;
}

void Game::M_00401630(int n)
{
    M_00401440(n);
    m_8c8->m_14 = 0;
    m_8c8->m_18 = 0;
    m_8c8->m_0 = 0;
    m_8c8->m_4 = 0;
    m_8c8->m_80 = 0;
    m_8c8->m_84 = 1;
    m_8c8->m_50ec = 0;
    m_8c8->m_50e8 = -1;
    m_8c8->m_50f0 = 0;
}

void Game::M_004010c0()
{
    M_0040c6e0();
}
