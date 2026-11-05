#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

void Game::M_004071c0()
{
    int active[4];
    int count;
    int i;
    int pick;

    count = 0;
    for (i = 0; i < 4; i++) {
        M_00406c90(i);
        if (DAT_00a232a0->m_0 == 1)
            active[count++] = i;
    }
    if (count == 0)
        return;
    pick = rand() % count;
    memset(DAT_00a46264, 0, sizeof(DAT_00a46264));
    DAT_00a46264[active[pick]] = 1;
}

void Game::M_0040ba00(int n)
{
    if (n >= 0 && n < 0x10000 && n >= m_87cbc4->m_7d5210) {
        M_0040aff0(5, n);
        memset(m_87cbec, 0, 0x14);
    }
}

NodeList::NodeList()
{
    head = 0;
    memset(&group, 0, sizeof(group));
    m_2c = 0;
}

void NodeList::FUN_00410f40()
{
    ListNode *node;
    ListNode *following;

    node = head;
    while (node) {
        following = node->next;
        if (node)
            delete node;
        node = following;
    }
    head = 0;
    memset(&group, 0, sizeof(group));
}
