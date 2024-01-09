#include "header.h"

#define HIT_KEEP_TIME 120

// 敵の種別コード
#define ET_BOMBER 0    // 爆発
#define ET_MARINE_LR 1 // 潜水艦
#define ET_MARINE_RL 2 // 潜水艦

// 敵種別毎の使用スプライト数
const uint8_t tbl_init_sn[9] = {
    1, // 0: 爆発
    3, // 1: 潜水艦 (左から右)
    3, // 2: 潜水艦 (右から左)
};

// 初期パターン定義
static const uint8_t ptn_bomb[1] = {0xA0};
static const uint8_t ptn_marineLR[3] = {0x18, 0x15, 0x16};
static const uint8_t ptn_marineRL[3] = {0x16, 0x15, 0x18};

static uint8_t* get_init_ptn(uint8_t type)
{
    switch (type) {
        case 0: return ptn_bomb;     // 0: 爆発
        case 1: return ptn_marineLR; // 1: 潜水艦 (左から右)
        case 2: return ptn_marineRL;
        default: return (uint8_t*)0;
    }
}

// 属性定義
static const uint8_t attr_bomb[1] = {0x85};
static const uint8_t attr_marineLR[3] = {0x00, 0x00, 0x00};
static const uint8_t attr_marineRL[3] = {0xC0, 0xC0, 0xC0};

static uint8_t* get_init_attr(uint8_t type)
{
    switch (type) {
        case 0: return attr_bomb;     // 0: 爆発
        case 1: return attr_marineLR; // 1: 潜水艦 (左から右)
        case 2: return attr_marineRL;
        default: return (uint8_t*)0;
    }
}

// スプライトの初期座法設定テーブル
static const int8_t ofxy_zero[1] = {0x00};
static const int8_t wh_size2[1] = {2};
static const int8_t ofx_marineLR[3] = {-28, -24, -16};
static const int8_t ofx_marineRL[3] = {0, 16, 22};
static const int8_t ofy_marine[3] = {0, 0, 0};
static const int8_t w_marineLR[3] = {0, 0, 1};
static const int8_t w_marineRL[3] = {1, 0, 0};
static const int8_t h_marine[3] = {1, 1, 1};

