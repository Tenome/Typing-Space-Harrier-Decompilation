/* Lane Q */
extern int DAT_004e261c[24];

extern int    DAT_0042e32c[];
extern char  *DAT_0042e12c[0x80];
extern char  *DAT_0042e124;
extern char  *DAT_0042e11c;
extern char  *DAT_0042e52c[];
extern int    DAT_004f0654;
extern char  *DAT_004f0658;
extern char  *DAT_004efa38;
extern char   DAT_009aa29c[];
int *FUN_004129a0(char *bitmap, char *dest, char **limit);
extern int DAT_004f0534[7];
extern int DAT_004efa1c[7];
extern int DAT_00a46048;

struct ScaleRow {
    int lineShift;
    int rowShift;
    int divisor;
};
struct ScaledBlitJob {
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
    int            m_28;
    int            m_2c;
    int            m_30;
    int            m_34;
    ScaleRow      *table;
    int            m_3c;
};
void FUN_00417b80(ScaledBlitJob *job);
