#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"

/* Functions that are written but do not match yet. Each carries the diagnosed residue. */

int Game::M_00410c20()
{
    BgmFile *file = DAT_004bdd70;
    const char *name = file->name;
    int ok = 1;

    if (*name != 0) {
        do {
            if (file->size == 0)
                break;
            if (M_00410ca0((char *)name, file->size) == DAT_00430658) {
                ok = 0;
                break;
            }
            name = file[1].name;
            file++;
        } while (*name != 0);
    }
    m_8 = 0;
    if (!ok) {
        M_00404a90();
        m_8 = 1;
        M_00404d80();
        m_a795c = 0xc;
    }
    return m_8;
}
