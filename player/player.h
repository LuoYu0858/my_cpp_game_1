#ifndef PLANTSVSPLANTS_PLAYER_H
#define PLANTSVSPLANTS_PLAYER_H

#include "camera.h"
#include "vector2.h"
#include "platform.h"
#include "particle.h"
#include "animation.h"
#include "player_id.h"
#include "global_variable.h"

#include <graphics.h>

// 玩家基类
class Player {
public:
    Player() {
        current_animation = &animation_idle_right;

        timer_attack_cd.set_wait_time(attack_cd);
        timer_attack_cd.set_one_shot(true);
        timer_attack_cd.set_callback([&]() { can_attack = true; });

        timer_invulnerable.set_wait_time(750);
        timer_invulnerable.set_one_shot(true);
        timer_invulnerable.set_callback([&]() { is_invulnerable = false; });

        timer_invulnerable_blink.set_wait_time(75);
        timer_invulnerable_blink.set_callback([&]() {
            is_showing_sketch_frame = not is_showing_sketch_frame;
        });

        timer_run_effect_generation.set_wait_time(100);
        timer_run_effect_generation.set_callback([&]() {
            IMAGE* frame = atlas_run_effect.get_image(0);
            Vector2 particle_position{
                position.x + (size.x - frame->getwidth()) / 2,
                position.y + size.y - frame->getheight()
            };
            particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
        });

        timer_die_effect_generation.set_wait_time(45);
        timer_die_effect_generation.set_callback([&]() {
            IMAGE* frame = atlas_run_effect.get_image(0);
            Vector2 particle_position{
                position.x + (size.x - frame->getwidth()) / 2,
                position.y + size.y - frame->getheight()
            };
            particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
        });

        animation_jump_effect.set_atlas(&atlas_jump_effect);
        animation_jump_effect.set_interval(25);
        animation_jump_effect.set_loop(false);
        animation_jump_effect.set_callback([&]() { is_jump_effect_visible = false; });

        animation_land_effect.set_atlas(&atlas_land_effect);
        animation_land_effect.set_interval(25);
        animation_land_effect.set_loop(false);
        animation_land_effect.set_callback([&]() { is_land_effect_visible = false; });
    }

    virtual ~Player() = default;

    virtual void on_update(int delta) {
        // 判断玩家移动方向
        int direction = is_right_key_down - is_left_key_down;
        if (direction != 0) {
            if (not is_attacking_ex) is_facing_right = direction > 0;
            // 玩家正在移动
            is_facing_right = direction > 0;
            current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
            float distance = (float)direction * run_velocity * (float)delta;  // 移动距离: 移动方向 * 速度 * 时间
            on_run(distance);
        } else {
            // 玩家没有移动
            current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
            timer_run_effect_generation.pause();
        }
        if (is_attacking_ex)
            current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;

        current_animation->on_update(delta);

        animation_jump_effect.on_update(delta);
        animation_land_effect.on_update(delta);

        timer_attack_cd.on_update(delta);
        timer_invulnerable.on_update(delta);
        timer_invulnerable_blink.on_update(delta);
        timer_run_effect_generation.on_update(delta);

        if (hp <= 0) timer_die_effect_generation.on_update(delta);

        erase_if(particle_list, [](const Particle& particle) {
            return not particle.check_valid();
        });

        for (auto& particle : particle_list) particle.on_update(delta);

        if (is_showing_sketch_frame) sketch_image(current_animation->get_frame(), &img_sketch);

        move_and_collide(delta);
    }

    virtual void on_draw(const Camera& camera) {
        if (is_jump_effect_visible)
            animation_jump_effect.on_draw(camera, (int)position_jump_effect.x, (int)position_jump_effect.y);
        if (is_land_effect_visible)
            animation_land_effect.on_draw(camera, (int)position_land_effect.x, (int)position_land_effect.y);
        for (auto& particle : particle_list) particle.on_draw(camera);
        if (hp > 0 and is_invulnerable and is_showing_sketch_frame)
            putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
        else
            current_animation->on_draw(camera, (int)position.x, (int)position.y);
        if (is_debug) {
            setlinecolor(RGB(0, 125, 255));
            rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
        }
    }

