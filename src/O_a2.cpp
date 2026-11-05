#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

int Game::M_00401da0(int n)
{
    int a;
    int b;
    int result;

    FUN_00401d70(n, &a, &b);
    result = 0;
    if (a == 0 && b == 0)
        result = 1;
    return result;
}
