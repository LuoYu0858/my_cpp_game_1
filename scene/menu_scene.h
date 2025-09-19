#ifndef PLANTSVSPLANTS_MENU_SCENE_H
#define PLANTSVSPLANTS_MENU_SCENE_H

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
    }

    void on_update(int delta) override {
        camera.on_update(delta);
        animation_peashooter_run_right.on_update(delta);
    }

    void on_draw() override {
        const Vector2& pos_camera = camera.get_position();
        animation_peashooter_run_right.on_draw((int)(100 - pos_camera.x), (int)(100 - pos_camera.y));
    }

    void on_input(const ExMessage& msg) override {
        if (msg.message == WM_KEYDOWN) scene_manager.switch_to(SceneManager::SceneType::Game);
    }

    void on_exit() override {
        std::cout << "主菜单退出" << std::endl;
    }

private:
    Camera camera;
    Animation animation_peashooter_run_right;
};

#endif //PLANTSVSPLANTS_MENU_SCENE_H
