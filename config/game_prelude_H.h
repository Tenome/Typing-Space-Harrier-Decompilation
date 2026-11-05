/* Lane H: intrusive list classes (0x410e30-0x410f80) and shared blit types. */
class NodeList;

class ListNode {
public:
    ListNode *prev;
    ListNode *next;
    NodeList *owner;
    unsigned char flags;

    ListNode();
    virtual ~ListNode();
    virtual void M_00410e90();
};

class GroupTable {
public:
    ListNode *slot[5];

    void Clear()
    {
        for (int i = 0; i < 5; i++)
            slot[i] = 0;
    }
};

class NodeList : public ListNode {
public:
    ListNode *head;
    GroupTable group;
    int m_2c;

    NodeList();
    virtual ~NodeList();
    virtual void M_00410ff0();
    void FUN_00410f40();
    void FUN_00410f80(ListNode *node);
};

struct IDirectDrawSurface;
extern IDirectDrawSurface *DAT_00a4b094;
extern IDirectDrawSurface *DAT_00a4a824;
extern int (*PaletteChangeHook)();
int FUN_004127d0();
void FUN_00412730();
extern "C" int DisplayReady();

/* 20-byte header in front of a packed stream; "tag" is compared with DAT_004ebb24. */
struct StreamHeader {
    int  offset;
    int  length;
    int  m_08;
    int  hasPayload;
    char tag[4];
};
extern char DAT_004ebb24[];
unsigned int FUN_00411bf0(StreamRec *stream, unsigned int offset, int absolute);
int FUN_00411b50(StreamRec *stream);
int FUN_004119a0(StreamRec *stream, int isFile, int reading, int append, char *source, unsigned int size);
int FUN_00411e80(unsigned int flags, char *source, unsigned int size, char *dest, unsigned int *destSize, int skip);

extern int                 DAT_00a4a6f8;
extern int                 DAT_00a4b090;
extern int                 DAT_00a4a6f4;
extern int                 DAT_00a4b0a4;
extern IDirectDrawSurface *DAT_00a4a864;
extern unsigned int        DAT_00a4a860;
extern int                 DAT_00a4a820;
extern unsigned int        DAT_00a4b0a0;
extern unsigned long       DAT_00a4a888;
extern unsigned long       DAT_00a4b098;
extern unsigned long       DAT_00a4a880;
extern unsigned long       DAT_00a4a884;
int FUN_00412400(void *window, int width, int height, int bpp, int coopFlags, unsigned int offWidth, unsigned int offHeight, unsigned long keyHigh, unsigned long keyLow);
SheetImage *FUN_004129a0(const unsigned char *file, unsigned int bufferStart, unsigned int *limit);
extern char *DAT_004e295c[];

/* SoundMgr with the WAV loader (0x4114d0) that fills one slot of the buffer and loop-point tables. */
struct SoundLoader : SoundMgr {
    int M_004114d0(const char *path, int index);
};
