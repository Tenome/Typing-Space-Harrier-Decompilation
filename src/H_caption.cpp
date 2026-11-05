#include <string.h>
#include "game.h"

/* Slides the caption row towards its target and draws it one letter per 16-pixel cell. */
void Game::M_00410900()
{
    char letter[16];
    char text[32];
    char ch;
    int blink;
    int x;
    int y;
    int i;
    int index;

    letter[1] = 0;
    if (DAT_008f544c->m_9c != 0) {
        if (DAT_008f544c->m_98 == 0) {
            DAT_008f544c->m_a0 = DAT_008f544c->m_90;
            DAT_008f544c->m_a4 = DAT_008f544c->m_94;
            DAT_008f544c->m_a8 = (0x28 - strlen(DAT_008f544c->m_10)) * 8;
            DAT_008f544c->m_ac = 8;
        }
        DAT_008f544c->m_98++;
        DAT_008f544c->m_90 = (DAT_008f544c->m_a8 - DAT_008f544c->m_a0) * DAT_008f544c->m_98 / DAT_008f544c->m_9c + DAT_008f544c->m_a0;
        DAT_008f544c->m_94 = (DAT_008f544c->m_ac - DAT_008f544c->m_a4) * DAT_008f544c->m_98 / DAT_008f544c->m_9c + DAT_008f544c->m_a4;
        if (DAT_008f544c->m_98 >= DAT_008f544c->m_9c)
            DAT_008f544c->m_9c = 0;
    }
    y = DAT_008f544c->m_94;
    x = DAT_008f544c->m_90;
    blink = (m_8a8 & 7) < 4;
    for (i = 0; i < DAT_008f544c->m_0c; i++) {
        ch = DAT_008f544c->m_10[i];
        if (ch == ' ') {
            x += 0x10;
        } else {
            index = 0;
            if (DAT_008f544c->m_50[i] == '@')
                index = blink ? 3 : 1;
            letter[0] = ch;
            strcpy(text, DAT_004e295c[index]);
            strcat(text, letter);
            M_0040bfa0(x, y, text);
            x += 0x10;
        }
    }
}
