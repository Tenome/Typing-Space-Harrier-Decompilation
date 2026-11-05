#include "game.h"

// OPEN: 856 vs 866 bytes (0x40f3a0, camera/effect update). Flow, switch and globals match; this version loses the original register
// plan: the original keeps `this` in a stack slot, the slot-search counter in a stack slot, shares one zero register (ecx) for the
// stores of 0 in the switch arms and loads its prologue in a different order. Class: register/schedule.

void Game::M_0040f3a0(int a, int y)
{
    int i;
    int j;
    int slot;
    int none;
    int now;
    int elapsed;
    int shift;
    int span;
    CamState *cam;
    Rec14 *rec14;
    int *rec;

    if (y < 0)
        y = 0;
    else if (y >= 0x1e0)
        y = 0x1df;
    G_00a3d468.a = 0x20;
    G_00a3d468.b = a;
    G_00a3d468.y = y;
    if (m_390 != 0) {
        G_00a3d468.a = 0;
        G_00a3d468.b = 0;
    }
    i = 0;
    if (DAT_0099d774 > 0) {
        cam = G_008f1010;
        none = DAT_00430658;
        for (rec14 = DAT_004e0ef8; i < DAT_0099d774; i++, rec14++) {
            DAT_00a0a29c = rec14;
            rec = m_3d8;
            if (rec14->m_00 == rec[0] + 1 && rec[12] != cam->now && rec[12] == rec14->m_04) {
                slot = none;
                for (j = 0; j < 8; j++) {
                    G_00a3ce50 = &cam->eff[j];
                    if (cam->eff[j].active == 0) {
                        slot = j;
                        break;
                    }
                }
                if (slot != none) {
                    G_00a3ce50->active = 1;
                    G_00a3ce50->kind = DAT_00a0a29c->m_10;
                    G_00a3ce50->m_8 = DAT_00a0a29c->m_0c;
                    G_00a3ce50->start = DAT_00a0a29c->m_04;
                    G_00a3ce50->m_10 = DAT_00a0a29c->m_08;
                    G_00a3ce50->m_14 = 0;
                    cam = G_008f1010;
                    none = DAT_00430658;
                }
            }
        }
    } else
        cam = G_008f1010;
    cam->now = m_3d8[12];
    cam = G_008f1010;
    now = cam->now;
    for (j = 0; j < 8; j++) {
        G_00a3ce50 = &cam->eff[j];
        if (G_00a3ce50->active == 0)
            continue;
        elapsed = now - G_00a3ce50->start;
        switch (G_00a3ce50->kind) {
        case 0:
        case 1:
            if (G_00a3ce50->kind == 1)
                cam->pos = elapsed;
            else
                cam->pos = cam->len - elapsed;
            cam = G_008f1010;
            if (cam->pos == 0) {
                cam->len = G_00a3ce50->m_10;
                cam = G_008f1010;
            }
            if (elapsed > cam->len) {
                G_00a3ce50->active = 0;
                G_008f1010->pos = 0;
                G_008f1010->len = 0;
                cam = G_008f1010;
            }
            span = cam->len;
            if (span > 0)
                cam->percent = cam->pos * 100 / span;
            break;
        case 2:
            cam->pos2 = elapsed;
            cam = G_008f1010;
            if (cam->pos2 == 0) {
                cam->len2 = G_00a3ce50->m_10;
                G_008f1010->m_20 = G_00a3ce50->m_8;
                cam = G_008f1010;
            }
            if (cam->pos2 > cam->len2) {
                G_00a3ce50->active = 0;
                G_008f1010->m_1c = G_008f1010->m_20;
                G_008f1010->pos2 = 0;
                G_008f1010->len2 = 0;
                cam = G_008f1010;
            }
            span = cam->len2;
            if (span > 0)
                cam->percent2 = cam->pos2 * 255 / span;
            break;
        case 3:
            cam->m_24 = G_00a3ce50->m_8;
            G_00a3ce50->active = 0;
            break;
        }
        cam = G_008f1010;
    }
    G_00a3d468.m_8 = cam->m_24;
    G_00a3d468.m_4 = cam->percent * (G_00a3d468.y - 2) / 100;
    G_00a3d468.m_14 = cam->m_1c;
    G_00a3d468.m_18 = cam->m_20;
    G_00a3d468.m_1c = cam->percent2;
    rec = m_3d8;
    if (rec[3] == 0) {
        shift = rec[12] * 4;
        if (shift < 0)
            shift = -shift;
        if (shift <= 0x100) {
            if (rec[12] >= 0)
                FUN_00419350(0x100 - shift);
            else
                FUN_00419310(0x100 - shift);
        }
    }
    if (FUN_004193c0(&G_00a3d468) == 0)
        FUN_004127a0();
}
