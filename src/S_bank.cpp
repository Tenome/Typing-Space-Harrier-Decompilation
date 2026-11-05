#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "game.h"

int Game::M_0040c9f0(BankRec *bank, int store)
{
    unsigned char *p;
    int count;
    int state;
    int i;
    int lineStart;

    p = (unsigned char *)bank->m_00[1];
    count = 0;
    state = 0;
    lineStart = 1;
    for (i = 0; i < bank->m_00[2]; i++, p++) {
        if (*p == 0x1a) {
            *p = 0;
            break;
        }
        if (*p < 0x20) {
            lineStart = 1;
        } else if (lineStart) {
            lineStart = 0;
            switch (state) {
            case 0:
                if (store != 0)
                    bank->m_14[count] = (char *)p;
                state = 1;
                break;
            case 1:
                if (store != 0)
                    bank->m_10[count] = (char *)p;
                count++;
                state = 0;
                break;
            }
        }
    }
    bank->m_0c = count;
    return count;
}
