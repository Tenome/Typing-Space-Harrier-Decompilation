#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <string.h>
#include "game.h"
int FUN_004119a0(StreamRec *stream, int isFile, int reading, int append, char *source, unsigned int size)
{
    int exists;

    stream->m_00 = isFile;
    stream->m_04 = reading;
    if (reading == 1) {
        if (isFile == 1) {
            FUN_00411930(source, &exists, (unsigned long *)&size);
            if (exists == 0)
                return 0;
            stream->m_08 = fopen(source, "rb");
            if (stream->m_08 == 0)
                return 0;
            stream->m_0c = 0;
        } else {
            stream->m_0c = (unsigned char *)source;
            stream->m_08 = 0;
        }
        stream->m_14 = 0;
        stream->m_10 = 0;
        stream->m_18 = size;
        stream->m_1c = size;
        return 1;
    }
    if (append == 1) {
        if (isFile == 1) {
            FUN_00411930(source, &exists, (unsigned long *)&size);
            if (exists == 1) {
                stream->m_08 = fopen(source, "r+b");
                if (stream->m_08 == 0)
                    return 0;
                stream->m_14 = 0;
                stream->m_10 = 0;
                stream->m_0c = 0;
                stream->m_18 = size;
                stream->m_1c = size;
                return 1;
            }
            stream->m_08 = fopen(source, "wb");
            if (stream->m_08 == 0)
                return 0;
            stream->m_14 = 0;
            stream->m_10 = 0;
            stream->m_18 = 0;
            stream->m_0c = 0;
            stream->m_1c = size;
            return 1;
        }
    } else if (isFile == 1) {
        stream->m_08 = fopen(source, "wb");
        if (stream->m_08 == 0)
            return 0;
        stream->m_0c = 0;
        goto finish;
    }
    stream->m_0c = (unsigned char *)source;
    stream->m_08 = 0;
finish:
    stream->m_14 = 0;
    stream->m_10 = 0;
    stream->m_18 = 0;
    stream->m_1c = size;
    return 1;
}
