/* Lane D (0x412b90-0x4188f0): DirectDraw glue, glyph sheets and the software blitter. */
struct IDirectDraw;
struct IDirectDrawSurface;
struct IDirectDrawPalette;
struct HDC__;
struct HFONT__;
struct tagLOGFONTA;

struct PalEntry {
    unsigned char r, g, b, flags;
};

/* The 256-entry palette shadow: colours, then one "in use" flag per entry (0x800 bytes in all). */
struct PaletteTable {
    PalEntry entry[256];
    int      used[256];
};

/* Header in front of an image stored in a caller-supplied buffer. */
struct SheetImage {
    int           width;
    int           height;
    unsigned int  bpp;
    PaletteTable *palette;
    unsigned char *pixels;
};

extern IDirectDraw        *DAT_00a4a6fc;
extern int                 DAT_00a4a6f0;
extern PaletteTable        DAT_00a4a890;

extern IDirectDrawSurface *DAT_00a4a830;
extern SheetImage         *DAT_00a4a834;
extern PaletteTable       *DAT_00a4a838;
extern unsigned char      *DAT_00a4a83c;
extern unsigned long       DAT_00a4a840;
extern unsigned long       DAT_00a4a844;
extern unsigned long       DAT_00a4a848;
extern unsigned long       DAT_00a4a84c;
extern unsigned long       DAT_00a4a850;
extern unsigned long       DAT_00a4a854;
extern HDC__               *DAT_00a4a858;
extern HFONT__             *DAT_00a4a85c;

int           FUN_00412b90(int width, int height, int charExtra, tagLOGFONTA *font, unsigned long fg, unsigned long bg, int prefer, int fillBg, void *buf, unsigned long *limit);
int           FUN_00412e80(const char *text, int x, int y);
SheetImage    *FUN_00412ec0();
void          FUN_00412fe0(unsigned long color1, unsigned long color2, int prefer, unsigned long *rgb1, unsigned long *rgb2, int *index1, int *index2);
int           FUN_00413280(IDirectDrawSurface *surf, unsigned char value);
int           FUN_00413350(IDirectDrawSurface *surf, unsigned char *dst, unsigned char fgIndex, unsigned char bgIndex, unsigned int fgPixel, unsigned int bgPixel);
unsigned long FUN_00412880(IDirectDrawSurface *surf, unsigned long color);

/* Glyph sheet header (0x20 bytes) followed by its SheetImage. */
struct GlyphSheet {
    int width;
    int height;
    int halfCellW;
    int cellW;
    int cellH;
    int singleCount;
    int doubleCount;
    SheetImage *image;
};

extern unsigned char  DAT_004ebb2b[];
extern unsigned char  DAT_004ebb2c[];
extern unsigned short DAT_004ebb32[];
extern unsigned short DAT_004ebb34[];
extern IDirectDrawPalette *DAT_00a4a87c;
extern int            DAT_00a4a88c;

int FUN_00413610(const unsigned char *ch, GlyphSheet *sheet, int *x, int *y, int *w, int *h);
int FUN_00413670(int index, GlyphSheet *sheet, int *x, int *y, int *w, int *h);
int FUN_004136f0(const unsigned char *ch);
int FUN_00413870(const unsigned char *ch);
int FUN_004139f0(PaletteTable *out);
int FUN_00413a20(const PaletteTable *in);
int FUN_00413a50();

/* Blit request node (linked list through next); the full layout is filled in as the blitters are matched. */
struct BlitRequest {
    unsigned int flags;
    int          m_4;
    BlitRequest *next;
};

extern int DAT_00a4b0a8;
extern int DAT_00a4a878;

int  FUN_00413a90(BlitRequest *list);
int  FUN_00413ad0(BlitRequest *request);
int  FUN_004152e0(BlitRequest *list);
int  FUN_00415320(BlitRequest *request);
void FUN_004178f0();
void FUN_00417ee0(int value);
void FUN_004147c0(PaletteTable *dst, const PaletteTable *src);

struct HWND__;
extern HWND__ *DAT_00a4b0b0;
int FUN_004188f0(int channel);

/* MCI background music: per-channel "open" flag (bit 0) and the CD-audio flag. */
extern "C" int mciChannelOpen[10];
extern "C" int mciCdOpen;
struct HINSTANCE__;
int  FUN_00417ef0(HINSTANCE__ *instance, HWND__ *parent);
void FUN_00417fd0();
extern int DAT_00a4b0e4[10];
extern int DAT_00a4b10c[10];
int FUN_004185a0(char *file, int channel, int from, int to, int repeat);

/* Parameters of one software blit, built on the stack by FUN_00413ad0. */
struct BlitJob {
    unsigned int   flags;
    unsigned char *src;
    unsigned char *dst;
    unsigned int   width;
    unsigned int   height;
    unsigned int   dstWidth;
    unsigned int   m_18;
    unsigned int   srcPitch;
    unsigned int   dstPitch;
    int           *keyTable;
    unsigned int   blendArg;
    unsigned char (*blend)(unsigned int src, unsigned int dst, unsigned int arg);
};
void FUN_00414830(BlitJob *job);
void FUN_004149d0(BlitJob *job);
void FUN_00414bd0(BlitJob *job);
void FUN_00414f00(BlitJob *job);

/* Job of the surface read-back / clip helpers called by FUN_00415320. */
struct ClipJob {
    int            m_0;
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
};
void FUN_00416100(ClipJob *job);

int FUN_00413780(int index, char *out, GlyphSheet *sheet);
int FUN_00413900(int index, char *out, GlyphSheet *sheet);
GlyphSheet *FUN_00413430(unsigned int width, unsigned int height, tagLOGFONTA *font, unsigned long fg, unsigned long bg, int prefer, int fillBg, void *buf, unsigned long *limit);
extern int          DAT_004ebb30;
extern int          DAT_004ebbc4;
extern int          DAT_00a4a828;
extern int          DAT_00a4b09c;
extern int          DAT_00a4a868[4];
extern int          DAT_00a4a700[0x48];
extern unsigned char DAT_004ebb2d[];
extern int DAT_00a4b15c;
extern int DAT_00a4b160;
