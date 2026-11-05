/* lane C: records and globals used by the 0x40cdd0-0x4129a0 code */
struct GroupRec {
    int m_00;
    int m_04;
    int m_08;
    int m_0c;
    int m_10;
    int m_14;
    int m_18;
    int m_1c;
    int m_20;
    int m_24;
    int m_28;
    int m_2c[0x21];
    int m_b0;
    int m_b4[0x80];
};

struct BankRec {
    int   m_00[3];
    int   m_0c;
    char **m_10;
    char **m_14;
    char  m_18[0x1004];
};

extern GroupRec  DAT_008f5450[72];
extern GroupRec *DAT_004f052c;
extern int       DAT_00a232a4;
extern BankRec   DAT_00901708[];
extern BankRec  *DAT_00a45e34;

int FUN_0040cf20();
void __stdcall FUN_0040d130(int bank);

struct PlayerDef {
    int m_00;
    int m_04;
    int m_08;
    int m_0c;
    char pad10[0x48];
};

extern int       DAT_004bde14[];
extern int       DAT_008f069c[];

struct LayoutItem {
    int m_00;
    int m_04;
    int m_08;
    int m_0c;
    int m_10;
    int m_14;
    int m_18;
    int m_1c;
    int m_20;
};

extern LayoutItem  DAT_00a3d488[17];
extern LayoutItem *DAT_008f1014;
extern LayoutItem *DAT_00a3d358;
extern LayoutItem *DAT_0099cfd4;
extern int         DAT_008f0ffc;
extern int         DAT_008f1008;
void FUN_004189d0(char *path);

struct Rec418 {
    int m_00[0x106];
};

extern Rec418  DAT_0099d780[];
extern Rec418 *DAT_0099d6bc;
extern int     DAT_00a3d6f0;
extern int     DAT_008f1004;
extern int     DAT_00a45e28;
extern int     DAT_0099d5bc[0x40];
extern int     DAT_004f0530;

struct TypedRec {
    int  m_00;
    int  m_04;
    int  m_08;
    int  m_0c;
    char m_10[0x40];
    char m_50[0x40];
    int  m_90;
    int  m_94;
    int  m_98;
    int  m_9c;
    int  m_a0;
    int  m_a4;
    int  m_a8;
    int  m_ac;
};
extern TypedRec DAT_0099d6c0;

extern TypedRec *DAT_008f544c;
extern int       DAT_00a3d360[0x41];
extern int      *DAT_004efa10;
extern int DAT_004e28dc[];
extern int DAT_00a461cc;

struct BgmFile {
    int         size;
    const char *name;
};

extern BgmFile DAT_004bdd70[];
extern char    DAT_004e2970[];

struct StreamRec {
    int            m_00;
    int            m_04;
    void          *m_08;
    unsigned char *m_0c;
    int            m_10;
    unsigned int   m_14;
    unsigned int   m_18;
    unsigned int   m_1c;
};

int FUN_00411910();
void FUN_00411920();
void FUN_00411930(const char *path, int *exists, unsigned long *size);
void FUN_00411b20(StreamRec *stream);
int FUN_00411cd0(StreamRec *stream);
int FUN_00411ce0(unsigned char *dest, int count, StreamRec *stream);
unsigned int FUN_00411d20(StreamRec *stream);
int FUN_00411da0(StreamRec *stream);

struct IDirectSound;
struct IDirectSoundBuffer;

/* DirectSound front end: the IDirectSound object plus a GlobalAlloc'd table of sound buffers. */
struct SoundMgr {
    IDirectSound        *m_00;
    int                  m_04;
    void                *m_08;
    IDirectSoundBuffer  *m_0c;
    int                  m_10;
    void                *m_14;

    SoundMgr *FUN_00411020();
    void FUN_00411070();
    int  FUN_00411100(void *hwnd, unsigned long level);
    int  FUN_00411120(int index, int loop);
    int  FUN_00411190(int index);
    int  FUN_004111f0(int index, int freq);
    int  FUN_00411260(int index, int volume);
    int  FUN_00411340(int count);
    SoundMgr *FUN_004118f0();
};


struct Rec14 {
    int m_00;
    int m_04;
    int m_08;
    int m_0c;
    int m_10;
};

extern int    *DAT_008f1010;
extern int     DAT_004ef908[];
extern int     DAT_0099d774;
extern Rec14  *DAT_00a0a29c;
extern Rec14   DAT_004e0ef8[];
extern int *DAT_00a2330c;

/* LZSS-style decoder state (0x412040-0x4123f0) */
extern StreamRec DAT_00a4a680;
extern StreamRec DAT_00a4a6a0;
extern int       DAT_00a4a6c0;
extern int       DAT_00a4a6c4;
extern int       DAT_00a4a6c8;
extern int       DAT_00a4a6cc;
extern int       DAT_00a4a6d0;
extern int       DAT_00a4a6d4;
extern int       DAT_00a4a6d8;
extern int       DAT_00a4a6dc;
extern unsigned int DAT_00a4a6e0;
extern int       DAT_00a4a6e4;
extern int       DAT_00a4a6e8;
extern int       DAT_004ebb14;
extern int       DAT_004ebb18;
extern int       DAT_004ebb1c;
extern int       DAT_00a4627c[0x880];
int FUN_00412040(unsigned int limit, StreamRec *stream);
void FUN_00412060();
void FUN_00412080();
int FUN_004120e0();
int FUN_00412190();
int FUN_004121a0(StreamRec *stream);
int FUN_004121f0(int bits);
int FUN_00412250();
int FUN_004122a0();
int FUN_004122d0(int value);
int FUN_00412300(int offset);
int FUN_00412350(int count);
void FUN_004123b0(int value);
void FUN_004123d0(int count);
int FUN_004123f0();
unsigned int FUN_00411db0(unsigned int c, StreamRec *stream);
extern int DAT_004efbd8[];
extern int DAT_004bde00[];
extern int DAT_004f021c;
