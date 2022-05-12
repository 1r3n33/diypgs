#pragma once

#include "../sdk/pcd8544.h"

namespace game
{
    class Bonus
    {
    private:
        uint8_t state;
        int8_t x;
        int8_t y;
        uint8_t sprite_ids[4];
        PCD8544::sprite_t sprites[4];

    public:
        void setup();
        void update();

        void on();
        void off();
    };
}
