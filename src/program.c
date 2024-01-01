#include "../vgszero/lib/sdcc/vgs0lib.h"
#include "palette.h"
#include "global.h"

// 乱数テーブル
const uint8_t random[256] = {
    0x2A, 0x46, 0x5E, 0x7D, 0x45, 0xDA, 0x1E, 0x72, 0x38, 0x43, 0xD4, 0xD1, 0x3E, 0x69, 0xAC, 0x7E,
    0x08, 0x79, 0x8F, 0xF5, 0x0F, 0xE8, 0xE4, 0x41, 0x6D, 0x71, 0x2F, 0x04, 0xE3, 0x5D, 0xD0, 0xC3,
    0x19, 0x7B, 0xDF, 0x1A, 0x6E, 0xCD, 0xC8, 0x84, 0x27, 0xCA, 0xBA, 0x53, 0xA8, 0x62, 0x16, 0xFF,
    0x3C, 0x22, 0x51, 0x95, 0x0E, 0x63, 0x26, 0xB3, 0x42, 0xED, 0xA0, 0x78, 0x73, 0xC5, 0x34, 0xDE,
    0x9F, 0xE6, 0xA1, 0xB9, 0x61, 0x59, 0x24, 0x9D, 0xF4, 0x68, 0x00, 0x5A, 0x7C, 0x91, 0x85, 0xC4,
    0xD5, 0x3D, 0xC2, 0x31, 0x99, 0x30, 0x17, 0x8E, 0x3A, 0x96, 0xB7, 0xC1, 0xB1, 0xB5, 0x3B, 0x93,
    0xEB, 0x4F, 0x4A, 0x9A, 0x70, 0x37, 0x60, 0x09, 0xD2, 0xAA, 0xD8, 0xB2, 0xD3, 0x29, 0xF7, 0x67,
    0x1D, 0x0D, 0xF9, 0x4D, 0xF6, 0x77, 0xEC, 0x82, 0x06, 0x2B, 0x14, 0xF3, 0x6F, 0xF1, 0x4E, 0xBD,
    0x83, 0xAF, 0x55, 0x81, 0x49, 0x6A, 0x50, 0x35, 0xA3, 0xE1, 0x8D, 0x75, 0xBC, 0xA9, 0x07, 0x65,
    0x01, 0x57, 0x97, 0xE5, 0xC9, 0x3F, 0x10, 0xC0, 0x89, 0xEE, 0x74, 0x9E, 0x66, 0x8B, 0x0C, 0x1F,
    0x25, 0x39, 0x64, 0xE2, 0x5C, 0x47, 0x40, 0x32, 0xFE, 0x6C, 0xF8, 0xB4, 0xA5, 0xB0, 0x44, 0x36,
    0xCE, 0x5F, 0x6B, 0x05, 0xD7, 0xAE, 0x33, 0x52, 0x1B, 0x11, 0x1C, 0xDC, 0x48, 0x02, 0xCF, 0xF0,
    0x80, 0x7F, 0x28, 0xE7, 0x92, 0xE0, 0x9B, 0x86, 0x20, 0xCB, 0x7A, 0x54, 0x0B, 0xC6, 0x94, 0xBF,
    0x76, 0xDD, 0xCC, 0xB8, 0x13, 0x4B, 0x0A, 0x5B, 0x88, 0xFD, 0x18, 0xFA, 0x9C, 0x98, 0xA4, 0x2C,
    0xDB, 0x12, 0xAD, 0x03, 0x58, 0xEF, 0xFB, 0xA6, 0xD6, 0x8C, 0xD9, 0xC7, 0x2D, 0xF2, 0x15, 0xA2,
    0x2E, 0xA7, 0x4C, 0x87, 0xB6, 0x90, 0x56, 0xE9, 0xEA, 0x23, 0xBE, 0xFC, 0xAB, 0x8A, 0x21, 0xBB};

