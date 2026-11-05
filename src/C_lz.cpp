#include "game.h"

int FUN_00412040(unsigned int limit, StreamRec *stream)
{
    if (stream == 0)
        return 0;
    stream->m_1c = limit;
    if (stream->m_18 > limit)
        stream->m_18 = limit;
    return 1;
}

void FUN_00412060()
{
    int r;

    FUN_00412080();
    r = FUN_004120e0();
    while (r != -1)
        r = FUN_004120e0();
}

void FUN_00412080()
{
    DAT_00a4a6c0 = 0;
    DAT_00a4a6c4 = 0;
    DAT_00a4a6c8 = 0;
    DAT_00a4a6cc = 0;
    DAT_00a4a6d0 = 0;
    DAT_00a4a6d4 = 0;
    DAT_00a4a6d8 = 0;
    DAT_004ebb14 = 1;
    DAT_004ebb18 = 1;
    DAT_00a4a6dc = 0;
    DAT_004ebb1c = 1;
    DAT_00a4a6e0 = 0;
    DAT_00a4a6e4 = 0;
    DAT_00a4a6e8 = 0;
}

int FUN_00412190()
{
    return FUN_004121a0(&DAT_00a4a680);
}

int FUN_004122a0()
{
    int value = 0;
    int i;
    int bit;

    for (i = 0; i < 8; i++) {
        bit = FUN_004121a0(&DAT_00a4a680);
        if (bit == -1)
            return -1;
        value = bit + value * 2;
    }
    return value;
}

int FUN_004122d0(int value)
{
    int room;

    DAT_00a4627c[DAT_00a4a6c8] = value;
    DAT_00a4a6c8 = (DAT_00a4a6c8 + 1) % 0x880;
    room = DAT_00a4a6c4 - DAT_00a4a6c8;
    if (room < 0)
        room += 0x880;
    return room;
}

int FUN_00412300(int offset)
{
    int result = -1;
    int slot = (DAT_00a4a6c4 + offset) % 0x880;
    int used = (DAT_00a4a6c8 - DAT_00a4a6c4 + 0x880) % 0x880;

    if (used == 0)
        used = 0x880;
    if (offset < used)
        result = DAT_00a4627c[slot];
    return result;
}

int FUN_00412350(int count)
{
    int i;
    int over;

    for (i = 0; i < count; i++)
        FUN_004123b0(FUN_00412300(DAT_00a4a6d4 + i));
    DAT_00a4a6d4 += count;
    over = DAT_00a4a6d4 - 0x800;
    if (0 < over) {
        DAT_00a4a6d4 = 0x800;
        FUN_004123d0(over);
    }
    return over;
}

void FUN_004123b0(int value)
{
    FUN_00411db0(value, &DAT_00a4a6a0);
}

void FUN_004123d0(int count)
{
    DAT_00a4a6c4 = (count + DAT_00a4a6c4) % 0x880;
}

int FUN_004123f0()
{
    return DAT_00a4a6d4;
}
