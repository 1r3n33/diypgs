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
        // Update balls position. Compute balls collision.
        // Return the number of active balls.
        uint8_t updateBalls();

        // Update bonus position. Check collision. Apply effect.
        // Must happen before calls to update*Paddle functions.
        void updateBonus();

        // Read controller input to update left paddle.
        void updateLeftPaddle();

        // Make the right paddle follow the closest ball.
        void updateRightPaddle();

    private:
        static constexpr uint8_t MAX_BALL_COUNT = 4;

        Ball balls[MAX_BALL_COUNT];
        CircularBuffer ballPositions[MAX_BALL_COUNT];

        Bonus bonus;
        Paddle left;
        Paddle right;
    };
}
