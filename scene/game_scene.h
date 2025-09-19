#ifndef PLANTSVSPLANTS_GAME_SCENE_H
#define PLANTSVSPLANTS_GAME_SCENE_H

#include "scene.h"
#include "scene_manager.h"
#include "global_variable.h"

#include <iostream>

// 游戏场景类
class GameScene : public IScene {
public:
    GameScene() = default;
    ~GameScene() override = default;

    void on_enter() override {
        std::cout << "进入游戏局内场景" << std::endl;
    }

    void on_update(int delta) override {
        std::cout << "游戏正在运行..." << std::endl;
    }

    void on_draw(const Camera& camera) override {
        outtextxy(10, 10, _T("游戏局内绘图内容"));
    }

    void on_input(const ExMessage& msg) override {
        if (msg.message == WM_KEYDOWN) scene_manager.switch_to(SceneManager::SceneType::Menu);
    }

    void on_exit() override {
        std::cout << "游戏局内退出" << std::endl;
    }
};

#endif //PLANTSVSPLANTS_GAME_SCENE_H
