#ifndef PLANTSVSPLANTS_TIMER_H
#define PLANTSVSPLANTS_TIMER_H

#include <functional>
#include <utility>

// 定时器类
class Timer {
public:
    Timer() = default;
    ~Timer() = default;

    // 重置定时器
    void restart() {
        pass_time = 0;
        shotted = false;
    }

    // 设置等待时间
    void set_wait_time(int v) {
        wait_time = v;
    }

    // 设置触发次数
    // true -> 单次触发 | false -> 多次触发
    void set_one_shot(bool flag) {
        one_shot = flag;
    }

    // 设置回调函数
    void set_callback(std::function<void()> func) {
        callback = std::move(func);
    }

    // 暂停
    void pause() {
        paused = true;
    }

    // 继续
    void resume() {
        paused = false;
    }

    // 定时器的帧更新
    void on_update(int delta) {
        // 暂停无需更新
        if (paused) return;
        // 增加已过时间
        pass_time += delta;
        // 已过时间大于等于需要等待的时间
        if (pass_time >= wait_time) {
            // 尝试触发回调函数, 并设置已触发标志, 重置已过时间
            if ((not one_shot or not shotted) and callback) callback();
            shotted = true;
            pass_time = 0;
        }
    }

private:
    int pass_time = 0;                  // 已过时间
    int wait_time = 0;                  // 等待时间
    bool paused = false;                // 是否暂停
    bool shotted = false;               // 是否触发
    bool one_shot = false;              // 单次触发
    std::function<void()> callback;     // 回调函数
};

#endif //PLANTSVSPLANTS_TIMER_H
