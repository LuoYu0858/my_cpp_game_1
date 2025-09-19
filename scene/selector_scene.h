#ifndef PLANTSVSPLANTS_SELECTOR_SCENE_H
#define PLANTSVSPLANTS_SELECTOR_SCENE_H

#include "scene.h"
#include "scene_manager.h"
#include "global_variable.h"

// 选择场景类
class SelectorScene : public IScene{
public:
    SelectorScene() = default;
    ~SelectorScene() override = default;

    void on_enter() override {

    }
    void on_update(int delta) override {

    }
    void on_draw() override {

    }
    void on_input(const ExMessage& msg) override {

    }
    void on_exit() override {

    }

private:
};

#endif //PLANTSVSPLANTS_SELECTOR_SCENE_H