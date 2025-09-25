#ifndef PLANTSVSPLANTS_GAME_SCENE_H
#define PLANTSVSPLANTS_GAME_SCENE_H

#include "util.h"
#include "scene.h"
#include "player.h"
#include "platform.h"
#include "status_bar.h"
#include "global_variable.h"

#include <vector>

// 游戏场景类
class GameScene : public IScene {
public:
    GameScene() = default;
    ~GameScene() override = default;

    void on_enter() override {
        is_game_over = false;
        is_slide_out_started = false;

        pos_img_winner_bar.x = -img_winner_bar.getwidth();
        pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
        pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;

        pos_img_winner_text = {
            pos_img_winner_bar.x,
            (getheight() - img_1P_winner.getheight()) / 2
        };
        pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

        timer_winner_slide_in.restart();
        timer_winner_slide_in.set_wait_time(2500);
        timer_winner_slide_in.set_one_shot(true);
        timer_winner_slide_in.set_callback([&]() { is_slide_out_started = true; });

        timer_winner_slide_out.restart();
        timer_winner_slide_out.set_wait_time(1000);
        timer_winner_slide_out.set_one_shot(true);
        timer_winner_slide_out.set_callback([&]() { scene_manager.switch_to(SceneManager::SceneType::Menu); });

        status_bar_1P.set_avatar(img_player_1_avatar);
        status_bar_2P.set_avatar(img_player_2_avatar);

        status_bar_1P.set_position(235, 625);
        status_bar_2P.set_position(675, 625);

        player_1->set_position(200, 50);
        player_2->set_position(975, 50);

        pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
        pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

        pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
        pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

        platform_list.resize(4);

        // 创建地面(大型平台)
        Platform& large_platform = platform_list[0];
        large_platform.img = &img_platform_large;
        // 平台绘制位置
        large_platform.render_position.x = 122;
        large_platform.render_position.y = 455;
        // 平台碰撞外形的位置(略小于实际位置)
        large_platform.shape.left = (float)large_platform.render_position.x + 30;
        large_platform.shape.right = (float)large_platform.render_position.x +
                (float)img_platform_large.getwidth() - 30;
        large_platform.shape.y = (float)large_platform.render_position.y + 60;

        // 创建3个小平台
        Platform& small_platform_1 = platform_list[1];
        Platform& small_platform_2 = platform_list[2];
        Platform& small_platform_3 = platform_list[3];
        small_platform_1.img = &img_platform_small;
        small_platform_2.img = &img_platform_small;
        small_platform_3.img = &img_platform_small;

        small_platform_1.render_position.x = 160;
        small_platform_1.render_position.y = 360;
        small_platform_1.shape.left = (float)small_platform_1.render_position.x + 40;
        small_platform_1.shape.right = (float)small_platform_1.render_position.x +
                (float)img_platform_small.getwidth() - 40;
        small_platform_1.shape.y = (float)small_platform_1.render_position.y +
                (float)img_platform_small.getheight() / 2;

        small_platform_2.render_position.x = 870;
        small_platform_2.render_position.y = 360;
        small_platform_2.shape.left = (float)small_platform_2.render_position.x + 40;
        small_platform_2.shape.right = (float)small_platform_2.render_position.x +
                (float)img_platform_small.getwidth() - 40;
        small_platform_2.shape.y = (float)small_platform_2.render_position.y +
                (float)img_platform_small.getheight() / 2;

        small_platform_3.render_position.x = 515;
        small_platform_3.render_position.y = 225;
        small_platform_3.shape.left = (float)small_platform_3.render_position.x + 40;
        small_platform_3.shape.right = (float)small_platform_3.render_position.x +
                (float)img_platform_small.getwidth() - 40;
        small_platform_3.shape.y = (float)small_platform_3.render_position.y +
                (float)img_platform_small.getheight() / 2;

        mciSendString(_T("play bgm_game repeat from 0"), nullptr, 0, nullptr);
    }

