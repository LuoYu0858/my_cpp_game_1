#ifndef PLANTSVSPLANTS_SCENE_MANAGER_H
#define PLANTSVSPLANTS_SCENE_MANAGER_H

#include "scene.h"
#include "global_variable.h"

// 场景管理器
class SceneManager {
public:
    // 场景状态(枚举)
    enum class SceneType {
        Menu,
        Game,
        Selector
    };

public:
    SceneManager() = default;
    ~SceneManager() = default;

    // 设置当前场景指针, 并进入当前场景
    void set_current_scene(IScene* scene) {
        current_scene = scene;
        current_scene->on_enter();
    }

    // 跳转场景
    void switch_to(const SceneType type) {
        current_scene->on_exit();
        switch (type) {
        case SceneType::Menu:
            current_scene = menu_scene;
            break;
        case SceneType::Game:
            current_scene = game_scene;
            break;
        case SceneType::Selector:
            current_scene = selector_scene;
            break;
        default:
            break;
        }
        current_scene->on_enter();
    }

    // 执行对应场景的帧更新操作
    void on_update(int delta) {
        current_scene->on_update(delta);
    }

    // 执行对应场景的绘图操作
    void on_draw() {
        current_scene->on_draw();
    }

    // 执行对应场景的处理输入操作
    void on_input(const ExMessage& msg) {
        current_scene->on_input(msg);
    }

private:
    IScene* current_scene = nullptr;
};

#endif //PLANTSVSPLANTS_SCENE_MANAGER_H
