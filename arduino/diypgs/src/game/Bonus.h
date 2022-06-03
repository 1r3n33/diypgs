#pragma once

#include "../hw/Pcd8544.h"

#include "../sdk/Vector.h"

namespace game
{
    class Bonus
    {
    public:
        enum State : uint8_t
        {
            DISABLED = 0,
            FADE_IN = 1,
            ENABLED = 2,
            FADE_OUT = 3
        };

        enum class Effect : uint8_t
        {
            INVERT_COMMAND_LEFT,  // Current effect of UP and DOWN buttons is inverted. (Left player)
            INVERT_COMMAND_RIGHT, // Current effect of UP and DOWN buttons is inverted. (Right player)
            ACCELERATE_BALL,      // Accelerate the ball that captured the bonus.
            MULTI_BALL,           // Spawn an additional ball. (Max. is Game::MAX_BALL_COUNT)
            CHANGE_SIZE_LEFT,     // Change size of the left paddle.
            CHANGE_SIZE_RIGHT,    // Change size of the right paddle.
            COUNT,
        };

        sdk::Vec2f16 center;
        uint8_t state;

    private:
        sdk::Vec2f16 origin;
        sdk::Vec2f16 dxdy;

        uint8_t switch_state_counter;
        uint8_t sprite_ids[4];
        hw::Pcd8544::Sprite sprites[4];

    public:
        void setup();
        void update();

        Effect capture();
    };
}
