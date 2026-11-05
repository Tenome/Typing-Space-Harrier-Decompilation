#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "game.h"

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
    m_8ecec8 = 0;
    for (i = 0; i < 0x80; i++)
        m_8eccc8[i] = 0;
    m_87fcac = 0;
    m_87fcb0 = -1;
}















