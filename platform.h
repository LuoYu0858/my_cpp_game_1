#ifndef PLANTSVSPLANTS_PLATFORM_H
#define PLANTSVSPLANTS_PLATFORM_H

#include "util.h"
#include "camera.h"
#include "global_variable.h"

// 平台类
class Platform {
public:
    // 平台碰撞外形
    struct CollisionShape {
        float y;            // 平台所在高度
        float left, right;  // 平台碰撞左边缘、右边缘
    };
public:
    Platform() = default;
    ~Platform() = default;

    // 平台绘制
    void on_draw(const Camera& camera) const {
        putimage_alpha(camera, render_position.x, render_position.y, img);

        if (is_debug) {
            setlinecolor(RGB(255, 0, 0));
            line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
        }
    }
public:
    IMAGE* img = nullptr;       // 平台图片
    CollisionShape shape;       // 平台碰撞外形
    POINT render_position{};    // 平台位置
};

#endif //PLANTSVSPLANTS_PLATFORM_H