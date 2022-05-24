#pragma once

#include "../hw/Buzzer.h"
#include "../hw/Gamepad.h"
#include "../hw/Pcd8544.h"

#include "ball.h"
#include "Bonus.h"
#include "circular_buffer.h"
#include "paddle.h"

namespace game
{
    class World
    {
    public:
        World(const hw::Pcd8544 &graphics, const hw::Buzzer &audio, const hw::Gamepad &controller);

    public:
        void setup();
        void update();

    private:
        const hw::Pcd8544 &graphics;
        const hw::Buzzer &audio;
        const hw::Gamepad &controller;

    private:
        Ball ball;
        Bonus bonus;
        CircularBuffer ball_pos;
        Paddle left;
        Paddle right;
    };
}