    void on_update(int delta) override {
        player_1->on_update(delta);
        player_2->on_update(delta);

        main_camera.on_update(delta);

        // 无效子弹移除
        std::erase_if(
            bullet_list, [](const Bullet* bullet) {
                bool delete_able = bullet->check_can_remove();
                if (delete_able) delete bullet;
                return delete_able;
            }
        );

        for (auto bullet : bullet_list) bullet->on_update(delta);

        const Vector2& position_player_1 = player_1->get_position();
        const Vector2& position_player_2 = player_2->get_position();

        if (position_player_1.y >= (float)getheight()) player_1->set_hp(0);
        if (position_player_2.y >= (float)getheight()) player_2->set_hp(0);

        if (player_1->get_hp() <= 0 or player_2->get_hp() <= 0) {
            if (not is_game_over) {
                mciSendString(_T("stop bgm_game"), nullptr, 0, nullptr);
                mciSendString(_T("play ui_win from 0"), nullptr, 0, nullptr);
            }
            is_game_over = true;
        }

        status_bar_1P.set_hp(player_1->get_hp());
        status_bar_1P.set_mp(player_1->get_mp());
        status_bar_2P.set_hp(player_2->get_hp());
        status_bar_2P.set_mp(player_2->get_mp());

        if (is_game_over) {
            pos_img_winner_bar.x += (int)(speed_winner_bar * (float)delta);
            pos_img_winner_text.x += (int)(speed_winner_text * (float)delta);

            if (not is_slide_out_started) {
                timer_winner_slide_in.on_update(delta);
                if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst)
                    pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
                if (pos_img_winner_text.x > pos_x_img_winner_text_dst)
                    pos_img_winner_text.x = pos_x_img_winner_text_dst;
            } else {
                timer_winner_slide_out.on_update(delta);
            }
        }
    }

    void on_draw(const Camera& camera) override {
        putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
        putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

        for (const Platform& platform : platform_list) platform.on_draw(camera);

        if (is_debug) {
            settextcolor(RGB(255, 0, 0));
            outtextxy(15, 15, _T("已开启调试模式, 按'Q'键关闭"));
        }

        player_1->on_draw(camera);
        player_2->on_draw(camera);

        for (const auto bullet : bullet_list) bullet->on_draw(camera);

        if (is_game_over) {
            putimage_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
            putimage_alpha(pos_img_winner_text.x, pos_img_winner_text.y,
                           player_1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
        } else {
            status_bar_1P.on_draw();
            status_bar_2P.on_draw();
        }
    }

    void on_input(const ExMessage& msg) override {
        player_1->on_input(msg);
        player_2->on_input(msg);

        switch (msg.message) {
            case WM_KEYUP:
                if (msg.vkcode == 0x51) is_debug = not is_debug;
                break;
            default: break;
        }
    }

    void on_exit() override {
        delete player_1, player_1 = nullptr;
        delete player_2, player_2 = nullptr;

        bullet_list.clear();
        main_camera.reset();
    }

private:
    const float speed_winner_bar = 3.0f;    // 结算动效背景移动速度
    const float speed_winner_text = 1.5f;   // 结算动效文本移动速度

private:
    POINT pos_img_sky = {0};            // 天空背景图位置
    POINT pos_img_hills = {0};          // 山脉背景图位置
    POINT pos_img_winner_bar = {0};     // 结算动效背景位置
    POINT pos_img_winner_text = {0};    // 结算动效文本位置

    StatusBar status_bar_1P;            // 玩家1的状态条
    StatusBar status_bar_2P;            // 玩家2的状态条

    bool is_game_over = false;          // 游戏是否结束

    bool is_slide_out_started = false;  // 结算动效是否开始滑出

    int pos_x_img_winner_bar_dst = 0;   // 结算动效背景移动的目标位置
    int pos_x_img_winner_text_dst = 0;  // 结算动效文本移动的目标位置

    Timer timer_winner_slide_in;        // 结算动效滑入定时器
    Timer timer_winner_slide_out;       // 结算动效滑出定时器
};

#endif //PLANTSVSPLANTS_GAME_SCENE_H
