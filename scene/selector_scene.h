#ifndef PLANTSVSPLANTS_SELECTOR_SCENE_H
#define PLANTSVSPLANTS_SELECTOR_SCENE_H

#include "scene.h"
#include "global_variable.h"

// 选择场景类
class SelectorScene : public IScene {
public:
    SelectorScene() = default;
    ~SelectorScene() override = default;

    void on_enter() override {
        animation_peashooter.set_atlas(&atlas_peashooter_idle_right);
        animation_sunflower.set_atlas(&atlas_sunflower_idle_right);
        animation_peashooter.set_interval(100);
        animation_sunflower.set_interval(100);

        static const int OFFSET_X = 50;

        pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;
        pos_img_VS.y = (getheight() - img_VS.getheight()) / 2;
        pos_img_tip.x = (getwidth() - img_selector_tip.getwidth()) / 2;
        pos_img_tip.y = getheight() - 125;
        pos_img_1P.x = (getwidth() / 2 - img_1P.getwidth()) / 2 - OFFSET_X;
        pos_img_1P.y = 35;
        pos_img_2P.x = getwidth() / 2 + (getwidth() / 2 - img_2P.getwidth()) / 2 + OFFSET_X;
        pos_img_2P.y = pos_img_1P.y;
        pos_img_1P_desc.x = (getwidth() / 2 - img_1P_desc.getwidth()) / 2 - OFFSET_X;
        pos_img_1P_desc.y = getheight() - 150;
        pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc.getwidth()) / 2 + OFFSET_X;
        pos_img_2P_desc.y = pos_img_1P_desc.y;
        pos_img_1P_gravestone.x = (getwidth() / 2 - img_gravestone_right.getwidth()) / 2 - OFFSET_X;
        pos_img_1P_gravestone.y = pos_img_1P.y + img_1P.getheight() + 35;
        pos_img_2P_gravestone.x = getwidth() / 2 + (getwidth() / 2 - img_gravestone_left.getwidth()) / 2 + OFFSET_X;
        pos_img_2P_gravestone.y = pos_img_1P_gravestone.y;
        pos_animation_1P.x = (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 - OFFSET_X;
        pos_animation_1P.y = pos_img_1P_gravestone.y + 80;
        pos_animation_2P.x =
            getwidth() / 2 + (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 + OFFSET_X;
        pos_animation_2P.y = pos_animation_1P.y;
        pos_img_1P_name.y = pos_animation_1P.y + 155;
        pos_img_2P_name.y = pos_img_1P_name.y;
        pos_1P_selector_btn_left.x = pos_img_1P_gravestone.x - img_1P_selector_btn_idle_left.getwidth();
        pos_1P_selector_btn_left.y = pos_img_1P_gravestone.y +
            (img_gravestone_right.getheight() - img_1P_selector_btn_idle_left.getheight()) / 2;
        pos_1P_selector_btn_right.x = pos_img_1P_gravestone.x + img_gravestone_right.getwidth();
        pos_1P_selector_btn_right.y = pos_1P_selector_btn_left.y;
        pos_2P_selector_btn_left.x = pos_img_2P_gravestone.x - img_2P_selector_btn_idle_left.getwidth();
        pos_2P_selector_btn_left.y = pos_1P_selector_btn_left.y;
        pos_2P_selector_btn_right.x = pos_img_2P_gravestone.x + img_gravestone_left.getwidth();
        pos_2P_selector_btn_right.y = pos_1P_selector_btn_left.y;
    }

    void on_update(int delta) override {
        animation_peashooter.on_update(delta);
        animation_sunflower.on_update(delta);
    }

    void on_draw(const Camera& camera) override {
        putimage(0, 0, &img_selector_background);

        putimage_alpha(pos_img_VS.x, pos_img_VS.y, &img_VS);

        putimage_alpha(pos_img_1P.x, pos_img_1P.y, &img_1P);
        putimage_alpha(pos_img_2P.x, pos_img_2P.y, &img_2P);
        putimage_alpha(pos_img_1P_gravestone.x, pos_img_1P_gravestone.y, &img_gravestone_right);
        putimage_alpha(pos_img_2P_gravestone.x, pos_img_2P_gravestone.y, &img_gravestone_left);

        // 根绝玩家角色类型渲染不同角色的动画
        switch (player_type_1) {
        case PlayerType::Peashooter:
            animation_peashooter.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
            pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() -
                textwidth(str_peashooter_name)) / 2;
            outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_peashooter_name);
            break;
        case PlayerType::Sunflower:
            animation_sunflower.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
            pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() -
                textwidth(str_sunflower_name)) / 2;
            outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_sunflower_name);
            break;
        default: break;
        }

        switch (player_type_2) {
        case PlayerType::Peashooter:
            animation_peashooter.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
            pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_right.getwidth() -
                textwidth(str_peashooter_name)) / 2;
            outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_peashooter_name);
            break;
        case PlayerType::Sunflower:
            animation_sunflower.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
            pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_right.getwidth() -
                textwidth(str_sunflower_name)) / 2;
            outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_sunflower_name);
            break;
        default: break;
        }

        putimage_alpha(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
        putimage_alpha(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);

        putimage_alpha(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);
    }

    void on_input(const ExMessage& msg) override {
    }

    void on_exit() override {
    }

