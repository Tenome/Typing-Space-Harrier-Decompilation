#include <stdio.h>
#include <windows.h>
#include "game.h"

int FUN_00411910()
{
    return 1;
}

void FUN_00411920()
{
}

void FUN_00411930(const char *path, int *exists, unsigned long *size)
{
    HANDLE file = CreateFileA(path, 0, 1, 0, 3, 0x80, 0);

    if (file == INVALID_HANDLE_VALUE) {
        *exists = 0;
        *size = 0;
    } else {
        *exists = 1;
        *size = GetFileSize(file, 0);
    }
    CloseHandle(file);
}

void FUN_00411b20(StreamRec *stream)
{
    if (stream != 0 && stream->m_00 == 1 && stream->m_08 != 0) {
        fclose((FILE *)stream->m_08);
        stream->m_08 = 0;
    }
}

int FUN_00411cd0(StreamRec *stream)
{
    if (stream == 0)
        return 0;
    return stream->m_14;
}

int FUN_00411ce0(unsigned char *dest, int count, StreamRec *stream)
{
    int done;
    int c;

    for (done = 0; done < count; done++) {
        c = FUN_00411d20(stream);
        if (c == -1)
            return done;
        if (dest != 0) {
            *dest = (unsigned char)c;
            dest++;
        }
    }
    return done;
}

unsigned int FUN_00411d20(StreamRec *stream)
{
    unsigned int c;
    unsigned int pos;

    if (stream == 0)
        return 0xffffffff;
    if (stream->m_00 == 1) {
        if (stream->m_14 >= stream->m_1c)
            return 0xffffffff;
        c = getc((FILE *)stream->m_08);
        if (c != 0xffffffff) {
            stream->m_14++;
            return c;
        }
    } else {
        pos = stream->m_14;
        if (pos >= stream->m_1c)
            return 0xffffffff;
        c = stream->m_0c[pos];
        stream->m_14 = pos + 1;
    }
    return c;
}

int FUN_00411da0(StreamRec *stream)
{
    if (stream == 0)
        return 0;
    return stream->m_18;
}
