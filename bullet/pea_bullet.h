#ifndef PLANTSVSPLANTS_PEA_BULLET_H
#define PLANTSVSPLANTS_PEA_BULLET_H

#include "bullet.h"
#include "animation.h"
#include "global_variable.h"

class PeaBullet : public Bullet {
public:
    PeaBullet() {
        size.x = 64, size.y = 64;

        damage = 8;

        animation_break.set_atlas(&atlas_pea_break);
        animation_break.set_interval(100);
        animation_break.set_loop(false);
        // 破碎动画播放结束后, 设置可被移除状态为true
        animation_break.set_callback([&]() { can_remove = true; });
    }

    ~PeaBullet() override = default;

    void on_collide() override {
        Bullet::on_collide();
        switch (rand() % 3) {
            case 0:
                mciSendString(_T("play pea_break_1 from 0"), nullptr, 0, nullptr);
                break;
            case 1:
                mciSendString(_T("play pea_break_2 from 0"), nullptr, 0, nullptr);
                break;
            case 2:
                mciSendString(_T("play pea_break_3 from 0"), nullptr, 0, nullptr);
                break;
            default: break;
        }
    }

    void on_update(int delta) override {
        position += velocity * (float)delta;    // 修改子弹位置
        if (not valid) animation_break.on_update(delta);    // 子弹失效更新破碎动画
        if (check_if_exceeds_screen()) can_remove = true;
    }

    void on_draw(const Camera& camera) const override {
        if (valid) putimage_alpha(camera, (int)position.x, (int)position.y, &img_pea);
        else animation_break.on_draw(camera, (int)position.x, (int)position.y);
    }

private:
    Animation animation_break;  // 豌豆子弹破碎动画
};

#endif //PLANTSVSPLANTS_PEA_BULLET_H