static const uint8_t titleBattle[32 * 5] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x84, 0x8D, 0x8E, 0x84, 0x8D, 0x88, 0x8c, 0x89, 0x88, 0x8c, 0x89, 0x87, 0x00, 0x00, 0x80, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x85, 0x85, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8A, 0x84, 0x8B, 0x8A, 0x84, 0x8B, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x8A, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x00, 0x85, 0x85, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x85, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x84, 0x83, 0x86, 0x00, 0x86, 0x00, 0x86, 0x00, 0x00, 0x86, 0x00, 0x81, 0x84, 0x89, 0x81, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t titleMarine[32 * 5] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x8C, 0x82, 0x8E, 0x84, 0x8D, 0x80, 0x84, 0x8D, 0x88, 0x8c, 0x89, 0x80, 0x8D, 0x87, 0x80, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x85, 0x85, 0x00, 0x85, 0x00, 0x85, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x85, 0x85, 0x8A, 0x84, 0x8B, 0x8A, 0x84, 0x90, 0x00, 0x85, 0x00, 0x85, 0x85, 0x85, 0x8A, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x85, 0x85, 0x00, 0x91, 0x00, 0x85, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0x86, 0x86, 0x86, 0x00, 0x86, 0x86, 0x00, 0x86, 0x88, 0x8F, 0x89, 0x86, 0x81, 0x83, 0x81, 0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const int8_t rasterTable[32] = {
    0, 0, 1, 1, 2, 2, 3, 3,
    4, 4, 3, 3, 2, 2, 1, 1,
    0, 0, -1, -1, -2, -2, -3, -3,
    -4, -4, -3, -3, -2, -2, -1, -1
};

void main(void)
{
    // パレットを初期化
    init_palette();

    // DPM を設定
    *VGS0_ADDR_BG_DPM = 4;
    *VGS0_ADDR_FG_DPM = 5;
    *VGS0_ADDR_SPRITE_DPM = 6;

    // FGにタイトル描画
    uint8_t i, j;
    uint8_t n;
    n = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 32; j++) {
            VGS0_ADDR_FG->ptn[i + 5][j] = titleBattle[n];
            VGS0_ADDR_FG->attr[i + 5][j] = 0x80;
            n++;
        }
    }
    n = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 32; j++) {
            VGS0_ADDR_FG->ptn[i + 10][j] = titleMarine[n];
            VGS0_ADDR_FG->attr[i + 10][j] = 0x81;
            n++;
        }
    }

    vgs0_fg_putstr(4, 1, 0x80, "SC      00    HI      00");
    vgs0_fg_putstr(11, 16, 0x80, "- KAISEN -");
    vgs0_fg_putstr(7, 20, 0x80, "PRESS START BUTTON");
    vgs0_fg_putstr(3, 24, 0x80, "@COPYRIGHT 2024 SUZUKIPLAN");

    // BGを表示
    n = 0;
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 32; j++) {
            VGS0_ADDR_BG->ptn[i][j] = 1;
            VGS0_ADDR_BG->attr[i][j] = 0x82;
            n++;
            n &= 3;
        }
    }

    // 泡を初期化
    for (i = 0; i < 16; i++) {
        uint8_t x = random[GV->ridx];
        GV->ridx++;
        uint8_t y = random[GV->ridx] % 192;
        GV->ridx++;
        vgs0_oam_set(i, x, y, 0x82, i);
    }

    // ループ
    uint8_t a = 0;
    uint8_t sidx = 0;
    while (1) {
        a++;
        // PRESS START BUTTON を点滅
        switch (a & 0x3F) {
            case 0:
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->attr[20][i] = 0x80;
                }
                break;
            case 24:
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->attr[20][i] = 0x81;
                }
                break;
            case 32:
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->attr[20][i] = 0x00;
                }
                break;
            case 56:
                for (i = 0; i < 32; i++) {
                    VGS0_ADDR_FG->attr[20][i] = 0x81;
                }
                break;
        }

        // スプライトの泡を動かす
        for (i = 0; i < 16; i++) {
            VGS0_ADDR_OAM[i].y -= 1;
            if (a & 1) {
                VGS0_ADDR_OAM[i].ptn += 1;
            }
            if (16 == VGS0_ADDR_OAM[i].ptn) {
                uint8_t x = random[GV->ridx];
                GV->ridx++;
                uint8_t y = random[GV->ridx] % 192;
                GV->ridx++;
                vgs0_oam_set(i, x, y, 0x82, 0);
            }
        }

        // V-SYNC待ち
        vgs0_wait_vsync();

        // BGを縦スクロール
        if (a & 1) {
            *VGS0_ADDR_BG_SCROLL_Y += 1;
        }

        // BGをラスタースクロール
        for (i = 0; i < 192; i++) {
            while (i != *VGS0_ADDR_COUNT_V) {
                ;
            }
            *VGS0_ADDR_BG_SCROLL_X = rasterTable[(sidx + i) & 0x1F];
        }
        sidx++;
    }
}
