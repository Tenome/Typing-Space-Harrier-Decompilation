#include <stdio.h>
#include <string.h>
#include "game.h"

int FUN_00411b50(StreamRec *stream)
{
    StreamHeader header;
    int start = FUN_00411cd0(stream);
    int end;

    if (FUN_00411ce0((unsigned char *)&header, sizeof(header), stream) != sizeof(header))
        return 0;
    if (FUN_00411bf0(stream, start, 1) != start)
        return 0;
    if (strncmp(header.tag, DAT_004ebb24, 4) != 0)
        return 0;
    if (header.hasPayload == 0)
        return 0;
    end = header.offset + header.length;
    return FUN_00411bf0(stream, end, 0) == end;
}

unsigned int FUN_00411bf0(StreamRec *stream, unsigned int offset, int absolute)
{
    unsigned int moved = offset;
    unsigned int limit;

    if (stream == 0)
        return 0;
    if (stream->m_00 == 1) {
        if (stream->m_08 == 0)
            return 0;
        if (absolute == 1) {
            if (fseek((FILE *)stream->m_08, offset, SEEK_SET) != 0)
                return 0;
            stream->m_14 = offset;
        } else {
            if (fseek((FILE *)stream->m_08, offset, SEEK_CUR) != 0)
                return 0;
            stream->m_14 += offset;
        }
        if (stream->m_14 > stream->m_18)
            stream->m_18 = stream->m_14;
        if (stream->m_18 > stream->m_1c)
            stream->m_1c = stream->m_18;
    } else {
        if (stream->m_0c == 0)
            return 0;
        if (absolute == 1) {
            stream->m_14 = offset;
            limit = stream->m_1c;
            if (offset >= limit) {
                moved = limit;
                stream->m_14 = limit;
            }
        } else {
            stream->m_14 += offset;
            limit = stream->m_1c;
            if (stream->m_14 >= limit) {
                moved = limit - stream->m_14 + offset;
                stream->m_14 = limit;
            }
        }
        if (stream->m_14 >= stream->m_18)
            stream->m_18 = stream->m_14;
    }
    return moved;
}
