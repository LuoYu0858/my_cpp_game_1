#ifndef PLANTSVSPLANTS_CAMERA_H
#define PLANTSVSPLANTS_CAMERA_H

#include "vector2.h"

class Camera {
public:
    Camera() = default;
    ~Camera() = default;

    const Vector2& get_position() const {
        return position;
    }

    // 重置摄像机位置
    void reset() {
        position.x = 0;
        position.y = 0;
    }

    // 摄像机的帧更新
    void on_update(int delta) {
        const Vector2 speed = {-0.35f, 0.0f};
        position += speed * (float)delta;
    }

private:
    Vector2 position;   // 摄像机位置
};

#endif //PLANTSVSPLANTS_CAMERA_H
