#include <stdio.h>
#include <string.h>
#include "game.h"

void Game::M_004048f0(int period, int x, int y, char *text)
{
    if (DAT_00a46040 < period)
        M_0040bfa0(x, y, text);
    DAT_00a46040++;
    if (DAT_00a46040 >= period * 2)
        DAT_00a46040 = 0;
}

void Game::M_004049f0()
{
    if (m_3d4 == 1) {
        m_3d4 = 0;
        M_00411910();
    }
}

int Game::M_00404a10()
{
    int key = 0;

    if (DAT_00a460c4 > 0) {
        key = DAT_00a461d0[0];
        if (key >= 'a' && key <= 'z')
            key -= ' ';
    }
    if (DAT_00a4625c == 1)
        return 0xd;
    if (DAT_00a46260 == 1)
        return 0x1b;
    if (DAT_00a46174 == 1)
        return 0x26;
    if (DAT_00a46178 == 1)
        return 0x28;
    if (DAT_00a4617c == 1)
        return 0x25;
    if (DAT_00a46180 == 1)
        key = 0x27;
    return key;
}

/* checksum over the settings words; the block is the first 0x390 bytes of Game, loose or serialized */
int Game::M_00405160(const int *w)
{
    return w[0xe0] + w[0xdf] + w[0xde] + w[8] + w[7] + w[6] + w[5] + w[4] + w[3] + 0x4cb2f + w[1];
}

#define XOR_KEY "God's in His Heaven, all's right with the world."

void Game::M_004051b0(unsigned char *buf, int len)
{
    int keylen = strlen(XOR_KEY);
    int i;

    if (keylen >= 0) {
        for (i = 0; i < len; i++)
            buf[i] = XOR_KEY[i % keylen] ^ buf[i] ^ 0xa7;
    }
}

void Game::M_00405200(char *dst, const char *src, int len)
{
    if (len > 0) {
        do {
            *dst = *src;
            dst++;
            src++;
            len--;
        } while (len != 0);
    }
}

int Game::M_00405290(const char *path)
{
    FILE *f;
    int size;

    f = fopen(path, "rb");
    if (f == 0)
        return DAT_00430658;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

void Game::M_00404940()
{
    if (m_8ac < 0) {
        M_00410460(-1);
        m_87fcc4 = 0;
        M_004013b0();
        M_00403730();
        FUN_00419e80();
        m_3d4 = 1;
    }
    if (FUN_0041aef0(M_00404a10()) == 0)
        m_a795c = 2;
    else
        FUN_0041a880();
    if (m_a795c != 5) {
        M_004049f0();
        M_004010d0();
        M_00403730();
        return;
    }
    m_8ac++;
    if (m_8ac >= 1000000)
        m_8ac = 1000000;
}

void Game::M_00404a90()
{
    int i;

    memset(this, 0, 0x390);
    m_4 = 0x31;
    m_c = 0;
    m_0 = 0;
    for (i = 0; i < 5; i++) {
        m_10[i] = 100000;
        m_24[i] = 0;
    }
    m_384 = 0;
    m_378 = 2;
    m_37c = 2;
    m_380 = 2;
    m_388 = 0;
    m_38 = 0;
    for (i = 0; i < 0xcf; i++) {
        switch (m_38) {
        case 0:
            m_3c[i] = DAT_004bdab8[i];
            break;
        case 1:
            m_3c[i] = DAT_00a45e40[i];
            break;
        }
    }
}

int Game::M_00404b20()
{
    char path[0x108];
    int baseSize, fullSize;
    unsigned char *block;

    M_00404a90();
    strcpy(path, DAT_011215bc);
    strcat(path, "SE\\");
    strcat(path, "CURSOR");
    strcat(path, ".WAV");
    baseSize = M_00405290(path);
    if (baseSize <= 0)
        return DAT_00430658;
    strcpy(path, DAT_011215bc);
    strcat(path, "SE\\");
    strcat(path, "CURSOR2");
    strcat(path, ".WAV");
    fullSize = M_00405220(path);
    if (fullSize <= 0)
        return DAT_00430658;
    if (fullSize <= baseSize)
        return DAT_00430658;
    if (fullSize - baseSize != 0x390)
        return DAT_00430658;
    block = (unsigned char *)DAT_008f144c + baseSize;
    M_004051b0(block, 0x390);
    if (M_00405160((const int *)block) == *(int *)(block + 8)) {
        M_00405200((char *)this, (char *)block, 0x390);
        if (m_378 < 0 || DAT_004bd608 <= m_378)
            m_378 = 2;
        if (m_37c < 0 || DAT_004bd60c <= m_37c)
            m_37c = 2;
        if (m_380 < 0 || DAT_004bd610 <= m_380)
            m_380 = 2;
    }
    return 0;
}

int Game::M_00404d80()
{
    char path[0x108];
    unsigned char block[0x390];
    int size;
    FILE *f;

    strcpy(path, DAT_011215bc);
    strcat(path, "SE\\");
    strcat(path, "CURSOR");
    strcat(path, ".WAV");
    size = M_00405220(path);
    strcpy(path, DAT_011215bc);
    strcat(path, "SE\\");
    strcat(path, "CURSOR2");
    strcat(path, ".WAV");
    f = fopen(path, "wb");
    if (f == 0)
        return DAT_00430658;
    fwrite(DAT_008f144c, 1, size, f);
    m_8 = M_00405160(&m_0);
    M_00405200((char *)block, (char *)this, 0x390);
    M_004051b0(block, 0x390);
    fwrite(block, 1, 0x390, f);
    fclose(f);
    M_00404fa0();
    return 0;
}

void Game::M_00404fa0()
{
    char path[0x108];
    FILE *f;
    int i;

    strcpy(path, DAT_011215bc);
    strcat(path, "SAVEDATA.TXT");
    f = fopen(path, "wb");
    if (f != 0) {
        fprintf(f, ";--------------------------------------------------------\n");
        fprintf(f, ";        TYPING SPACE HARRIER CONFIGRATION FILE\n");
        fprintf(f, ";--------------------------------------------------------\n");
        fprintf(f, "\n[OPTION]\n");
        fprintf(f, "CONTINUE   = %d\n", m_380);
        fprintf(f, "PLAYER     = %d\n", m_37c);
        fprintf(f, "DIFFICULTY = %d\n", m_378);
        fprintf(f, "\n[INFOMATION]\n");
        fprintf(f, "GAME CLEAR = %d\n", m_c);
        fprintf(f, "TYPE TIMES = %d\n", m_8 + 0x12c728e);
        fprintf(f, "\n[PLAY TIMES]\n");
        for (i = 0; i < 5; i++)
            fprintf(f, "TIMES_%02d = %10d\n", i, m_24[i] * 10);
        fprintf(f, "\n[SCORE]\n");
        for (i = 0; i < 5; i++)
            fprintf(f, "SCORE_%02d = %10d\n", i, m_10[i] * 10);
        fprintf(f, "\n[QUIZ RANK]\n");
        fprintf(f, "DEFAULT = %d\n", m_4);
        fprintf(f, "EASY    = %d\n", m_0 + 0x8bc);
    }
}