static uint8_t* get_init_ofx(uint8_t type)
{
    switch (type) {
        case 0: return ofxy_zero;    // 0: 爆発
        case 1: return ofx_marineLR; // 1: 潜水艦 (左から右)
        case 2: return ofx_marineRL;
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_ofy(uint8_t type)
{
    switch (type) {
        case 0: return ofxy_zero;  // 0: 爆発
        case 1: return ofy_marine; // 1: 潜水艦 (左から右)
        case 2: return ofy_marine;
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_width(uint8_t type)
{
    switch (type) {
        case 0: return wh_size2;   // 0: 爆発
        case 1: return w_marineLR; // 1: 潜水艦 (左から右)
        case 2: return w_marineRL;
        default: return (uint8_t*)0;
    }
}

static uint8_t* get_init_height(uint8_t type)
{
    switch (type) {
        case 0: return wh_size2; // 0: 爆発
        case 1: return h_marine; // 1: 潜水艦 (左から右)
        case 2: return h_marine;
        default: return (uint8_t*)0;
    }
}

// 当たり判定定義テーブル
static const rect_t hittbl[3] = {
    {8, 8, 8, 8},     // 0: 爆発
    {-24, 0, 24, 16}, // 1: 潜水艦 (左から右)
    {0, 0, 24, 16}    // 2: 潜水艦 (右から左)
};

// 敵を追加
void add_enemy(uint8_t type, uint8_t x, uint8_t y)
{
    uint8_t i, j;

    // 追加可能なレコードを探索
    i = GV->enemyIndex;
    while (GV->enemy[i].flag) {
        i++;
        i &= 0x1F;
        if (i == GV->enemyIndex) {
            return; // 空きなし
        }
    }
    GV->enemyIndex = i;
    Enemy* enemy = &GV->enemy[GV->enemyIndex];

    // スプライトに空きがあるかチェック
    for (i = 0; i < tbl_init_sn[type]; i++) {
        j = GV->espIndex;
        while (VGS0_ADDR_OAM[SP_ENEMY + j].ptn != 0x00) {
            j++;
            j &= 0x7F;
            if (j == GV->enemyIndex) {
                return; // 空きなし
            }
        }
        enemy->si[i] = SP_ENEMY + j;
        GV->espIndex = j + 1;
        GV->espIndex &= 0x7F;
    }

    // テーブルに初期値を設定
    enemy->flag = 1;
    enemy->n8[0] = 0;
    enemy->n8[1] = 0;
    enemy->n8[2] = 0;
    enemy->n8[3] = 0;
    enemy->n16.value = 0;
    enemy->check = 0;
    enemy->type = type;
    enemy->x.raw[1] = x;
    enemy->y.raw[1] = y;
    enemy->vx.value = 0;
    enemy->vy.value = 0;
    enemy->sn = tbl_init_sn[type];

    // OAMに初期値を設定
    uint8_t* ptn = get_init_ptn(type);
    uint8_t* attr = get_init_attr(type);
    int8_t* ofx = get_init_ofx(type);
    int8_t* ofy = get_init_ofy(type);
    int8_t* w = get_init_width(type);
    int8_t* h = get_init_height(type);
    for (i = 0; i < enemy->sn; i++) {
        vgs0_oam_set(enemy->si[i], x + ofx[i], y + ofy[i], attr[i], ptn[i], w[i], h[i]);
        GV->espIndex += 1;
        GV->espIndex &= 0x7F;
    }
    score_increment(0);
    GV->enemyIndex += 1;
    GV->enemyIndex &= 0x1F;
}

// 敵を削除
static void erase_enemy(Enemy* enemy) __z88dk_fastcall
{
    enemy->flag = 0;
    for (uint8_t i = 0; i < enemy->sn; i++) {
        VGS0_ADDR_OAM[enemy->si[i]].ptn = 0x00;
        VGS0_ADDR_OAM[enemy->si[i]].attr = 0x00;
    }
}

// 座標更新
static void update_enemy_position(Enemy* enemy) __z88dk_fastcall
{
    uint8_t dx = enemy->x.raw[1];
    uint8_t dy = enemy->y.raw[1];
    enemy->x.value += enemy->vx.value;
    enemy->y.value += enemy->vy.value;
    dx = enemy->x.raw[1] - dx;
    dy = enemy->y.raw[1] - dy;

    // 表示座標に変化があればスプライトを動かす
    if (dx || dy) {
        for (uint8_t i = 0; i < enemy->sn; i++) {
            VGS0_ADDR_OAM[enemy->si[i]].x += dx;
            VGS0_ADDR_OAM[enemy->si[i]].y += dy;
        }
    }
}

// 自機ショットとの当たり判定チェック
static void check_hit_pshot(Enemy* enemy) __z88dk_fastcall
{
    if (0 == enemy->flag || 0 == enemy->check) {
        return;
    }
    uint8_t et = enemy->y.raw[1];
    et += hittbl[enemy->type].y;
    uint8_t eb = et;
    eb += hittbl[enemy->type].height;
    uint8_t el = enemy->x.raw[1];
    el += hittbl[enemy->type].x;
    uint8_t er = el;
    er += hittbl[enemy->type].width;
    for (uint8_t i = 0; i < 8; i++) {
        Shot* shot = &GV->shot[i];
        if (shot->flag) {
            // Y座標が範囲内かチェック
            if (shot->y.raw[1] < eb && et < shot->y.raw[1] + 8) {
                // X座標が範囲内ならヒット
                if (shot->x < er && el < shot->x + 8) {
                    if (0 != enemy->type) {
                        add_enemy(ET_BOMBER, el + (er - el - 24) / 2, et + (eb - et - 24) / 2);
                    }
                    GV->shot[i].flag = 0;
                    VGS0_ADDR_OAM[SP_SHOT + i].attr = 0x00;
                    add_enemy(ET_BOMBER, GV->shot[i].x - 8, GV->shot[i].y.raw[1] - 8);
                    if (0 != enemy->type) {
                        enemy->flag = 0;
                    }
                    GV->hit++;
                    GV->hkt = HIT_KEEP_TIME;
                    return;
                }
            }
        }
    }
}

// 爆発の当たり判定チェック（爆発以外のものを誘爆）
static void check_hit_bomb(Enemy* bomb) __z88dk_fastcall
{
    if (0 == bomb->check || ET_BOMBER != bomb->type) {
        return; // I'm not explosion
    }
    uint8_t bt = bomb->y.raw[1] + 8;
    uint8_t bb = bomb->y.raw[1] + 16;
    uint8_t bl = bomb->x.raw[1] + 8;
    uint8_t br = bomb->x.raw[1] + 16;
    for (uint8_t i = 0; i < 32; i++) {
        Enemy* enemy = &GV->enemy[i];
        if (ET_BOMBER != enemy->type && enemy->flag && enemy->check) {
            // Y座標が範囲内かチェック
            uint8_t et = enemy->y.raw[1];
            et += hittbl[enemy->type].y;
            uint8_t eb = et;
            eb += hittbl[enemy->type].height;
            if (bt < eb && et < bb) {
                // X座標が範囲内ならヒット
                uint8_t el = enemy->x.raw[1];
                el += hittbl[enemy->type].x;
                uint8_t er = el;
                er += hittbl[enemy->type].width;
                if (bl < er && el < br) {
                    add_enemy(ET_BOMBER, el + (er - el - 24) / 2, et + (eb - et - 24) / 2);
                    erase_enemy(enemy);
                    GV->hit++;
                    GV->hkt = HIT_KEEP_TIME;
                }
            }
        }
    }
}

void move_enemy(void) __z88dk_fastcall
{
    uint8_t i;
    for (i = 0; i < 32; i++) {
        Enemy* enemy = &GV->enemy[i];
        if (enemy->flag) {
            switch (enemy->type) {
                case ET_BOMBER: move_bomber(enemy); break;
                case ET_MARINE_LR: move_marineLR(enemy); break;
            }
            if (0 == enemy->flag) {
                erase_enemy(enemy);
            } else {
                update_enemy_position(enemy);
                check_hit_pshot(enemy);
                if (0 == enemy->flag) {
                    erase_enemy(enemy);
                } else {
                    check_hit_bomb(enemy);
                }
            }
        }
    }
}
