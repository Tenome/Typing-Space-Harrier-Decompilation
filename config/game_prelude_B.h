/* lane B: records and globals used by the 0x406ef0-0x40cc70 code */
/* struct Player moved to game_prelude_00.h (shared, needed by lane A) */

extern Player *DAT_00a232a0;
extern int DAT_004bd608;
extern int DAT_004e2110[];
extern int DAT_004e20fc[];
extern int DAT_00a460bc;
extern int DAT_00a46264[4];

struct Obj_009a629c {
    int m_0;
    int m_4;
    int m_8;
    int m_c;
    int m_10;
    int m_14;
    int m_18;
    int m_1c;
    int m_20;
    int m_24;
    int m_28;
    int m_2c;
    int m_30;
    int m_34;
    int m_38;
    int m_3c;
};

extern Obj_009a629c DAT_009a629c[];
extern Obj_009a629c *DAT_00a3d6ec;
extern int DAT_004e1fdc[];
extern int DAT_004e2314[4];
extern int DAT_00a4625c;
extern int DAT_00a45e28;

struct Stack_009a5e98 {
    int m_0;
    int m_4[4];
};
extern Stack_009a5e98 DAT_009a5e98;

extern int DAT_004f065c[16];
extern int DAT_00430840[];
extern int DAT_004efbd4;

struct Rec_87cbc8 {
    int m_0;
    int m_4;
    int m_8;
    int m_c;
    int m_10;
    int m_14;
    int m_18;
    int m_1c;
    int m_20;
    int m_24;
    int m_28;
    int m_2c;
    int m_30;
    int m_34;
    int m_38;
    int m_3c;
    int m_40;
    int m_44;
    int m_48;
    int m_4c;
    int m_50;
    int m_54;
    int m_58;
    int m_5c;
    int m_60;
    int m_64;
    int m_68;
    int m_6c;
    int m_70;
    int m_74;
    int m_78;
    int m_7c;
    int m_80;
    int m_84;
    int m_88;
    int m_8c;
};

extern int DAT_0099cfd0;
extern int DAT_008f0ff4;
extern int DAT_004efa14;
extern int DAT_00a3d704;

struct StageRow {
    int m_0;
    int m_4;
    int m_8;
    int m_c;
    int m_10;
    int m_14;
    int m_18;
    int m_1c;
    int m_20;
    int m_24;
    int m_28;
    int m_2c;
    int m_30;
    int m_34;
    int m_38;
    int m_3c;
    int m_40;
    int m_44;
    int m_48;
    int m_4c;
    int m_50;
    int m_54;
};
extern StageRow DAT_004bddf8[];
extern int DAT_004efbd8[];

extern int DAT_004e2328[];
extern int DAT_00a4604c;
int FUN_004185a0(const char *path, int flag);
extern int DAT_008f069c[0x255];

struct Timer {
    int m_0;
    int m_4;
    int m_8;
    int m_c;
    int m_10;
};
extern Timer DAT_00a3ce58[0x40];
extern Timer *DAT_004efa18;
extern int DAT_004f0550;
extern int DAT_004f0554[];
void FUN_00418f70(int n);
extern int DAT_004e26a8[];
