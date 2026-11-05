#include <string.h>
#include "game.h"

void Game::M_00407fd0()
{
    int stage;
    int phase;
    int song;
    int next;
    int owner;

    stage = m_3d8[0];
    next = stage + 1;
    phase = m_3d8[0xc];
    song = m_3d8[stage * 2 + 0xd0];
    if (m_a795c == 4)
        DAT_0099d778 = stage;
    if (m_3d0 == 0 || DAT_00a23308 >= m_3d8[0]) {
        if (phase == 0 && m_3d0 == 0)
            m_87fcac = DAT_00a460bc * 5;
    } else if (phase == 0) {
        M_004010d0();
        M_00403730();
        m_a795c = 0xb;
        DAT_00a460c8 = phase;
        return;
    }
    if (M_004100c0() != 0 && phase == 1)
        M_0040fc50();
    if (next >= 0x12) {
        if (m_87fcb0 != song) {
            owner = m_3d0;
            m_87fcb0 = song;
            if (owner == 0)
                m_87fcac = DAT_00a460bc * 5;
            if (*DAT_004306c0[song] == '*')
                M_0040fc50();
        }
    } else {
        m_87fcb0 = -1;
    }
    if (next == 0x13) {
        if (phase == 0) {
            DAT_008f1000->m_180 = 0;
            m_87fcc4 = 0;
            m_398 = 1;
            m_8b0 = 1;
            m_3c0 = 1;
            return;
        }
    } else if (next == 0x14 && phase >= 0) {
        owner = m_87fcc4;
        m_a795c = 0;
        m_39c = 1;
        if (owner == 0)
            m_c = 1;
    }
}

void Game::M_00408320(int stage)
{
    int i;
    int s;
    int row;
    int stageNo;
    int flag;

    m_3d8 = m_3dc;
    memset(m_3dc, 0, sizeof(m_3dc));
    m_3d8[0xe] = 0;
    m_3d8[0] = stage;
    m_3d8[1] = stage;
    m_3d8[3] = 1;
    m_3d8[4] = -1;
    m_3d8[0xc] = 0;
    m_3d8[0xd] = 1;
    for (i = 1; *DAT_0043065c[i - 1] != 0; i++)
        m_3d8[2] = i;
    for (s = 0; s < 0x20; s++) {
        row = 0;
        stageNo = DAT_004bddf8[0].m_0;
        flag = DAT_004bddf8[0].m_4;
        while (stageNo >= 0) {
            if (s + 1 == stageNo && flag != 0) {
                M_004082f0(m_3d8[0xe]);
                m_3d8[0xe]++;
                m_3d8[0xcf + s * 2]++;
                m_3d8[0xd0 + s * 2] = 0;
                m_8a4[0] = 0;
                m_8a4[1] = s + 1;
                m_8a4[2] = row;
            }
            row++;
            stageNo = DAT_004bddf8[row].m_0;
            flag = DAT_004bddf8[row].m_4;
        }
    }
}
