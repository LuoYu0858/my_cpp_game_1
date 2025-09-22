#ifndef PLANTSVSPLANTS_BULLET_H
#define PLANTSVSPLANTS_BULLET_H

#include <easyx.h>

#include "camera.h"
#include "vector2.h"
#include "player_id.h"

#include <functional>

// 子弹基类
class Bullet {
public:
    Bullet() = default;
    ~Bullet() = default;

    int get_damage() {
        return damage;
    }

    void set_damage(int val) {
        damage = val;
    }


    [[nodiscard]] Vector2 get_position() const {
        return position;
    }

    void set_position(float x, float y) {
        position.x = x, position.y = y;
    }

    [[nodiscard]] const Vector2& get_size() const {
        return size;
    }

    void set_velocity(float x, float y) {
        velocity.x = x, velocity.y = y;
    }

    [[nodiscard]] PlayerID get_collide_target() const {
        return target_id;
    }

    void set_collide_target(PlayerID id) {
        target_id = id;
    }

    void set_callback(std::function<void()> func) {
        callback = func;
    }

    bool get_valid() const {
        return valid;
    }

    void set_valid(bool flag) {
        valid = flag;
    }

    bool check_can_remove() const {
        return can_remove;
    }

    virtual void on_collide() {
        if (callback) callback();
    }

    // 子弹是否与玩家发生碰撞
    virtual bool check_collision(const Vector2& pos, const Vector2& size) {
        return this->position.x + this->size.x / 2 >= pos.x
                and this->position.x + this->size.x / 2 <= pos.x + size.x
                and this->position.y + this->size.y / 2 >= pos.y
                and this->position.y + this->size.y / 2 <= pos.y + size.y;
    }

    virtual void on_update(int delta) {}

    virtual void on_draw(const Camera& camera) const {}

protected:
    // 检测子弹是否飞出屏幕
    // 子弹边界是否全部位于屏幕边界外
    bool check_if_exceeds_screen() {
        return position.x + size.x <= 0 or position.x >= getwidth()
                or position.y + size.y <= 0 or position.y >= getheight();
    }

protected:
    Vector2 size;                       // 子弹尺寸
    Vector2 position;                   // 子弹位置
    Vector2 velocity;                   // 子弹速度

    int damage = 10;                    // 子弹伤害

    bool valid = true;                  // 子弹是否有效
    bool can_remove = false;            // 子弹是否可以被删除

    std::function<void()> callback;     // 子弹碰撞回调函数

    PlayerID target_id = PlayerID::P1;  // 子弹碰撞目标玩家ID
};

#endif //PLANTSVSPLANTS_BULLET_H