private:
    static void outtextxy_shaded(int x, int y, LPCTSTR str) {
        settextcolor(RGB(45, 45, 45));
        outtextxy(x + 2, y + 2, str);
        settextcolor(RGB(255, 255, 255));
        outtextxy(x, y, str);
    }

private:
    enum class PlayerType {
        Peashooter = 0,
        Sunflower,
        Invalid
    };

private:
    POINT pos_img_VS = {0};                 // VS 艺术字图片位置
    POINT pos_img_tip = {0};                // 提示信息文本图片位置
    POINT pos_img_1P = {0};                 // 1P 文本图片位置
    POINT pos_img_2P = {0};                 // 2P 文本图片位置
    POINT pos_img_1P_desc = {0};            // 1P 键位描述图片位置
    POINT pos_img_2P_desc = {0};            // 2P 键位描述图片位置
    POINT pos_img_1P_name = {0};            // 1P 角色姓名文本位置
    POINT pos_img_2P_name = {0};            // 2P 角色姓名文本位置
    POINT pos_animation_1P = {0};           // 1P 角色动画位置
    POINT pos_animation_2P = {0};           // 2P 角色动画位置
    POINT pos_img_1P_gravestone = {0};      // 1P 墓碑图片位置
    POINT pos_img_2P_gravestone = {0};      // 2P 墓碑图片位置
    POINT pos_1P_selector_btn_left = {0};   // 1P 向左切换按钮位置
    POINT pos_1P_selector_btn_right = {0};  // 1P 向右切换按钮位置
    POINT pos_2P_selector_btn_left = {0};   // 2P 向左切换按钮位置
    POINT pos_2P_selector_btn_right = {0};  // 2P 向右切换按钮位置

    Animation animation_peashooter;         // 豌豆射手动画
    Animation animation_sunflower;          // 向日葵动画

    PlayerType player_type_1 = PlayerType::Peashooter;  // 1P 角色类型
    PlayerType player_type_2 = PlayerType::Sunflower;   // 2P 角色类型

    LPCTSTR str_peashooter_name = _T("豌豆射手");   // 豌豆射手角色名
    LPCTSTR str_sunflower_name = _T("向日葵");     // 向日葵角色名

    int selector_background_scroll_offset_x = 0;    // 背景板滚动距离

    bool is_btn_1P_left_down = false;           // 1P 向左切换按钮是否按下
    bool is_btn_1P_right_down = false;          // 1P 向右切换按钮是否按下
    bool is_btn_2P_left_down = false;           // 2P 向左切换按钮是否按下
    bool is_btn_2P_right_down = false;          // 2P 向右切换按钮是否按下
};

#endif //PLANTSVSPLANTS_SELECTOR_SCENE_H
