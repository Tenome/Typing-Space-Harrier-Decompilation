/* Lane U */
extern short DAT_004e24bc[];
extern short DAT_008f1418[];

struct CamEffect {
    int active;
    int kind;
    int m_8;
    int start;
    int m_10;
    int m_14;
    int m_18;
};

struct CamState {
    int       now;
    int       pos;
    int       len;
    int       percent;
    int       pos2;
    int       len2;
    int       percent2;
    int       m_1c;
    int       m_20;
    int       m_24;
    CamEffect eff[8];
};

struct FrameParams {
    int y;
    int m_4;
    int m_8;
    int a;
    int b;
    int m_14;
    int m_18;
    int m_1c;
};

extern CamState    *G_008f1010;
extern CamEffect   *G_00a3ce50;
extern FrameParams  G_00a3d468;
void FUN_00419310(int n);
void FUN_00419350(int n);
int  FUN_004193c0(FrameParams *params);
