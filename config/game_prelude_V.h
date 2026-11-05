/* Lane V: distortion blits (rows with per-row shift and horizontal scale). */
struct DistortRow {
    int lineShift;
    int rowShift;
    int divisor;
};
struct DistortJob {
    unsigned int   flags;
    unsigned char *base;
    int            baseW;
    int            baseH;
    int            basePitch;
    unsigned char *buf;
    int            width;
    int            rows;
    int            bufPitch;
    int            x;
    int            y;
    int            m_2c;
    int            m_30;
    int            m_34;
    DistortRow    *table;
    int           *keys;
};
void FUN_00417530(DistortJob *job);
void FUN_004176f0(DistortJob *job);

/* Full layout of the request FUN_00413ad0 reads (BlitRequest only names the list link). */
struct BlitCommand : BlitRequest {
    int            srcX;
    int            srcY;
    int            srcW;
    int            srcH;
    int            dstX;
    int            dstY;
    int            dstW;
    int            dstH;
    int            angle;
    unsigned int   blendArg;
    unsigned char (*blend)(unsigned int src, unsigned int dst, unsigned int arg);
    unsigned long  keyLow;
    unsigned long  keyHigh;
    int            m_40[4];
    SheetImage    *srcImage;
    SheetImage    *dstImage;
};
int FUN_00413ad0(BlitCommand *request);
