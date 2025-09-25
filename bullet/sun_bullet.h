#ifndef PLANTSVSPLANTS_SUN_BULLET_H
#define PLANTSVSPLANTS_SUN_BULLET_H

#include "bullet.h"
#include "animation.h"
#include "global_variable.h"

class SunBullet : public Bullet {
public:
    SunBullet() {
        size.x = 96, size.y = 96;
        damage = 15;

        animation_idle.set_atlas(&atlas_sun);
        animation_idle.set_interval(75);
        animation_idle.set_loop(true);

        animation_explode.set_atlas(&atlas_sun_explode);
        animation_explode.set_interval(75);
        animation_explode.set_loop(false);
        animation_explode.set_callback([&]() { can_remove = true; });

        IMAGE* frame_idle = animation_idle.get_frame();
        IMAGE* frame_explode = animation_explode.get_frame();
        explode_render_offset = {
            (static_cast<float>(frame_idle->getwidth()) - static_cast<float>(frame_explode->getwidth())) / 2.0f,
            (static_cast<float>(frame_idle->getheight()) - static_cast<float>(frame_explode->getheight())) / 2.0f
        };
    }

    ~SunBullet() override = default;

    void on_collide() override {
        Bullet::on_collide();
        // 日光炸弹碰撞到敌人时进行摄像机抖动
        main_camera.shake(3, 250);

        mciSendString(_T("play sun_explode from 0"), nullptr, 0, nullptr);
    }

    void on_update(int delta) override {
        if (valid) {
            velocity.y += gravity * (float)delta;
            position += velocity * (float)delta;
        }

        if (not valid) animation_explode.on_update(delta);
        else animation_idle.on_update(delta);
        if (check_if_exceeds_screen()) can_remove = true;
    }

    void on_draw(const Camera& camera) const override {
        if (valid) animation_idle.on_draw(camera, (int)position.x, (int)position.y);
        else {
            animation_explode.on_draw(camera,
                                      (int)(position.x + explode_render_offset.x),
                                      (int)(position.y + explode_render_offset.y));
        }
        Bullet::on_draw(camera);
    }

private:
    const float gravity = 1e-3f;    // 日光炸弹重力

private:
    Animation animation_idle;       // 日光炸弹默认动画
    Animation animation_explode;    // 日光炸弹波炸动画
    Vector2 explode_render_offset;  // 波炸动画渲染偏移
};

#endif //PLANTSVSPLANTS_SUN_BULLET_H
