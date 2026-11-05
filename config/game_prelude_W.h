extern int DAT_004e25fc[8];

struct KeyImageIds {
    int main;
    int shadow;
    int overlay;
    int optional;
};

struct KeyPartIds {
    int id[8];
    int count;
};

struct KeyPartRamp {
    int a;
    int b;
    int c;
};

extern char          DAT_00fb9e48[0x64000];
extern char          DAT_00f4dd70[0x64000];
extern char          DAT_00a4c324[];
extern int           DAT_004ebe88;
extern int           DAT_00a4c294[18];
extern int           DAT_00a4c2dc[18];
extern int           DAT_00f4dce0[18];
extern int           DAT_00f4dd28[18];
extern int           DAT_00f4dc08[];
extern KeyImageIds   DAT_004ebd68[18];
extern KeyPartIds    DAT_004ebe90[18];
extern int           DAT_004ec118[18];
extern KeyPartRamp   DAT_004ec310[];
