#ifndef PLANTSVSPLANTS_MENU_SCENE_H
#define PLANTSVSPLANTS_MENU_SCENE_H

#include "scene.h"
#include "scene_manager.h"
#include "global_variable.h"

#include <iostream>

// 菜单场景类
class MenuScene : public IScene {
public:
    MenuScene() = default;
    ~MenuScene() override = default;

    void on_enter() override {
        std::cout << "进入主菜单" << std::endl;
    }

    void on_update() override {
        std::cout << "主菜单正在运行" << std::endl;
    }

    void on_draw() override {
        outtextxy(10, 10, _T("主菜单绘图内容"));
    }

    void on_input(const ExMessage& msg) override {
        if (msg.message == WM_KEYDOWN) scene_manager.switch_to(SceneManager::SceneType::Game);
    }

    void on_exit() override {
        std::cout << "主菜单退出" << std::endl;
    }

private:
};

#endif //PLANTSVSPLANTS_MENU_SCENE_H
