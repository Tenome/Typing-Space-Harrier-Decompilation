#include <string.h>
#include "game.h"

void Game::M_00402930()
{
    Slot *s = m_8c4;

    M_0040f1c0(s->m_3c + s->m_1c, s->m_40 + s->m_20, s->m_94);
}

int Game::M_00402a30()
{
    char text[0x1400];

    return M_00402a80(text) * 10;
}

int Game::M_00402a50()
{
    int remaining = m_8c4->m_84;
    int w = DAT_00a460bc * 5;

    if (remaining < DAT_00a460bc * 10)
        w = (DAT_00a460bc * remaining * 5) / (DAT_00a460bc * 10);
    return w;
}

int Game::M_00402be0()
{
    int keys = 0;
    int r;
    int n;

    if (m_8c4->m_60 != 0)
        return 0;
    if (!M_00403670(0))
        return 0;
    for (n = 0; n < 0x1f; n++) {
        if (!M_00403670(0))
            break;
        r = M_00402c50();
        if (r <= 0) {
            if (keys == 0)
                keys = DAT_00430658;
            break;
        }
        keys |= r;
        M_00403650();
    }
    return keys;
}

void Game::M_00402e90()
{
    int i;
    int n;
    int key;

    m_a7980 = 0;
    for (i = 0; i < 0xa00; i++)
        m_8c4->m_2894[i] = -1;
    n = 0;
    for (i = 0; i < 0xa00; i++) {
        if (m_8c4->m_1494[m_a7980] == 0)
            return;
        key = M_00403570();
        if (key >= 0 && n < 0xa00)
            m_8c4->m_2894[n++] = key;
    }
}

int Game::M_00402c50()
{
    int order[12];
    int matched[12];
    int hits[12];
    int result;
    int nhits;
    int step;
    int found;
    int any;
    int i, k;
    int id;
    int *ord;
    int *hit;
    char ch;

    if (m_8c4->m_60 != 0)
        return 0;
    if (!M_00403670(0))
        return 0;
    for (i = 0; i < 12; i++)
        order[i] = i;
    nhits = 0;
    id = m_8c4->m_50dc;
    result = 0;
    order[id] = 0;
    order[0] = id;
    for (step = 0; step < 1; step++) {
        for (i = 0; i < 12; i++)
            matched[i] = 0;
        any = 0;
        ch = M_00403670(step);
        if (!ch)
            break;
        found = 0;
        hit = &hits[nhits];
        ord = order;
        for (i = 0; i < 12; i++, ord++) {
            id = *ord;
            if (m_8c4->m_5094[id * KEY_LEN] != 0 && m_8c4->m_5094[id * KEY_LEN] == ch) {
                found = 1;
                matched[id] = 1;
                any = 1;
                result |= 1;
                if (m_8c4->m_5094[id * KEY_LEN + 1] == 0) {
                    result |= 2;
                    break;
                }
                for (k = 0; k < 5; k++)
                    m_8c4->m_5094[id * KEY_LEN + k] = m_8c4->m_5094[id * KEY_LEN + 1 + k];
                m_8c4->m_5094[id * KEY_LEN + 5] = 0;
                *hit = id;
                nhits++;
                hit++;
            }
        }
        M_00410280(ch, found);
        if (nhits > 0)
            m_8c4->m_50dc = hits[0];
        if (result & 2) {
            M_00402f10(id);
        } else if (any) {
            for (i = 0; i < 12; i++) {
                if (matched[i] == 0)
                    m_8c4->m_5094[i * KEY_LEN] = 0;
            }
        } else {
            result = 0;
        }
        if (m_8c4->m_50ec != 0) {
            result |= 4;
            M_00401480(-1);
            break;
        }
    }
    if (result == 0)
        result = DAT_00430658;
    return result;
}

int Game::M_004034c0(char *out, int id)
{
    int sel = 0;

    out[1] = 0;
    out[0] = 0;
    if (id >= 0x400 && id <= 0x409) {
        out[1] = 0;
        out[0] = (char)id + '0';
    } else if (id >= 0x40a && id <= 0x423) {
        out[0] = (char)id + 'W';
    } else if (id >= 0 && id < 0x19e) {
        sel = id;
        if (id >= 0xcf)
            sel = id - 0xcf;
        sel = m_3c[sel];
        strcpy(out, DAT_00429040[id].rom[sel]);
    }
    return sel;
}

char Game::M_00403670(int n)
{
    char c = DAT_00a461d0[n];

    if (c >= 'A' && c <= 'Z')
        c += ' ';
    return c;
}
