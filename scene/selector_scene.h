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

        // 背景板滚动扫描线每帧向右移动5个像素, 如果扫描线的位置大于等于选择背景图片的宽度, 让扫描线返回起点
        selector_background_scroll_offset_x += 5;
        if (selector_background_scroll_offset_x >= img_peashooter_selector_background_left.getwidth())
            selector_background_scroll_offset_x = 0;
    }

    void on_draw(const Camera& camera) override {
        IMAGE* img_p1_selector_background = nullptr;
        IMAGE* img_p2_selector_background = nullptr;

        switch (player_type_2) {
        case PlayerType::Peashooter:
            img_p1_selector_background = &img_peashooter_selector_background_right;
            break;
        case PlayerType::Sunflower:
            img_p1_selector_background = &img_sunflower_selector_background_right;
            break;
        default:
            img_p1_selector_background = &img_peashooter_selector_background_right;
            break;
        }

        switch (player_type_1) {
        case PlayerType::Peashooter:
            img_p2_selector_background = &img_peashooter_selector_background_left;
            break;
        case PlayerType::Sunflower:
            img_p2_selector_background = &img_sunflower_selector_background_left;
            break;
        default:
            img_p2_selector_background = &img_peashooter_selector_background_left;
            break;
        }

        putimage(0, 0, &img_selector_background);

        putimage_alpha(selector_background_scroll_offset_x - img_p1_selector_background->getwidth(), 0,
                       img_p1_selector_background);
        putimage_alpha(selector_background_scroll_offset_x, 0,
                       img_p1_selector_background->getwidth() - selector_background_scroll_offset_x, 0,
                       img_p1_selector_background, 0, 0);
        putimage_alpha(getwidth() - img_p2_selector_background->getwidth(), 0,
                       img_p2_selector_background->getwidth() - selector_background_scroll_offset_x, 0,
                       img_p2_selector_background, selector_background_scroll_offset_x, 0);
        putimage_alpha(getwidth() - selector_background_scroll_offset_x, 0, img_p2_selector_background);

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
        putimage_alpha(pos_1P_selector_btn_left.x, pos_1P_selector_btn_left.y,
                       is_btn_1P_left_down ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);
        putimage_alpha(pos_1P_selector_btn_right.x, pos_1P_selector_btn_right.y,
                       is_btn_1P_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
        putimage_alpha(pos_2P_selector_btn_left.x, pos_2P_selector_btn_left.y,
                       is_btn_2P_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
        putimage_alpha(pos_2P_selector_btn_right.x, pos_2P_selector_btn_right.y,
                       is_btn_2P_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);
        putimage_alpha(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);

        putimage_alpha(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
        putimage_alpha(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);
    }

    void on_input(const ExMessage& msg) override {
        switch (msg.message) {
        case WM_KEYDOWN:
            switch (msg.vkcode) {
            case 0x41:
                is_btn_1P_left_down = true;
                break;
            case 0x44:
                is_btn_1P_right_down = true;
                break;
            case VK_LEFT:
                is_btn_2P_left_down = true;
                break;
            case VK_RIGHT:
                is_btn_2P_right_down = true;
                break;
            default: break;
            }
            break;
        case WM_KEYUP:
            switch (msg.vkcode) {
            case 0x41:
                is_btn_1P_left_down = false;
                // 将玩家角色枚举转int并减1, 再加上Invalid的值, 确保其始终大于等于0
                // 对Invalid的值进行取模, 确保最终的值不会大于等于Invalid的值
                // 最后将这个int类型的值转化成PlayerType类型
                player_type_1 = (PlayerType)(((int)PlayerType::Invalid +
                    (int)player_type_1 - 1) % (int)PlayerType::Invalid);
                mciSendString(_T("play ui_switch from 0"), nullptr, 0, nullptr);
                break;
            case 0x44:
                is_btn_1P_right_down = false;
                player_type_1 = (PlayerType)(((int)player_type_1 + 1) % (int)PlayerType::Invalid);
                mciSendString(_T("play ui_switch from 0"), nullptr, 0, nullptr);
                break;
            case VK_LEFT:
                is_btn_2P_left_down = false;
                player_type_2 = (PlayerType)(((int)PlayerType::Invalid +
                    (int)player_type_2 - 1) % (int)PlayerType::Invalid);
                mciSendString(_T("play ui_switch from 0"), nullptr, 0, nullptr);
                break;
            case VK_RIGHT:
                is_btn_2P_right_down = false;
                player_type_2 = (PlayerType)(((int)player_type_2 + 1) % (int)PlayerType::Invalid);
                mciSendString(_T("play ui_switch from 0"), nullptr, 0, nullptr);
                break;
            case VK_RETURN:
                scene_manager.switch_to(SceneManager::SceneType::Game);
                mciSendString(_T("play ui_confirm from 0"), nullptr, 0, nullptr);
                break;
            default: break;
            }
            break;
        default: break;
        }
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
