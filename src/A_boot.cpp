#include "game.h"

void Game::M_004014d0(int a)
{
    int i;

    M_00408320(a);
    m_3b8 = 0;
    m_390 = 0;
    m_39c = 0;
    m_398 = 0;
    m_3d0 = 0;
    M_00410200(1);
    M_00401480(-1);
    M_00406680(0);
    m_3bc = 0;
    m_87fcb4 = 0;
    m_3c0 = m_37c + 1;
    m_8b0 = 0;
    m_a7978 = 0;
    m_a7974 = 0;
    m_a7970 = 0;
    m_a797c = 0;
    for (i = 0; i < 0x20; i++)
        M_00401630(i);
    m_950 = 0;
    for (i = 0; i < 0x20; i++)
        m_954[i] = DAT_00430658;
    M_0040f840();
    M_00407ed0();
    M_0040c6a0();
    M_00409cc0();
    M_004010d0();
    M_00406140();
    M_004067b0();
    M_00406c40();
    M_0040f330();
    M_0040fc40();
    M_00410760();
    m_87cbc4->m_7d5228 = -1;
    M_00404110(0);
    m_3b0 = DAT_004bd59c[m_380] + DAT_004bd54c[m_378] + DAT_004bd574[m_37c];
    m_3b4 = 300;
}

int Game::M_00401930()
{
    int n = M_00401980();
    if (n < 0)
        return DAT_00430658;
    if (M_00401a00(n) < 0)
        return DAT_00430658;
    m_3b8++;
    M_00401400(n);
    return n;
}

int Game::M_00401980()
{
    int none = DAT_00430658;
    int i;

    for (i = 0; i < 0x20; i++) {
        if (M_004019b0(i) <= 0)
            return i;
    }
    return none;
}

int Game::M_004019b0(int n)
{
    int age = DAT_00430658;

    M_00401440(n);
    if (m_8c8->m_0 != 0)
        age = m_8c8->m_4;
    return age;
}

void Game::M_004019e0(int n, int v)
{
    M_00401440(n);
    m_8c8->m_4 = v;
}

int Game::M_00401a00(int n)
{
    int i;

    for (i = 0; i < 0x20; i++) {
        int v = M_004019b0(i);
        if (0 < v) {
            if (v < 0x400)
                v++;
            M_004019e0(i, v);
        }
    }
    M_004019e0(n, 1);
    return 0;
}

int Game::M_00401a40(int n, char c)
{
    int found = 0;
    Slot *s;
    int i;

    M_00401440(n);
    s = m_8c8;
    if (s->m_0 == 0)
        return 0;
    if (s->m_60 != 0)
        return 0;
    for (i = 0; i < 12; i++) {
        if (s->m_5094[i * KEY_LEN] != 0 && c == s->m_5094[i * KEY_LEN]) {
            found = 1;
            break;
        }
    }
    return found;
}

void Game::M_004016b0()
{
    int i;
    int best;
    int bestAge;
    char c;

    if (DAT_008f1000->m_180 <= 0 && m_a795c != 3 && *m_3d8 + 1 <= 0x12) {
        DAT_008f1000->m_180 = 0;
        m_87fcc4 = 0;
        if (*m_3d8 + 1 <= 0x12)
            DAT_00a460c8 = 0;
    }
    m_3b8 = 0;
    for (i = 0; i < 0x20; i++) {
        M_00401400(i);
        if (m_8c4->m_0 != 0)
            m_3b8++;
    }
    if (m_3b8 == 0)
        M_00401480(-1);
    if (DAT_00a46260 == 1) {
        if (m_87fcc4 == 0) {
            if (m_a795c != 3) {
                if (m_8b8 != -1) {
                    M_00401400(m_8b8);
                    m_8c4->m_50e8 = -1;
                    M_00402e90();
                    M_00402f10(0);
                }
                M_00401480(-1);
                if (*m_3d8 + 1 <= 0x12)
                    DAT_00a460c8 = 0;
                return;
            }
        } else if (m_a795c != 3) {
            if (m_8b8 != -1) {
                M_00401400(m_8b8);
                m_8c4->m_50e8 = -1;
                M_00402e90();
                M_00402f10(0);
            }
            M_00401480(-1);
            if (*m_3d8 + 1 <= 0x12)
                DAT_00a460c8 = 0;
            return;
        }
    }
    if (m_8b8 != -1) {
        M_00401400(m_8b8);
        if (m_8c4->m_0 != 0 && m_8c4->m_8 == m_8bc)
            return;
        M_00401480(-1);
    }
    if (m_3b8 == 0) {
        M_00403670(0);
    } else {
        c = M_00403670(0);
        if (c == 0)
            return;
        m_950 = 0;
        for (i = 0; i < 0x20; i++) {
            if (M_00401a40(i, c)) {
                m_954[m_950] = i;
                m_950++;
            }
        }
        switch (m_950) {
        case 0:
            break;
        case 1:
            M_00401480(m_954[0]);
            return;
        default:
            best = 0;
            bestAge = 0;
            for (i = 0; i < m_950; i++) {
                int slot = m_954[i];
                int age = M_004019b0(slot);
                if (bestAge < age) {
                    bestAge = age;
                    best = slot;
                }
            }
            M_00401480(best);
            return;
        }
    }
    M_00401480(-1);
}
