#ifndef PLANTSVSPLANTS_SUN_BULLET_EX_H
#define PLANTSVSPLANTS_SUN_BULLET_EX_H

#include "bullet.h"
#include "animation.h"
#include "global_variable.h"


class SunBulletEx : public Bullet {
public:
    SunBulletEx() {
        size.x = 275, size.y = 275;

        damage = 45;

        animation_idle.set_atlas(&atlas_sun_ex);
        animation_idle.set_interval(75);
        animation_idle.set_loop(true);

        animation_explode.set_atlas(&atlas_sun_ex_explode);
        animation_explode.set_interval(75);
        animation_explode.set_loop(false);
        animation_explode.set_callback([&]() { can_remove = true; });

        IMAGE* frame_idle = animation_idle.get_frame();
        IMAGE* frame_explode = animation_explode.get_frame();
        explode_render_offset = {
            (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f,
            (frame_idle->getheight() - frame_explode->getheight()) / 2.0f
        };
    }

    ~SunBulletEx() override = default;

    void on_collide() override {
        Bullet::on_collide();

        main_camera.shake(18, 350);

        mciSendString(_T("play sun_explode_ex from 0"), nullptr, 0, nullptr);
    }

    bool check_collision(const Vector2& pos, const Vector2& p_size) override {
        return max(position.x + size.x, pos.x + p_size.x) - min(position.x, pos.x) <= size.x + p_size.x and
                max(position.y + size.y, pos.y + p_size.y) - min(position.y, pos.y) <= size.y + p_size.y;
    }

    void on_update(int delta) override {
        if (valid) position += velocity * (float)delta;
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
    }

private:
    Animation animation_idle;       // 巨大日光炸弹默认动画
    Animation animation_explode;    // 巨大日光炸弹爆炸动画
    Vector2 explode_render_offset;  // 爆炸动画渲染偏移
};

#endif //PLANTSVSPLANTS_SUN_BULLET_EX_H
