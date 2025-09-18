#ifndef PLANTSVSPLANTS_GAME_SCENE_H
#define PLANTSVSPLANTS_GAME_SCENE_H

#include "scene.h"

#include <iostream>

class GameScene final : public IScene {
public:
    GameScene() = default;
    ~GameScene() override = default;

    void on_enter() override {
        std::cout << "进入游戏局内场景" << std::endl;
    }

    void on_update() override {
        std::cout << "游戏正在运行..." << std::endl;
    }

    void on_draw() override {
        outtextxy(10, 10, _T("游戏局内绘图内容"));
    }

    void on_input(const ExMessage& msg) override {
    }

    void on_exit() override {
        std::cout << "游戏局内退出" << std::endl;
    }
};

#endif //PLANTSVSPLANTS_GAME_SCENE_H
