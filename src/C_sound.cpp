#include <windows.h>
#include <dsound.h>
#include "game.h"

SoundMgr *SoundMgr::FUN_00411020()
{
    m_10 = 0;
    m_00 = 0;
    m_0c = 0;
    m_08 = 0;
    m_04 = -1;
    if (DirectSoundCreate(0, (LPDIRECTSOUND *)this, 0) != 0) {
        m_00 = 0;
        return this;
    }
    m_10 = 1;
    return this;
}

void SoundMgr::FUN_00411070()
{
    LPDIRECTSOUNDBUFFER *buffers;
    LPDIRECTSOUNDBUFFER buf;
    int i;

    if (m_00 != 0) {
        if (m_0c != 0)
            m_0c->Stop();
        if (m_08 != 0) {
            buffers = (LPDIRECTSOUNDBUFFER *)GlobalLock(m_08);
            for (i = 0; i < m_04; i++) {
                buf = *buffers;
                if (buf != 0) {
                    buf->Stop();
                    buf->Release();
                }
                *buffers = 0;
                buffers++;
            }
            GlobalUnlock(m_08);
            GlobalFree(m_08);
        }
        m_08 = 0;
        m_00->Release();
        m_00 = 0;
    }
}

int SoundMgr::FUN_00411100(void *hwnd, unsigned long level)
{
    return m_00->SetCooperativeLevel((HWND)hwnd, level) == 0;
}

int SoundMgr::FUN_00411120(int index, int loop)
{
    LPDIRECTSOUNDBUFFER *buffers;
    LPDIRECTSOUNDBUFFER *slot;
    LPDIRECTSOUNDBUFFER buf;
    int ok = 0;
    DWORD flags = 0;

    if (m_00 == 0 || m_08 == 0 || index < 0 || index >= m_04)
        return 0;
    if (loop != 0)
        flags = DSBPLAY_LOOPING;
    buffers = (LPDIRECTSOUNDBUFFER *)GlobalLock(m_08);
    slot = buffers + index;
    buf = *slot;
    if (buf != 0 && buf->Play(0, 0, flags) == 0)
        ok = 1;
    GlobalUnlock(m_08);
    return ok;
}

int SoundMgr::FUN_00411190(int index)
{
    LPDIRECTSOUNDBUFFER *buffers;
    LPDIRECTSOUNDBUFFER *slot;
    LPDIRECTSOUNDBUFFER buf;
    int ok = 0;

    if (m_00 == 0 || m_08 == 0 || index < 0 || index >= m_04)
        return 0;
    buffers = (LPDIRECTSOUNDBUFFER *)GlobalLock(m_08);
    slot = buffers + index;
    buf = *slot;
    if (buf != 0) {
        if (buf->Stop() == 0)
            ok = 1;
    }
    GlobalUnlock(m_08);
    return ok;
}

int SoundMgr::FUN_00411260(int index, int volume)
{
    LPDIRECTSOUNDBUFFER *buffers;
    LPDIRECTSOUNDBUFFER *slot;
    LPDIRECTSOUNDBUFFER buf;

    if (m_00 == 0 || m_08 == 0 || index < 0 || index >= m_04)
        return 0;
    if (volume > 0)
        volume = 0;
    else if (volume < -10000)
        volume = -10000;
    buffers = (LPDIRECTSOUNDBUFFER *)GlobalLock(m_08);
    slot = buffers + index;
    buf = *slot;
    if (buf != 0) {
        if (buf->SetVolume(volume) != 0)
            volume = 0;
    }
    GlobalUnlock(m_08);
    return volume;
}

SoundMgr *SoundMgr::FUN_004118f0()
{
    FUN_00411020();
    m_14 = 0;
    return this;
}
