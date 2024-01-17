#include "header.h"

#define nAnimationCounter enemy->n8[0]
#define nPatternIndex enemy->n8[1]
#define nPreviousX enemy->n8[2]
#define nUnkWait enemy->n8[3]

// 4: 鳥
void move_bird(Enemy* enemy) __z88dk_fastcall
{
    if (1 == enemy->flag) {
        enemy->check = 1;
        enemy->vx.raw[0] = get_random(&GV->ridx);
        enemy->vx.value += 128;
        if (get_random(&GV->ridx) & 0x80) {
            // 左 -> 右
            enemy->flag = 0x02;
        } else {
            // 右 -> 左
            enemy->flag = 0x82;
            VGS0_ADDR_OAM[enemy->si[0]].attr |= 0x40;
            enemy->vx.value = -((int16_t)enemy->vx.value);
        }
        nPreviousX = enemy->x.raw[1];
    }

    if (0 == nUnkWait) {
        if (enemy->x.raw[1] < GV->player.x.raw[1] + 24) {
            if (GV->player.x.raw[1] < enemy->x.raw[1] + 8) {
                nUnkWait = (get_random(&GV->ridx) & 0x38) | 0x07;
                add_enemy(ET_UNK, enemy->x.raw[1] + 4, enemy->y.raw[1] + 13);
            }
        }
    } else {
        nUnkWait--;
    }

    if (enemy->flag == 0x02) {
        if (enemy->x.raw[1] < nPreviousX) {
            enemy->flag = 0x03;
        }
    } else if (enemy->flag == 0x03) {
        if (248 < enemy->x.raw[1]) {
            enemy->flag = 0;
            return;
        }
    } else {
        if (nPreviousX < enemy->x.raw[1]) {
            enemy->flag = 0;
            return;
        }
    }
    nPreviousX = enemy->x.raw[1];

    nAnimationCounter++;
    nAnimationCounter &= 3;
    if (0 == nAnimationCounter) {
        nPatternIndex = 1 - nPatternIndex;
        if (nPatternIndex) {
            VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x6A;
        } else {
            VGS0_ADDR_OAM[enemy->si[0]].ptn = 0x68;
            vgs0_se_play(14);
        }
    }
}