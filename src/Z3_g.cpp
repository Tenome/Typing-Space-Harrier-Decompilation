#include <string.h>
#include "game.h"

int FUN_0041adc0(unsigned char key, const char *name, int *col, int *row)
{
    int i;
    const KeyDef *e;

    for (i = 0, e = DAT_004ec3d8; i < 48; i++, e++) {
        if (key != 0 && e->key != 0 && e->key == key) {
            *col = e->col;
            *row = e->row;
            return 1;
        }
        if (name != 0 && e->name != 0 && strcmp(e->name, name) == 0) {
            *col = e->col;
            *row = e->row;
            return 1;
        }
    }
    return 0;
}
