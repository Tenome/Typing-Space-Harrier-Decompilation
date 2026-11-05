/* Lane I (0x416200-0x4193c0): clipped read-back / blit helpers of FUN_00415320 and the loaders after them. */
struct ReadbackJob {
    unsigned int   flags;
    unsigned char *base;
    int            baseW;
    int            baseH;
    int            basePitch;
    unsigned char *buf;
    int            width;
    int            height;
    int            bufPitch;
    int            x;
    int            y;
    int            m_2c;
    int            m_30;
    int            m_34;
    int            m_38;
    int           *keys;
};
void FUN_00416200(ReadbackJob *job);
void FUN_00416350(ReadbackJob *job);
void FUN_00416440(ReadbackJob *job);

/* Stage setup records of FUN_00418f70 */
struct BigBuf {
    int v[0x200];
};
struct ImgRec {
    int     m_0;
    int     m_4;
    int     m_8;
    BigBuf *data;
    int     m_10;
};
struct StageRec {
    int     m_0;
    int     layered;
    ImgRec *first;
    ImgRec *fourth;
    ImgRec *fifth;
    ImgRec *third;
    ImgRec *optional;
    int     m_1c[2];
    ImgRec *extra[32];
    int     extraCount;
};
struct LayerNode {
    unsigned int flags;
    LayerNode   *m_4;
    LayerNode   *m_8;
    int          m_c[4];
    int          m_1c;
    int          m_20;
    int          m_24[10];
    void        *m_4c;
    int          m_50;
    int          m_54;
    int          m_58[4];
    ImgRec      *img;
    int          m_6c;
};
struct SlotNode {
    unsigned int flags;
    SlotNode    *prev;
    SlotNode    *next;
    int          m_c[4];
    int          m_1c;
    int          m_20;
    int          m_24[5];
    int          m_38;
    int          m_3c;
    int          m_40[4];
    ImgRec      *img;
    int          m_54;
};
/* One object per memory region the stage setup of FUN_00418f70 writes. */
struct StageData {
    StageRec rec;
    ImgRec   sceneA;
    ImgRec   sceneB;
    BigBuf   bufA;
    BigBuf   bufB;
    BigBuf   bufC;
    int      m_dbf8;
    int      clear1;
    int      clear2;
    int      clear3;
    int      m_dc08[14];
    ImgRec   slotImg[32];
};
struct StageTable {
    StageRec rec[18];
    BigBuf   bufA;
    int      m_3140;
    int      m_3144;
    BigBuf   slotBuf[32];
    BigBuf   bufB;
};
struct OverlayData {
    LayerNode node;
    ImgRec    img[2];
    int       m_200[2];
    BigBuf    buf[2];
    int       count;
    int       m_c;
    LayerNode node2;
    ImgRec    img2;
};
struct LayerData {
    LayerNode node[3];
    ImgRec    img;
    int       m_pad;
    SlotNode  slot[32];
};
extern StageTable  DAT_00fb1d70;
extern StageData   DAT_00f4c328;
extern OverlayData DAT_00a4b168;
extern LayerData   DAT_0101de48;
extern BigBuf      DAT_00fb87c8;
extern int         DAT_00f4c324;
void FUN_00418f70(int stage);
