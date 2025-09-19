#ifndef PLANTSVSPLANTS_MENU_SCENE_H
#define PLANTSVSPLANTS_MENU_SCENE_H

#include "timer.h"
#include "scene.h"
#include "atlas.h"
#include "camera.h"
#include "animation.h"
#include "scene_manager.h"
#include "global_variable.h"

#include <iostream>

// 菜单场景类
class MenuScene : public IScene {
public:
    MenuScene() = default;
    ~MenuScene() override = default;

    void on_enter() override {
        animation_peashooter_run_right.set_atlas(&atlas_peashooter_run_right);
        animation_peashooter_run_right.set_interval(105);
        animation_peashooter_run_right.set_loop(true);

        timer.set_wait_time(1000);
        timer.set_one_shot(false);
        timer.set_callback([]() {
            std::cout << "Shot!" << std::endl;
        });
    }

    void on_update(int delta) override {
        timer.on_update(delta);
        camera.on_update(delta);
        animation_peashooter_run_right.on_update(delta);
    }

    void on_draw() override {
        const Vector2& pos_camera = camera.get_position();
        animation_peashooter_run_right.on_draw((int)(100 - pos_camera.x), (int)(100 - pos_camera.y));
    }

    void on_input(const ExMessage& msg) override {
        if (msg.message == WM_KEYDOWN) camera.shake(5, 350);
    }

    void on_exit() override {
        std::cout << "主菜单退出" << std::endl;
    }

private:
    Timer timer;
    Camera camera;
    Animation animation_peashooter_run_right;
};

#endif //PLANTSVSPLANTS_MENU_SCENE_H
