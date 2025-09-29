#ifndef PLANTSVSPLANTS_GLOOMSHROOM_PLAYER_H
#define PLANTSVSPLANTS_GLOOMSHROOM_PLAYER_H

#include "player.h"

class GloomshroomPlayer : public Player {
public:
    explicit GloomshroomPlayer(bool facing_right = true) : Player(facing_right) {

    }

    ~GloomshroomPlayer() override = default;

    void on_update(int delta) override {

    }
    void on_attack() override {

    }
    void on_attack_ex() override {

    }
private:
};

#endif //PLANTSVSPLANTS_GLOOMSHROOM_PLAYER_H