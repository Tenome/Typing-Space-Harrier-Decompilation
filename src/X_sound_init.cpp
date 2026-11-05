#include <windows.h>
#include <dsound.h>

struct SoundMgr {
    IDirectSound        *m_00;
    int                  m_04;
    void                *m_08;
    IDirectSoundBuffer  *m_0c;
    int                  m_10;
    void                *m_14;

    SoundMgr();
    ~SoundMgr();
    SoundMgr *FUN_00411020();
    void FUN_00411070();
};

class NodeList {
public:
    NodeList();
    virtual ~NodeList();
};

SoundMgr DAT_00a460a0;
NodeList DAT_00a46198;

SoundMgr::SoundMgr()
{
    FUN_00411020();
    m_14 = 0;
}

SoundMgr::~SoundMgr()
{
    HGLOBAL *entries;
    int i;

    if (m_14 != 0) {
        entries = (HGLOBAL *)GlobalLock(m_14);
        for (i = 0; i < m_04; i++) {
            if (*entries != 0)
                GlobalFree(*entries);
            *entries = 0;
            entries++;
        }
        GlobalUnlock(m_14);
        GlobalFree(m_14);
    }
    m_14 = 0;
    FUN_00411070();
}
