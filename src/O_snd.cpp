#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dsound.h>
#include "game.h"
#include "game.h"
int SoundMgr::FUN_004111f0(int index, int freq)
{
    LPDIRECTSOUNDBUFFER *buffers;
    LPDIRECTSOUNDBUFFER *slot;
    LPDIRECTSOUNDBUFFER buf;

    if (m_00 == 0 || m_08 == 0 || index < 0 || index >= m_04)
        return 0;
    buffers = (LPDIRECTSOUNDBUFFER *)GlobalLock(m_08);
    slot = buffers + index;
    buf = *slot;
    if (buf != 0 && buf->SetFrequency(freq) != 0)
        freq = 0;
    GlobalUnlock(m_08);
    return freq;
}
