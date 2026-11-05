#include "game.h"

// OPEN: 756 vs 754 bytes (0x409e00, Game::M_00409e00). All blocks and the 0x140 frame match (slot is an int[0x40], only 0x1d zeroed).
// Differences: the original stores the M_00409300 result straight into slot[i] between the argument pushes and keeps the pair id
// in edi; this version keeps a separate stack home for len and ebx for the pair id. Class: register/schedule.

void Game::M_00409e00(int a, int b, int c)
{
    int slot[0x40];
    int pair[2];
    int tick[2];
    float scale[2];
    int xOut[2];
    int yOut[2];
    int width, height;
    int shiftX, shiftY;
    int i, k;
    int part;
    int deltaX, deltaY;
    int speed;
    int pause;
    int tex;
    int result;
    int unit;
    int len;

    if (c >= 0x50)
        return;
    part = DAT_004e24bc[c * 2 + 1];
    tex = DAT_004e24bc[c * 2];
    if (part < 0 || tex < 0)
        return;
    unit = DAT_004efbd8[tex];
    M_0040be30(M_0040bf80(part, 0), &width, &height);
    for (i = 0; i < 0x1d; i++)
        slot[i] = 0;
    for (i = 0; i < 0x1d; i++) {
        result = M_0040bbe0();
        if (result == 0)
            return;
        slot[i] = result;
        M_0040ba00(result);
    }
    pair[0] = a;
    pair[1] = b;
    for (i = 0; i < 2; i++) {
        len = M_00409300(pair[i]);
        slot[i] = len;
        M_0040b4d0(pair[i], 1, &xOut[i], &yOut[i]);
        if (len <= 0x146)
            return;
        scale[i] = M_0040b390(pair[i]);
        if (scale[i] >= 2.0)
            scale[i] = 2.0f;
        tick[i] = M_004096b0(len);
        if (tick[i] < 0)
            return;
        shiftX = (int)(width * scale[i]);
        shiftY = (int)(height * scale[i]);
        deltaX = slot[1] - slot[0];
        deltaY = slot[3] - slot[2];
        speed = 0;
        if (i == 1) {
            speed = tick[0] - tick[1];
            if (speed < 0)
                speed = -speed;
            if (a != m_87cbc4->m_7d5228)
                speed <<= 2;
            deltaY = 10000;
            deltaX = deltaY;
        }
        M_0040b0e0(slot[i], 1, shiftX, shiftY, speed, deltaX);
        M_0040b0e0(slot[2 + i], 1, shiftY, shiftX, speed, deltaY);
    }
    result = M_0040bc60();
    if (result <= 0)
        return;
    M_0040af40(0, result);
    M_0040af40(3, unit);
    for (i = 0; i < 25; i++) {
        if (m_87cbd4->m_2c[i] > 0)
            (&m_87cbc8->m_2c)[i] = m_87cbd4->m_2c[i];
    }
    m_87cbc8->m_14 = 1;
    m_87cbc8->m_64 = slot[0];
    m_87cbc8->m_6c = slot[2];
    for (i = 0; i < 25; i++) {
        if ((&m_87cbc8->m_2c)[i] <= 0)
            (&m_87cbc8->m_2c)[i] = slot[4 + i];
    }
    m_87cbc8->m_0 = unit;
    m_87cbc8->m_10 = 1;
    for (i = 0; i < 0x40; i++) {
        if (i < 0x1d)
            (&m_87cbc8->m_0)[0x24 + i] = slot[i];
        else
            (&m_87cbc8->m_0)[0x24 + i] = 0;
    }
    switch (c) {
    case 0:
        pause = 0x16;
        break;
    case 1:
        pause = 0x12;
        break;
    case 2:
        pause = 0x14;
        break;
    default:
        pause = 0x16;
        break;
    }
    DAT_008f1418[c] = (DAT_008f1418[c] == 0);
    M_00408650(DAT_008f1418[c] + pause);
}
