#ifndef PLANTSVSPLANTS_ANIMATION_H
#define PLANTSVSPLANTS_ANIMATION_H

#include "util.h"
#include "atlas.h"
#include "camera.h"

#include <graphics.h>
#include <functional>

// 动画类
class Animation {
public:
    Animation() = default;
    ~Animation() = default;

    // 重置动画状态
    void reset() {
        timer = 0;
        idx_frame = 0;
    }

    // 设置动画图集
    void set_atlas(Atlas* new_atlas) {
        reset();
        atlas = new_atlas;
    }

    // 设置是否循环动画
    void set_loop(bool flag) {
        is_loop = flag;
    }

    // 设置动画帧间隔
    void set_interval(int ms) {
        interval = ms;
    }

    // 设置回调函数
    void set_callback(std::function<void()> func) {
        callback = func;
    }

    // 获取动画帧索引
    int get_idx_frame() {
        return idx_frame;
    }

    // 获取动画帧图片
    IMAGE* get_frame() {
        return atlas->get_image(idx_frame);
    }

    // 判断动画是否播放完毕
    bool check_finished() {
        // 如果动画循环播放永远不会播放完毕, 始终返回false
        // 否则判断当前动画帧索引是否是图集的最后一张图片, 如果是返回true
        return is_loop ? false : (idx_frame == atlas->get_size() - 1);
    }

    // 动画帧更新
    void on_update(int delta) {
        // 每次更新时累加计时器的值
        timer += delta;
        // 判断是否更新下一章图片
        if (timer >= interval) {
            // 更新 -> 重置定时器, 并让帧索引加1
            timer = 0;
            ++idx_frame;
            // 确定帧索引的实际值
            // 如果帧索引没到图集的最后一张图片 -> 正常加1即可
            // 如果到达图集最后一张图片判断是否可循环播放该动画
            // 循环播放 -> 从第0个索引开始重新播放
            // 不可循环 -> 将帧索引锁定为图集的最后一张图片, 若存在回调函数执行回调函数
            if (idx_frame >= atlas->get_size()) {
                idx_frame = is_loop ? 0 : atlas->get_size() - 1;
                if (not is_loop and callback) callback();
            }
        }
    }

    // 绘图
    void on_draw(const Camera& camera, int x, int y) const {
        putimage_alpha(camera, x, y, atlas->get_image(idx_frame));
    }

private:
    int timer = 0;                  // 计时器
    int interval = 0;               // 帧间隔
    int idx_frame = 0;              // 帧索引
    bool is_loop = true;            // 是否循环
    Atlas* atlas = nullptr;         // 图集
    std::function<void()> callback; // 回调函数
};

#endif //PLANTSVSPLANTS_ANIMATION_H
