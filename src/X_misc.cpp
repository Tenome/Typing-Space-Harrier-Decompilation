#include "game.h"

extern "C" unsigned char blendHalf(unsigned int src, unsigned int dst, unsigned int arg)
{
    unsigned char index = (unsigned char)dst;

    if (index >= 0xfb && index <= 0xfe)
        return index - 4;
    return index;
}
