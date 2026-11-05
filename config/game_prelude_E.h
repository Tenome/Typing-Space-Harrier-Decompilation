/* Lane E (0x4189a0-0x41eeb0) globals and free functions. */
struct PadState {
    char flags;
    char pad[3];
};
struct KeyIcon {
    int kind;
    int on;
};
struct KeyDef {
    int col;
    int row;
    unsigned char key;
    char name[3];
    int x;
    int y;
};
extern KeyDef DAT_004ec3d8[48];
extern PadState DAT_00a4b134[];
extern int  DAT_00f4dbf8;
extern int  DAT_00fb3140;
extern KeyIcon DAT_0101e350[4][13];
extern int  DAT_0101e574;
extern int  DAT_0102231c;
extern int  DAT_0101eac0;
extern int  DAT_01022310;
extern int  DAT_0101fc94;
extern int  DAT_01020618;
extern int  DAT_0101f410[4];
extern int  DAT_0101fdac;
extern int  DAT_0101fda4;
extern int  DAT_01121590[4];
extern int  DAT_0101e55c[4];
extern int  DAT_0101eac8;
extern int  DAT_0101eac4;
extern int  DAT_0101e5dc;
extern int  DAT_0101fc90;
extern int  DAT_0102061c;
extern int  DAT_0101fdb4;
extern int  DAT_0102230c;
extern int  DAT_01022314;
extern int  DAT_0101fdb8;
extern int  DAT_01022318;
extern int  DAT_0101e5e4;
extern int  DAT_01022308;

int  FUN_0041ad10(int on, int col, int row, unsigned char key, const char *name, int kind);
int  FUN_0041adc0(unsigned char key, const char *name, int *col, int *row);
void FUN_0041c860(int i, int v);
void FUN_0041c880(int v);
void FUN_0041c890(int v);
void FUN_0041c7b0(int v);
void FUN_0041ce10(int v);

/* 22-word draw record (0x58 bytes); m_08 links the next record, see FUN_00413a90 */
struct DrawObj {
    int m_00, m_04, m_08, m_0c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28;
    int m_2c, m_30, m_34, m_38, m_3c, m_40, m_44, m_48, m_4c, m_50, m_54;
};
struct IconPos {
    int x;
    int y;
};
extern int       DAT_0101f424[12];
extern int       DAT_01121560[12];
extern DrawObj   DAT_0101fdd8[12][2];
extern IconPos   DAT_004ec7bc[12];
extern int       DAT_0101ead4[12];
extern int       DAT_0101fc60[12];
extern int       DAT_0101ea08;
extern int       DAT_0101fda0;
extern int       DAT_01021800;
extern DrawObj   DAT_0101fc98;
extern int       DAT_011215a8;
extern int       DAT_0101f420;
extern int       DAT_0101fdb0;
extern int       DAT_0101e5e0;
extern int       DAT_0101f454;
void FUN_0041ce30(int i, int on, int col, int row);
void FUN_0041cf60(int i, int v);
int  FUN_0041cf70(int i);
void FUN_0041cfd0(int i, int x, int y);
void FUN_0041d070(int v);
void FUN_0041d080(int x, int y, int alpha);
extern int          DAT_004ec798[4];
extern const unsigned char *DAT_004ec7a8[4];
extern const unsigned char *DAT_004ec7b8;
int  FUN_0041cb30(const unsigned char *text, int x, int y, int fade);
void FUN_0041cac0(int i, int a, int b);
void FUN_0041c7d0(int a, int b);
extern int  DAT_0101fdc0;
extern int  DAT_0101e554;
extern int  DAT_0101fdc4;
extern int  DAT_0101fdbc;
extern int  DAT_0101e5d0[3];
void FUN_0041caf0(int a, int b);
void FUN_0041cb10(int a, int b);
int  FUN_0041d950(int n);

extern int  DAT_0101eacc;
extern int  DAT_0101fdcc;
extern int  DAT_0101fdc8;
extern int  DAT_0101f304;
int  FUN_004188f0(int a);
int  FUN_004185a0(const char *path, int a, int b, int c, int d);
int  FUN_004189a0(int n);
void FUN_0041d770(int level);
struct NameRow {
    int m_00;
    int m_04;
    int len;
    int state;
};
extern NameRow DAT_0101e270[14];
extern char *DAT_004ec850[14];
extern int  DAT_0101f458;
extern int  DAT_0101e570;
extern int  DAT_011215a0;
extern char DAT_0101eb04[];
extern const char *DAT_004ec3cc[0x31];
struct LabelDef {
    const char *text;
    int m_04;
};
extern LabelDef DAT_004ec978[48];
struct TextHost {
    char m_pad[0x34];
    GlyphSheet *m_34;
};
extern TextHost *DAT_0101fdd0;
extern int DAT_011215ac;
extern int DAT_011215b0;
extern int DAT_011215b4;
extern unsigned int DAT_0101e56c;
extern DrawObj DAT_01022320[0x200];
extern SoundMgr DAT_00a460a0;
struct NameTiming {
    int wait0;
    int wait1;
    int wait2;
    int slow;
};
extern NameTiming DAT_004ec888[15];
extern const char *DAT_004ecaf8[10];
extern char DAT_004e2d68[];
extern char DAT_004ecf78[];
extern int  DAT_0101fda8;
void FUN_0041c7d0(int a, int b);
