#ifndef PLANTSVSPLANTS_PEASHOOTER_PLAYER_H
#define PLANTSVSPLANTS_PEASHOOTER_PLAYER_H

#include "player.h"
#include "global_variable.h"

#include <iostream>

class PeashooterPlayer : public Player {
public:
    PeashooterPlayer() {
        animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
        animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
        animation_run_left.set_atlas(&atlas_peashooter_run_left);
        animation_run_right.set_atlas(&atlas_peashooter_run_right);

        animation_idle_left.set_interval(100);
        animation_idle_right.set_interval(100);
        animation_run_left.set_interval(100);
        animation_run_right.set_interval(100);

        size.x = 96, size.y = 96;
    }

    ~PeashooterPlayer() override = default;

    void on_update(int delta) override {
        Player::on_update(delta);
        std::cout << "豌豆射手正在更新" << std::endl;
    }

private:
};

#endif //PLANTSVSPLANTS_PEASHOOTER_PLAYER_H
