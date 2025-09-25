#ifndef PLANTSVSPLANTS_MENU_SCENE_H
#define PLANTSVSPLANTS_MENU_SCENE_H

#include "timer.h"
#include "scene.h"
#include "atlas.h"
#include "camera.h"
#include "animation.h"
#include "scene_manager.h"
#include "global_variable.h"

// 菜单场景类
class MenuScene : public IScene {
public:
    MenuScene() = default;
    ~MenuScene() override = default;

    // 菜单场景进入
    void on_enter() override {
        mciSendString(_T("play bgm_menu repeat from 0"), nullptr, 0, nullptr);
    }

    // 菜单场景帧更新
    void on_update(int delta) override {
    }

    // 菜单场景渲染
    void on_draw(const Camera& o_camera) override {
        putimage(0, 0, &img_menu_background);
    }

    // 处理菜单场景输入
    void on_input(const ExMessage& msg) override {
        if (msg.message == WM_KEYUP) {
            mciSendString(_T("play ui_confirm from 0"), nullptr, 0, nullptr);
            scene_manager.switch_to(SceneManager::SceneType::Selector);
        }
    }

    // 菜单场景退出
    void on_exit() override {
    }

private:
    Timer timer;
    Camera camera;
    Animation animation_peashooter_run_right;
};

#endif //PLANTSVSPLANTS_MENU_SCENE_H
