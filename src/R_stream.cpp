#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

unsigned int FUN_00411db0(unsigned int c, StreamRec *stream)
{
    unsigned int r;

    if (stream != 0 && stream->m_04 != 1) {
        if (stream->m_00 == 1) {
            r = putc((unsigned char)c, (FILE *)stream->m_08);
            if (r != c)
                return r;
            stream->m_14++;
            if (stream->m_14 > stream->m_18)
                stream->m_18 = stream->m_14;
            if (stream->m_18 > stream->m_1c)
                stream->m_1c = stream->m_18;
            return r;
        }
        if (stream->m_14 < stream->m_1c && (int)c >= 0 && (int)c < 0x100) {
            stream->m_0c[stream->m_14] = (unsigned char)c;
            stream->m_14++;
            if (stream->m_14 > stream->m_18)
                stream->m_18 = stream->m_14;
            return c;
        }
    }
    return 0xffffffff;
}
