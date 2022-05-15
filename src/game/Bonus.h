#pragma once

#include "../sdk/pcd8544.h"

namespace game
{
    class Bonus
    {
    public:
        enum State : uint8_t
        {
            ENABLED = 1,
        };

        enum class Effect : uint8_t
        {
            INVERT_COMMAND_LEFT,
            INVERT_COMMAND_RIGHT,
            ACCELERATE_BALL,
            COUNT,
        };

        int8_t x;
        int8_t y;
        uint8_t state;

    private:
        uint8_t switch_state_counter;
        uint8_t sprite_ids[4];
        PCD8544::sprite_t sprites[4];

    public:
        void setup();
        void update();

        void on();
        void off();

        Effect getEffect() const;
    };
}
