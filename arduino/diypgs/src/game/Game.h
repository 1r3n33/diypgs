#pragma once

#include "World.h"

namespace game
{
    class Game : public World
    {
    public:
        Game(const hw::Pcd8544 &graphics, const hw::Buzzer &audio, const hw::Gamepad &controller);

    public:
        void setup() override;
        World::State update() override;

    private:
        Ball ball;
        Bonus bonus;
        CircularBuffer ball_pos;
        Paddle left;
        Paddle right;
    };
}
