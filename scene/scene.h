#ifndef PLANTSVSPLANTS_ISCENE_H
#define PLANTSVSPLANTS_ISCENE_H

#include <graphics.h>

// 场景接口
class IScene {
public:
    IScene() = default;
    virtual ~IScene() = default;

    virtual void on_enter() = 0;                        // 进入->初始化
    virtual void on_update(int delta) = 0;                       // 每次帧更新时调用
    virtual void on_draw() = 0;                         // 当前帧绘制
    virtual void on_input(const ExMessage& msg) = 0;    // 处理当前帧输入
    virtual void on_exit() = 0;                         // 退出->销毁
};

#endif //PLANTSVSPLANTS_ISCENE_H
