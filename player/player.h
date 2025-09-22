#ifndef PLANTSVSPLANTS_PLAYER_H
#define PLANTSVSPLANTS_PLAYER_H

#include "camera.h"
#include "vector2.h"
#include "platform.h"
#include "animation.h"
#include "player_id.h"
#include "global_variable.h"

#include <graphics.h>

// 玩家基类
class Player {
public:
    Player() {
        current_animation = &animation_idle_right;
    }

    virtual ~Player() = default;

    virtual void on_update(int delta) {
        // 判断玩家移动方向
        int direction = is_right_key_down - is_left_key_down;
        if (direction != 0) {
            // 玩家正在移动
            is_facing_right = direction > 0;
            current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
            float distance = (float)direction * run_velocity * (float)delta;  // 移动距离: 移动方向 * 速度 * 时间
            on_run(distance);
        } else {
            // 玩家没有移动
            current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
        }
        current_animation->on_update(delta);

        move_and_collide(delta);
    }

    virtual void on_draw(const Camera& camera) {
        current_animation->on_draw(camera, (int)position.x, (int)position.y);
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
        position.x += distance;
    }

    virtual void on_jump() {
        // 只有竖直方向速度为0才可跳跃
        if (velocity.y == 0) velocity.y += jump_velocity;
    }

    void set_id(PlayerID player_id) {
        id = player_id;
    }

    void set_position(float x, float y) {
        position.x = x, position.y = y;
    }

protected:
    void move_and_collide(int delta) {
        velocity.y += gravity * (float)delta;   // G = g * time
        position += velocity * (float)delta;    // 将这一帧的位置按照移动的速度更新

        // 单项平台碰撞
        // 玩家可以从平台下向上跳跃到平台上, 但是无法转过平台
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
                        break;
                    }
                }
            }
        }
    }

protected:
    const float gravity = 1.6e-3f;          // 玩家重力
    const float run_velocity = .45f;        // 跑动速度
    const float jump_velocity = -.75f;      // 跳跃速度
protected:
    Vector2 size;                           // 角色尺寸
    Vector2 position;                       // 角色位置
    Vector2 velocity;                       // 角色速度

    Animation animation_idle_left;          // 朝向左的默认动画
    Animation animation_idle_right;         // 朝向右的默认动画
    Animation animation_run_left;           // 朝向左的奔跑动画
    Animation animation_run_right;          // 朝向右的奔跑动画

    Animation* current_animation = nullptr; // 当前正在播放的动画

    PlayerID id = PlayerID::P1;             // 玩家ID序号

    bool is_left_key_down = false;          // 向左移动按键是否按下
    bool is_right_key_down = false;         // 向右移动按键是否按下

    bool is_facing_right = true;            // 角色是否面向右侧
};

#endif //PLANTSVSPLANTS_PLAYER_H
