#ifndef PLANTSVSPLANTS_CAMERA_H
#define PLANTSVSPLANTS_CAMERA_H

#include "timer.h"
#include "vector2.h"

// 摄像机类
class Camera {
public:
    Camera() {
        timer_shake.set_one_shot(true);
        timer_shake.set_callback([&]() {
            is_shaking = false;
            reset();
        });
    }

    ~Camera() = default;

    [[nodiscard]] const Vector2& get_position() const {
        return position;
    }

    // 重置摄像机位置
    void reset() {
        position.x = 0;
        position.y = 0;
    }

    // 摄像机抖动
    void shake(float strength, int duration) {
        is_shaking = true;
        shaking_strength = strength;

        timer_shake.set_wait_time(duration);
        timer_shake.restart();
    }

    // 摄像机的帧更新
    void on_update(int delta) {
        timer_shake.on_update(delta);

        // 如果摄像机正在抖动, 快速随机修改摄像机位置
        if (is_shaking) {
            position.x = (-50 + (float)(rand() % 100)) / 50.f * shaking_strength;
            position.y = (-50 + (float)(rand() % 100)) / 50.f * shaking_strength;
        }
    }
private:
    Vector2 position;           // 摄像机位置
    Timer timer_shake;          // 摄像机抖动定时器
    bool is_shaking = false;    // 摄像机是否正在抖动
    float shaking_strength = 0; // 摄像机抖动幅度
};

#endif //PLANTSVSPLANTS_CAMERA_H
