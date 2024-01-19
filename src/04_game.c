#include "header.h"

void submain(uint8_t arg) __z88dk_fastcall
{
    if (arg) {
        return; // arg で別処理分岐（未実装）
    }
    uint8_t i;

    // スコアと波をBGからFGに描き直す
    vgs0_fg_putstr(2, 2, 0x80, "SC         0    HI         0");
    score_print(VGS0_ADDR_FG);
    for (i = 0; i < 32; i++) {
        VGS0_ADDR_BG->ptn[2][i] = 0x00;  // スコア(BG)をクリア
        VGS0_ADDR_BG->ptn[9][i] = 0x00;  // 波をクリア
        VGS0_ADDR_FG->ptn[9][i] = 0xA2;  // 波を描画
        VGS0_ADDR_FG->attr[9][i] = 0x80; // 波を描画
    }

    // ハイスコア以外のグローバル変数を初期化
    vgs0_memset(0xC000 + 8, 0x00, sizeof(GlobalVariables) - 8);
    GV->player.x.value = 0x7400;
    GV->player.y.value = 0x4000;
    GV->player.hp = 80;
    GV->player.chp = 0;

    // OAMを初期化
    vgs0_memset((uint16_t)VGS0_ADDR_OAM, 0x00, sizeof(OAM) * 256);
    vgs0_oam_set(SP_JIKI, GV->player.x.raw[1], GV->player.y.raw[1], 0x80, 0x10, 2, 1);
    vgs0_oam_set(SP_JIKI + 1, GV->player.x.raw[1] + 8, GV->player.y.raw[1], 0x80, 0x2F, 0, 0);
    VGS0_ADDR_OAM[SP_LASER].widthMinus1 = 1;
    VGS0_ADDR_OAM[SP_LASER].bank = BANK_LASER_SP;
    VGS0_ADDR_OAM[SP_LTOP].widthMinus1 = 3;
    VGS0_ADDR_OAM[SP_LTOP].heightMinus1 = 1;
    VGS0_ADDR_OAM[SP_LTOP].bank = BANK_LASER2_SP;
    VGS0_ADDR_OAM[SP_LBOTTOM].y = 177;
    VGS0_ADDR_OAM[SP_LBOTTOM].widthMinus1 = 3;
    VGS0_ADDR_OAM[SP_LBOTTOM].heightMinus1 = 0;
    VGS0_ADDR_OAM[SP_LBOTTOM].bank = BANK_LASER2_SP;

    // 体力ゲージの初期描画
    for (i = 0; i < 5; i++) {
        VGS0_ADDR_FG->ptn[3][2 + i] = i + 1;
        VGS0_ADDR_FG->ptn[4][2 + i] = (i + 1) | 0x10;
        VGS0_ADDR_FG->attr[3][2 + i] = 0x87;
        VGS0_ADDR_FG->attr[4][2 + i] = 0x87;
        VGS0_ADDR_OAM[SP_HP + i].x = 16 + (i << 3);
        VGS0_ADDR_OAM[SP_HP + i].y = 32;
        VGS0_ADDR_OAM[SP_HP + i].bank = BANK_MEDAL_SP;
    }
    render_hp();

    vgs0_bgm_play(1);

    // メインループ
    while (1) {
        vgs0_wait_vsync();
        GV->frame++;

        // 敵出現制御
        level_proc();

        // 追加可能なら星と泡を追加
        add_star();
        add_bubble();

        // プレイヤーショットの移動
        move_pshot();

        // プレイヤーの移動
        move_player();

        // 敵キャラの移動
        move_enemy();

        // 画面エフェクトの処理を実行
        screen_effect_proc();

        // スコア加算値の計算処理
        score_calc();

        // スコア再描画 & ハイスコア更新（加算時のみ）
        if (GV->scoreAdded) {
            score_print(VGS0_ADDR_FG);
        }

        // ヒット数を描画
        hit_print();
    }
}