    virtual void on_input(const ExMessage& msg) {
        switch (msg.message) {
            case WM_KEYDOWN:
                switch (id) {
                    case PlayerID::P1:
                        switch (msg.vkcode) {
                            case 0x41:
                                is_left_key_down = true;
                                break;
                            case 0x44:
                                is_right_key_down = true;
                                break;
                            case 0x57:
                                on_jump();
                                break;
                            case 0x46:
                                if (can_attack) {
                                    on_attack();
                                    can_attack = false;
                                    timer_attack_cd.restart();
                                }
                                break;
                            case 0x47:
                                if (mp >= 100) {
                                    on_attack_ex();
                                    mp = 0;
                                }
                                break;
                            default: break;
                        }
                        break;
                    case PlayerID::P2:
                        switch (msg.vkcode) {
                            case VK_LEFT:
                                is_left_key_down = true;
                                break;
                            case VK_RIGHT:
                                is_right_key_down = true;
                                break;
                            case VK_UP:
                                on_jump();
                                break;
                            case VK_OEM_PERIOD:
                                if (can_attack) {
                                    on_attack();
                                    can_attack = false;
                                    timer_attack_cd.restart();
                                }
                                break;
                            case VK_OEM_2:
                                if (mp >= 100) {
                                    on_attack_ex();
                                    mp = 0;
                                }
                                break;
                            default: break;
                        }
                        break;
                    default: break;
                }
                break;
            case WM_KEYUP:
                switch (id) {
                    case PlayerID::P1:
                        switch (msg.vkcode) {
                            case 0x41:
                                is_left_key_down = false;
                                break;
                            case 0x44:
                                is_right_key_down = false;
                                break;
                            default: break;
                        }
                        break;
                    case PlayerID::P2:
                        switch (msg.vkcode) {
                            case VK_LEFT:
                                is_left_key_down = false;
                                break;
                            case VK_RIGHT:
                                is_right_key_down = false;
                            default: break;
                        }
                        break;
                    default: break;
                }
                break;
            default: break;
        }
    }

    virtual void on_run(float distance) {
        if (is_attacking_ex) return;
        position.x += distance;
        timer_run_effect_generation.resume();
    }

    virtual void on_jump() {
        // 只有竖直方向速度为0且不处于特殊攻击状态才可跳跃
        if (velocity.y == 0 and not is_attacking_ex) velocity.y += jump_velocity;
        is_jump_effect_visible = true;
        animation_jump_effect.reset();

        IMAGE* effect_frame = animation_jump_effect.get_frame();
        position_jump_effect = {
            position.x + (size.x - effect_frame->getwidth()) / 2,
            position.y + size.y - effect_frame->getheight()
        };
    }

    virtual void on_land() {
        is_land_effect_visible = true;
        animation_land_effect.reset();

        IMAGE* effect_frame = animation_land_effect.get_frame();
        position_land_effect = {
            position.x + (size.x - effect_frame->getwidth()) / 2,
            position.y + size.y - effect_frame->getheight()
        };
    }

    virtual void on_attack() = 0;

    virtual void on_attack_ex() = 0;

    // 进入无敌状态
    void make_invulnerable() {
        is_invulnerable = true;
        timer_invulnerable.restart();
    }

    void set_id(PlayerID player_id) {
        id = player_id;
    }

    void set_position(float x, float y) {
        position.x = x, position.y = y;
    }

    [[nodiscard]] const Vector2& get_position() const {
        return position;
    }

    [[nodiscard]] const Vector2& get_size() const {
        return size;
    }

    [[nodiscard]] int get_hp() const {
        return hp;
    }

