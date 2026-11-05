#include <string.h>
#include "game.h"

int FUN_0041ae90(int col, int row, int *x, int *y)
{
    int i;
    int found = 0;

    for (i = 0; i < 48; i++) {
        if (col >= 0 && row >= 0 && DAT_004ec3d8[i].col == col && DAT_004ec3d8[i].row == row) {
            *x = DAT_004ec3d8[i].x;
            *y = DAT_004ec3d8[i].y;
            found = 1;
            break;
        }
    }
    return found;
}
