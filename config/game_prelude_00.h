/* Typing Space Harrier: the one big game object (operator new(0x21efbf8), global DAT_011216c4).
   Members m_XXXX and methods M_XXXXXXXX are placeholders named after the original offset / address;
   rename through game_fields.tsv / game_methods.txt / symbols.tsv once the meaning is known. */

struct SeEntry {
    const char *name;
    int         id;
};

extern SeEntry  DAT_004306e8[];     /* sound-effect table, terminated by name[0] == 0 or '*' */
extern SeEntry *DAT_00901700;       /* cursor into the table while loading / releasing */
extern char     DAT_011215bc[];     /* game base directory */
extern int      DAT_0099d778;
extern int      DAT_009016fc;
extern int      DAT_00a23308;
extern int      DAT_00430658;

int  FUN_00412800();
int FUN_004127a0();
int  FUN_0041e260(const char *path, int id);
void FUN_0041e290(int id);

struct Player {
    int m_0;
    int m_4;
    int m_8[16];
    int m_48;
    int m_4c;
    int m_50;
    int m_54;
    int m_58;
    int m_5c;
};

