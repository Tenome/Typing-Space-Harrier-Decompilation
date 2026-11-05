#include "game.h"

void Game::M_0040a900(int index)
{
    int id;
    int x;
    int y;
    int cellW;
    int cellH;
    int tile;
    int tileW;
    int tileH;
    int spare;
    int action;
    int actionSpare;
    float scale;
    int tileBase;
    int tileEnd;
    int code;

    if (index < 0 || m_87cbc4->m_7c520c <= index)
        return;
    id = m_87cbc4->m_7c5210[index];
    M_0040af40(0, id);
    if (m_87cbc8->m_0 <= 0)
        return;
    M_0040b4d0(id, 1, &x, &y);
    M_0040b4d0(id, 2, &cellW, &cellH);
    M_0040b4d0(id, 3, &tile, &spare);
    if (tile < 0)
        return;
    tile += m_87cbc8->m_1c * m_87cbc8->m_18;
    M_0040be30(tile, &tileW, &tileH);
    scale = 1.0f;
    if (tileW > 0)
        scale = (float)cellW / tileW;
    if (id != m_87cbc4->m_7d5228) {
        tileBase = M_0040bf80(0x26, 0);
        tileEnd = tileBase + 0x26;
        if ((DAT_00a45e28 == 0 && DAT_00a3d6f0 == 0) || tileBase > tile || tile > tileEnd) {
            M_0040a520(&x, &y, m_87cbc8->m_14);
            if (m_87cbc8->m_14 == 0 && (m_a795c == 4 || m_a795c == 3 || m_a795c == 8))
                y -= cellH / 4;
        }
        tileBase = M_0040bf80(0x26, 0x1e);
    } else
        tileBase = M_0040bf80(0x26, 0);
    if (tile == tileBase) {
        if (x < 0)
            x = 0;
        else if (x >= 0x280)
            x = 0x27f;
        tile = DAT_004e25fc[x / 80] + tileBase;
        if (DAT_00a45e28 == 2 && (m_8a8 & 1) != 0)
            goto skipDraw;
    }
    M_0040bee0(x, y, tile, scale);
skipDraw:
    M_0040b4d0(id, 0xc, &action, &actionSpare);
    code = DAT_00430658;
    switch (action) {
    case -810040:
        code = 10;
        break;
    case -810041:
        code = 11;
        break;
    case -810042:
        code = 12;
        break;
    case -810043:
        code = 13;
        break;
    case -810044:
        code = 14;
        break;
    case -810045:
        code = 15;
        break;
    case -810060:
        code = 16;
        break;
    case -810061:
        code = 17;
        break;
    case -810062:
        code = 18;
        break;
    case -810063:
        code = 19;
        break;
    case -810064:
        code = 20;
        break;
    case -810065:
        code = 21;
        break;
    case -810070:
        code = 22;
        break;
    case -810071:
        code = 23;
        break;
    case -810072:
        code = 24;
        break;
    case -810073:
        code = 25;
        break;
    case -810074:
        code = 26;
        break;
    case -810075:
        code = 27;
        break;
    case -810080:
        code = 28;
        break;
    case -810081:
        code = 29;
        break;
    case -810082:
        code = 30;
        break;
    case -810083:
        code = 31;
        break;
    case -810084:
        code = 32;
        break;
    case -810085:
        code = 33;
        break;
    case -810090:
        code = 34;
        break;
    case -810091:
        code = 35;
        break;
    case -810092:
        code = 36;
        break;
    case -810093:
        code = 37;
        break;
    case -810094:
        code = 38;
        break;
    case -810095:
        code = 39;
        break;
    case -810110:
        code = 40;
        break;
    case -810111:
        code = 41;
        break;
    case -810112:
        code = 42;
        break;
    case -810113:
        code = 43;
        break;
    case -810114:
        code = 44;
        break;
    case -810115:
        code = 45;
        break;
    case -810130:
        code = 46;
        break;
    case -810131:
        code = 47;
        break;
    case -810132:
        code = 48;
        break;
    case -810133:
        code = 49;
        break;
    case -810134:
        code = 50;
        break;
    case -810135:
        code = 51;
        break;
    case -810150:
        code = 52;
        break;
    case -810151:
        code = 53;
        break;
    case -810152:
        code = 54;
        break;
    case -810153:
        code = 55;
        break;
    case -810154:
        code = 56;
        break;
    case -810155:
        code = 57;
        break;
    case -810160:
        code = 58;
        break;
    case -810161:
        code = 59;
        break;
    case -810162:
        code = 60;
        break;
    case -810163:
        code = 61;
        break;
    case -810164:
        code = 62;
        break;
    case -810165:
        code = 63;
        break;
    case -810170:
        code = 64;
        break;
    case -810171:
        code = 65;
        break;
    case -810172:
        code = 66;
        break;
    case -810173:
        code = 67;
        break;
    case -810174:
        code = 68;
        break;
    case -810175:
        code = 69;
        break;
    case -800005:
        code = 5;
        break;
    case -800000:
        code = 0;
        break;
    case -800001:
        code = 1;
        break;
    case -800002:
        code = 2;
        break;
    }
    if (code != DAT_00430658 && M_0040f880() == 0)
        M_00409e00(id, m_87cbc4->m_7d5228, code);
}
