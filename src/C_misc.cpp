#include <windows.h>
#include <string.h>
#include "game.h"

void Game::M_0040e2d0()
{
    memset(m_8eced8, 0, sizeof(m_8eced8));
    m_8ef6d8 = m_8eced8[0];
    m_8ef790 = m_8ef6e0;
    m_8ef794 = m_8ef738;
    m_8ececc = 8;
    M_0040f0a0();
    m_8eced0 = m_8ef798;
    m_8eced4 = m_8eced0 + 0x1900000;
    M_0040e380();
}

int Game::M_0040e340(int n)
{
    m_8ef6d8 = m_8eced8[n];
    return n;
}

int Game::M_0040e360(int n)
{
    m_8ef6dc = m_8eced8[n];
    return n;
}

void Game::M_0040f030()
{
    char path[0x108];

    wsprintfA(path, "%sbg\bg.bin", DAT_011215bc);
    FUN_004189d0(path);
}
