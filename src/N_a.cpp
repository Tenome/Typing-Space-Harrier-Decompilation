#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"

// OPEN: 2 bytes: orig shares eax=0 between loop init and return 0 (epilogue sharing)
int Game::M_0040d850(char *first, char *second)
{
    char key;
    int count;
    int k;
    int i;
    char keys[12];

    M_00401630(0x20);
    M_00401400(0x20);
    strcpy(m_8c4->m_1494, first);
    strcpy(m_8c4->m_94, second);
    m_8c4->m_88 = strlen(m_8c4->m_1494) >> 1;
    M_00402e90();
    M_00402f10(0);
    count = 0;
    for (i = 0; i < 12; i++) {
        key = m_8c4->m_5094[i * KEY_LEN];
        if (key == '\0')
            continue;
        for (k = 0; k < count; k++) {
            if (key == keys[k])
                goto next;
        }
        keys[count++] = key;
    next:;
    }
    for (i = 0; i < count; i++) {
        key = keys[i];
        for (k = 0; k < 0x20; k++) {
            if (M_00401a40(k, key))
                return 1;
        }
    }
    return 0;
}

