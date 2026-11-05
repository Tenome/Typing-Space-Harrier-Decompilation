#include "game.h"

/* Runs every request of the list; 0 if any of them failed. */
int FUN_00413a90(BlitRequest *list)
{
    int ok;

    if (DAT_00a4b0a8 == 0)
        return 0;
    ok = 1;
    for (; list != 0; list = list->next) {
        if (FUN_00413ad0(list) == 0)
            ok = 0;
    }
    return ok;
}

int FUN_004152e0(BlitRequest *list)
{
    int ok;

    if (DAT_00a4b0a8 == 0)
        return 0;
    ok = 1;
    for (; list != 0; list = list->next) {
        if (FUN_00415320(list) == 0)
            ok = 0;
    }
    return ok;
}

/* Copies the in-use entries of src over dst; flags a palette change when dst is the shadow palette. */
void FUN_004178f0()
{
}

void FUN_00417ee0(int value)
{
    DAT_00a4a878 = value;
}
