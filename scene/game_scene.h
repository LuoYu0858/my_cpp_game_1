#ifndef PLANTSVSPLANTS_GAME_SCENE_H
#define PLANTSVSPLANTS_GAME_SCENE_H

#include "util.h"
#include "scene.h"
#include "scene_manager.h"
#include "global_variable.h"

// 游戏场景类
class GameScene : public IScene {
public:
    GameScene() = default;
    ~GameScene() override = default;

    void on_enter() override {
        pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
        pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

        pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
        pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;
    }

    void on_update(int delta) override {
    }

    void on_draw(const Camera& camera) override {
        putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
        putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);
    }

    void on_input(const ExMessage& msg) override {
    }

    void on_exit() override {
    }
private:
    POINT pos_img_sky = {0};    // 天空背景图位置
    POINT pos_img_hills = {0};  // 山脉背景图位置
};

#endif //PLANTSVSPLANTS_GAME_SCENE_H
