#pragma once

#include "../hw/Pcd8544.h"

namespace game
{
    class Paddle
    {
    private:
        enum Size : uint8_t
        {
            SMALL = 0,
            NORMAL = 1,
            LARGE = 2,
        };

    public:
        void setup(const uint8_t left);

        void move(const int8_t dy);

        void target(const uint8_t target);

        void invert();

        void changeSize();

    public:
        int8_t x;
        int8_t y;

    private:
        uint8_t size;
        uint8_t inverted;

        uint8_t sprite_ids[2];
        hw::Pcd8544::Sprite sprites[2];
    };
}
