#include "game.h"

inline void Game::PruneStageSlots(int stage)
{
    int kind;
    int count;
    int i;
    int live;
    int kept;
    short entry;
    int needed;
    int ticks;

    kind = DAT_004bddf8[stage].m_2c;
    if (kind >= 0x38 && kind <= 0x3c) {
        count = m_8a2cc8[stage];
        needed = kind - 0x37;
        ticks = m_8e3cc8[stage];
        if (ticks != 0 && ticks % 100 == 0) {
            live = 0;
            for (i = 0; i < count; i++) {
                if (m_8c3cc8[stage][i] == 0)
                    live++;
            }
            if (live != needed) {
                kept = 0;
                for (i = 0; i < count; i++) {
                    entry = m_8c3cc8[stage][i];
                    if (entry <= 0) {
                        m_8a2cc8[stage]--;
                    } else {
                        m_8c3cc8[stage][kept] = entry;
                        m_8a3cc8[stage][kept] = m_8a3cc8[stage][i];
                        kept++;
                    }
                }
                for (i = kept; i < count; i++) {
                    m_8a3cc8[stage][i] = DAT_00430658;
                    m_8c3cc8[stage][i] = 0;
                }
            }
        }
    }
}

void Game::M_00408d00()
{
    int stage;
    int row;
    int slot;
    int i;
    int total;
    int percent;

    for (stage = 0; stage < 0x20; stage++) {
        M_00401400(stage);
        if (m_8c4->m_0 == 0)
            continue;
        row = m_8c4->m_8;
        if (row < 0)
            continue;
        slot = DAT_00430658;
        for (i = 0; i < m_8a2cc8[row]; i++) {
            if (stage == m_8a3cc8[row][i]) {
                slot = i;
                break;
            }
        }
        if (slot < 0)
            continue;
        total = m_8c4->m_88;
        if (total <= 0) {
            m_8c3cc8[row][slot] = 100;
            m_8a3cc8[row][slot] = -1;
            continue;
        }
        percent = m_8c4->m_50e8 * 100 / total;
        if (percent >= 100) {
            percent = 100;
            m_8a3cc8[row][slot] = -1;
        }
        m_8c3cc8[row][slot] = percent;
        m_8e3cc8[row] = 0;
        for (i = 0; i < m_8a2cc8[row]; i++)
            m_8e3cc8[row] += m_8c3cc8[row][i];
        if (m_8e3cc8[row] % 100 == 0) {
            PruneStageSlots(row);
            M_004090a0(row);
        }
    }
}
