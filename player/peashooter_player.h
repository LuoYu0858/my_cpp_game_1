#ifndef PLANTSVSPLANTS_PEASHOOTER_PLAYER_H
#define PLANTSVSPLANTS_PEASHOOTER_PLAYER_H

#include "player.h"
#include "pea_bullet.h"
#include "global_variable.h"

class PeashooterPlayer : public Player {
public:
    explicit PeashooterPlayer(bool facing_right = true) : Player(facing_right) {
        animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
        animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
        animation_run_left.set_atlas(&atlas_peashooter_run_left);
        animation_run_right.set_atlas(&atlas_peashooter_run_right);
        animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
        animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);
        animation_die_left.set_atlas(&atlas_peashooter_die_left);
        animation_die_right.set_atlas(&atlas_peashooter_die_right);

        animation_idle_left.set_interval(100);
        animation_idle_right.set_interval(100);
        animation_run_left.set_interval(100);
        animation_run_right.set_interval(100);
        animation_attack_ex_left.set_interval(100);
        animation_attack_ex_right.set_interval(100);
        animation_die_left.set_interval(175);
        animation_die_right.set_interval(175);

        animation_die_left.set_loop(false);
        animation_die_right.set_loop(false);

        size.x = 96, size.y = 96;

        timer_attack_ex.set_wait_time(attack_ex_duration);
        timer_attack_ex.set_one_shot(true);
        timer_attack_ex.set_callback([&]() { is_attacking_ex = false; });

        timer_spawn_pea_ex.set_wait_time(100);
        timer_spawn_pea_ex.set_callback([&]() { spawn_pea_bullet(speed_pea_ex); });

        attack_cd = 100;
    }

    ~PeashooterPlayer() override = default;

    void on_update(int delta) override {
        Player::on_update(delta);

        if (is_attacking_ex) {
            main_camera.shake(3, 100);
            timer_attack_ex.on_update(delta);
            timer_spawn_pea_ex.on_update(delta);
        }
    }

    void on_attack() override {
        spawn_pea_bullet(speed_pea);

        switch (rand() % 2) {
            case 0:
                mciSendString(_T("play pea_shoot_1 from 0"), nullptr, 0, nullptr);
                break;
            case 1:
                mciSendString(_T("play pea_shoot_2 from 0"), nullptr, 0, nullptr);
                break;
            default: break;
        }
    }

    void on_attack_ex() override {
        is_attacking_ex = true;
        timer_attack_ex.restart();

        is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

        mciSendString(_T("play pea_shoot_ex from 0"), nullptr, 0, nullptr);
    }

private:
    void spawn_pea_bullet(float speed) {
        Bullet* bullet = new PeaBullet();
        const Vector2& bullet_size = bullet->get_size();

        // 根据当前角色朝向, 让子弹生成在角色面朝方向的角色图像边缘
        Vector2 bullet_position = {
            is_facing_right ? position.x + size.x - bullet_size.x / 2 : position.x - bullet_size.x / 2, position.y
        };

        Vector2 bullet_velocity = {is_facing_right ? speed : -speed, 0};
        bullet->set_position(bullet_position.x, bullet_position.y);
        bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);
        bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
        bullet->set_callback([&]() { mp += 15; });

        bullet_list.push_back(bullet);
    }

private:
    const float speed_pea = .9f;            // 普通攻击豌豆子弹速度
    const float speed_pea_ex = 1.9f;        // 特殊攻击豌豆子弹速度
    const int attack_ex_duration = 2500;    // 特殊攻击持续时间

private:
    Timer timer_attack_ex;      // 特殊攻击状态定时器
    Timer timer_spawn_pea_ex;   // 豌豆子弹发射定时器
};

#endif //PLANTSVSPLANTS_PEASHOOTER_PLAYER_H
