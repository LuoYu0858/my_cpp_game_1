﻿#ifndef PLANTSVSPLANTS_PARTICLE_H
#define PLANTSVSPLANTS_PARTICLE_H

#include "util.h"
#include "atlas.h"
#include "vector2.h"

// 粒子对象类
class Particle {
public:
    Particle() = default;

    Particle(const Vector2& position, Atlas* atlas, int lifespan) : lifespan(lifespan), position(position),
                                                                    atlas(atlas) {}

    ~Particle() = default;

    void set_atlas(Atlas* new_atlas) {
        atlas = new_atlas;
    }

    void set_position(const Vector2& new_position) {
        position = new_position;
    }

    void set_lifespan(int ms) {
        lifespan = ms;
    }

    [[nodiscard]] bool check_valid() const {
        return valid;
    }

    void on_update(int delta) {
        timer += delta;
        if (timer >= lifespan) {
            timer = 0;
            ++idx_frame;
            if (idx_frame >= atlas->get_size()) {
                idx_frame = atlas->get_size() - 1;
                valid = false;
            }
        }
    }

    void on_draw(const Camera& camera) const {
        putimage_alpha(camera, (int)position.x, (int)position.y, atlas->get_image(idx_frame));
    }

private:
    int timer = 0;          // 粒子动画播放定时器
    int lifespan = 0;       // 单帧粒子动画持续时长
    int idx_frame = 0;      // 当前正在播放的动画帧
    Vector2 position;       // 粒子的世界位置坐标
    bool valid = true;      // 粒子动画是否有效
    Atlas* atlas = nullptr; // 粒子动画所使用图集
};

#endif //PLANTSVSPLANTS_PARTICLE_H
