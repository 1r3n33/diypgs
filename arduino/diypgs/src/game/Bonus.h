#pragma once

#include "../sdk/pcd8544.h"

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
            INVERT_COMMAND_LEFT,
            INVERT_COMMAND_RIGHT,
            ACCELERATE_BALL,
            COUNT,
        };

        int16_t center_x;
        int16_t center_y;
        uint8_t state;

    private:
        int16_t origin_x;
        int16_t origin_y;
        int16_t dx;
        int16_t dy;

        uint8_t switch_state_counter;
        uint8_t sprite_ids[4];
        PCD8544::sprite_t sprites[4];

    public:
        void setup();
        void update();

        Effect capture();
    };
}