    [[nodiscard]] int get_mp() const {
        return mp;
    }

protected:
    void move_and_collide(int delta) {
        float last_velocity_y = velocity.y;

        velocity.y += gravity * (float)delta;   // G = g * time
        position += velocity * (float)delta;    // 将这一帧的位置按照移动的速度更新

        // 单向平台碰撞
        // 玩家可以从平台下向上跳跃到平台上, 但是无法穿过平台
        // 检测玩家下落过程数据即可
        if (velocity.y > 0) {
            // 遍历场景中所有平台对象, 检测是否有玩家与平台发生碰撞
            for (const auto& platform : platform_list) {
                // 获取每一个平台的碰撞外形
                const auto& [y, left, right] = platform.shape;
                /*
                                      |-----|     <- 玩家
                                      20   25

                 |---------------|    <- 平台
                 0              15

                 max() -> (15, 25) = 25  min() -> (0, 20) = 0
                 max() - min() = 25
                 宽度: (15 - 0) + (25 - 20) = 20
                 因为 25 > 20 因此玩家与平台水平方向未发生碰撞


                              |-----|     <- 玩家
                             13    18

                 |---------------|    <- 平台
                 0              15

                 max() -> (15, 18) = 18  min() -> (0, 13) = 0
                 max() - min() = 18
                 宽度: (15 - 0) + (18 - 13) = 20
                 因为 18 <= 20 因此玩家与平台水平方向发生碰撞

                 */
                // is_collide_x 是否在水平方向发生碰撞:
                // position.x -> 玩家碰撞箱(矩形)最左上角坐标
                // max(): 玩家的position.x + size.x -> 玩家碰撞箱最右侧的水平坐标和平台碰撞外形最右侧坐标比较, 选大值
                // min(): 玩家碰撞箱最左侧的水平坐标和平台碰撞外形最左侧坐标比较, 选小值
                // max() - min(): 玩家宽度与平台宽度的二位投影
                // 如果max() - min()的值小于角色宽度 + 平台宽度, 则二者水平方向一定重合
                bool is_collide_x = (
                    max(position.x + size.x, right) -
                    min(position.x, left) <= size.x + (right - left)
                );
                bool is_collide_y = (y >= position.y and y <= position.y + size.y);

                // 当玩家水平和竖直方向碰撞逻辑均满足, 才可判定发生碰撞
                if (is_collide_x and is_collide_y) {
                    // 计算发生碰撞的这一帧玩家竖直方向移动距离
                    float delta_pos_y = velocity.y * (float)delta;
                    // 计算玩家上一帧的脚底位置
                    float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
                    // 如果玩家上一帧整个碰撞箱穿过平台, 这一帧将玩家放在平台上
                    if (last_tick_foot_pos_y <= y) {
                        position.y = y - size.y;
                        velocity.y = 0;
                        // 前一帧竖直速度不为0, 当前帧竖直速度为0, 那么这一帧玩家落到平台上
                        if (last_velocity_y != 0) on_land();
                        break;
                    }
                }
            }
        }
        // 玩家与子弹碰撞监测
        if (not is_invulnerable) {
            for (auto bullet : bullet_list) {
                if (not bullet->get_valid() or bullet->get_collide_target() != id) continue;
                if (bullet->check_collision(position, size)) {
                    make_invulnerable();
                    bullet->on_collide();
                    bullet->set_valid(false);
                    hp -= bullet->get_damage();
                }
            }
        }
    }

protected:
    const float gravity = 1.6e-3f;          // 玩家重力
    const float run_velocity = .6f;         // 跑动速度
    const float jump_velocity = -.75f;      // 跳跃速度
protected:
    int hp = 100;                           // 角色生命值
    int mp = 0;                             // 角色能量

    int attack_cd = 500;                    // 玩家普通攻击冷却时间(ms)

    Vector2 size;                           // 角色尺寸
    Vector2 position;                       // 角色位置
    Vector2 velocity;                       // 角色速度

    Vector2 position_jump_effect;           // 跳跃动画播放位置
    Vector2 position_land_effect;           // 落地动画播放位置

    Animation animation_idle_left;          // 朝向左的默认动画
    Animation animation_idle_right;         // 朝向右的默认动画
    Animation animation_run_left;           // 朝向左的奔跑动画
    Animation animation_run_right;          // 朝向右的奔跑动画
    Animation animation_attack_ex_left;     // 朝向左的特殊攻击动画
    Animation animation_attack_ex_right;    // 朝向右的特殊攻击动画
    Animation animation_jump_effect;        // 跳跃特效动画
    Animation animation_land_effect;        // 落地特效动画

    Animation* current_animation = nullptr; // 当前正在播放的动画

    PlayerID id = PlayerID::P1;             // 玩家ID序号

    bool is_left_key_down = false;          // 向左移动按键是否按下
    bool is_right_key_down = false;         // 向右移动按键是否按下

    bool is_facing_right = true;            // 角色是否面向右侧

    bool can_attack = true;                 // 是否可以释放普通攻击

    bool is_attacking_ex = false;           // 是否正在释放特殊攻击

    bool is_invulnerable = false;           // 角色是否处于无敌状态
    bool is_showing_sketch_frame = false;   // 当前帧是否应该显示剪影

    bool is_jump_effect_visible = false;    // 跳跃动画是否可见
    bool is_land_effect_visible = false;    // 落地动画是否可见

    Timer timer_attack_cd;                  // 普通攻击冷却时间定时器

    Timer timer_invulnerable;               // 无敌状态定时器
    Timer timer_invulnerable_blink;         // 无敌状态闪烁定时器

    Timer timer_run_effect_generation;      // 跑动特效粒子发射定时器
    Timer timer_die_effect_generation;      // 死亡特效粒子发射定时器

    IMAGE img_sketch;                       // 动画帧剪影图片

    std::vector<Particle> particle_list;    // 粒子对象列表
};

#endif //PLANTSVSPLANTS_PLAYER_H
