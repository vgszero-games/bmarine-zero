#include "header.h"

// スコア表示（ハイスコア更新を含む）
void score_print(NameTable* nam) __z88dk_fastcall
{
    uint8_t detectS = 0;
    uint8_t detectH = 0;
    uint8_t updateH = 0;
    uint8_t notUpdateH = 0;
    for (uint8_t i = 0; i < 8; i++) {
        // score
        if (!updateH) {
            if (GV->hi[7 - i] < GV->sc[7 - i]) {
                if (0 == notUpdateH) {
                    updateH = 1;
                }
            } else if (GV->sc[7 - i] < GV->hi[7 - i]) {
                notUpdateH = 1;
            }
        }
        if (updateH) {
            GV->hi[7 - i] = GV->sc[7 - i];
        }
        if (7 == i || 0 != detectS || 0 != GV->sc[7 - i]) {
            nam->ptn[2][5 + i] = GV->sc[7 - i] + '0';
            detectS = 1;
        } else {
            nam->ptn[2][5 + i] = ' ';
        }
        // high score
        if (7 == i || 0 != detectH || 0 != GV->hi[7 - i]) {
            nam->ptn[2][21 + i] = GV->hi[7 - i] + '0';
            detectH = 1;
        } else {
            nam->ptn[2][21 + i] = ' ';
        }
    }
    GV->scoreAdded = 0;
}

// 指定ケタ（0: 10の位, 7: 億の位）のスコアを加算
void score_increment(uint8_t keta) __z88dk_fastcall
{
    GV->scoreAdded = 1;
    if (7 < keta) {
        for (uint8_t i = 0; i < 8; i++) {
            GV->sc[i] = 9;
        }
        return;
    }
    GV->sc[keta] += 1;
    if (9 < GV->sc[keta]) {
        GV->sc[keta] = 0;
        score_increment(keta + 1);
    }
}

void hit_print() __z88dk_fastcall
{
    if (9999 < GV->hit) {
        GV->hit = 9999;
        GV->hitstr[0] = '9';
        GV->hitstr[1] = '9';
        GV->hitstr[2] = '9';
        GV->hitstr[3] = '9';
    } else {
        if (GV->hit < 1000) {
            GV->hitstr[0] = ' ';
        } else {
            GV->hitstr[0] = GV->hit / 1000;
            GV->hitstr[0] += '0';
        }

        if (GV->hit < 100) {
            GV->hitstr[1] = ' ';
        } else {
            GV->hitstr[1] = GV->hit / 100;
            GV->hitstr[1] += '0';
        }

        if (GV->hit < 10) {
            GV->hitstr[2] = ' ';
        } else {
            GV->hitstr[2] = GV->hit / 10;
            GV->hitstr[2] += '0';
        }

        if (0 < GV->hit) {
            GV->hitstr[3] = GV->hit % 10;
            GV->hitstr[3] += '0';
            GV->hitstr[4] = 'H';
            GV->hitstr[5] = 'I';
            GV->hitstr[6] = 'T';
        } else {
            GV->hitstr[3] = ' ';
            GV->hitstr[4] = ' ';
            GV->hitstr[5] = ' ';
            GV->hitstr[6] = ' ';
        }
        vgs0_fg_putstr(23, 4, 0x80, GV->hitstr);
    }
}