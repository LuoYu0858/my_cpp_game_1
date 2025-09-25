#ifndef PLANTSVSPLANTS_SUNFLOWER_PLAYER_H
#define PLANTSVSPLANTS_SUNFLOWER_PLAYER_H

#include "player.h"
#include "sun_bullet.h"
#include "sun_bullet_ex.h"
#include "global_variable.h"

class SunflowerPlayer : public Player {
public:
    explicit SunflowerPlayer(bool facing_right = true) : Player(facing_right) {
        animation_idle_left.set_atlas(&atlas_sunflower_idle_left);
        animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
        animation_run_left.set_atlas(&atlas_sunflower_run_left);
        animation_run_right.set_atlas(&atlas_sunflower_run_right);
        animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_left);
        animation_attack_ex_right.set_atlas(&atlas_sunflower_attack_ex_right);
        animation_sun_text.set_atlas(&atlas_sun_text);
        animation_die_left.set_atlas(&atlas_sunflower_die_left);
        animation_die_right.set_atlas(&atlas_sunflower_die_right);

        animation_idle_left.set_interval(100);
        animation_idle_right.set_interval(100);
        animation_run_left.set_interval(100);
        animation_run_right.set_interval(100);
        animation_attack_ex_left.set_interval(100);
        animation_attack_ex_right.set_interval(100);
        animation_sun_text.set_interval(100);
        animation_die_left.set_interval(175);
        animation_die_right.set_interval(175);

        animation_die_left.set_loop(false);
        animation_die_right.set_loop(false);
        animation_attack_ex_left.set_loop(false);
        animation_attack_ex_right.set_loop(false);
        animation_sun_text.set_loop(false);

        animation_attack_ex_left.set_callback([&]() {
            is_attacking_ex = false;
            is_sun_text_visible = false;
        });

        animation_attack_ex_right.set_callback([&]() {
            is_attacking_ex = false;
            is_sun_text_visible = false;
        });

        size.x = 96, size.y = 96;

        attack_cd = 250;
    };
    ~SunflowerPlayer() override = default;

    void on_update(int delta) override {
        Player::on_update(delta);

        if (is_sun_text_visible) animation_sun_text.on_update(delta);
    }

    void on_draw(const Camera& camera) override {
        Player::on_draw(camera);

        if (is_sun_text_visible) {
            IMAGE* frame = animation_sun_text.get_frame();
            Vector2 text_position = {
                position.x - (size.x - static_cast<float>(frame->getwidth())) / 2,
                position.y - static_cast<float>(frame->getheight())
            };
            animation_sun_text.on_draw(camera, (int)text_position.x, (int)text_position.y);
        }
    }

    void on_attack() override {
        Bullet* bullet = new SunBullet();

        const Vector2& bullet_size = bullet->get_size();
        Vector2 bullet_position{
            position.x + (size.x - bullet_size.x) / 2, position.y
        };

        bullet->set_position(bullet_position.x, bullet_position.y);
        bullet->set_velocity(is_facing_right ? velocity_sun.x : -velocity_sun.x, velocity_sun.y);
        bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
        bullet->set_callback([&]() { mp += 35; });
        bullet_list.push_back(bullet);
    }

    void on_attack_ex() override {
        is_attacking_ex = true;
        is_sun_text_visible = true;

        animation_sun_text.reset();
        is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

        Bullet* bullet = new SunBulletEx();
        Player* target_player = id == PlayerID::P1 ? player_2 : player_1;

        const Vector2& bullet_size = bullet->get_size();
        const Vector2& target_size = target_player->get_size();
        const Vector2& target_position = target_player->get_position();

        Vector2 bullet_position{target_position.x + (target_size.x - bullet_size.x) / 2, -size.y};
        Vector2 bullet_velocity{0, speed_sun_ex};

        bullet->set_position(bullet_position.x, bullet_position.y);
        bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);
        bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
        bullet->set_callback([&]() { mp += 50; });

        bullet_list.push_back(bullet);

        mciSendString(_T("play sun_text from 0"), nullptr, 0, nullptr);
    }

private:
    const float speed_sun_ex = .15f;                // 大型日光炸弹下落速度
    const Vector2 velocity_sun = {.2f, -.7f};   // 小型日光炸弹抛射速度

private:
    Animation animation_sun_text;       // 头顶文本动画
    bool is_sun_text_visible = false;   // 是否显示头顶文本
};

#endif //PLANTSVSPLANTS_SUNFLOWER_PLAYER_H
