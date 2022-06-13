#pragma once

#include "../hw/Pcd8544.h"

namespace game
{
    class Paddle
    {
    public:
        enum Side : uint8_t
        {
            LEFT = 0,
            RIGHT = 1,
        };

    private:
        enum Size : uint8_t
        {
            SMALL = 0,
            NORMAL = 1,
            LARGE = 2,
        };

        enum Transition : uint8_t
        {
            NONE = 0,
            INVERT_DIRECTION = 1,
            CHANGE_SIZE = 2
        };

    public:
        void setup(const Side side);

        void update();

        void move(const int8_t dy);

        void target(const uint8_t target);

        void invertDirection();
        void changeSize();

        uint8_t getWidth() const;
        uint8_t getHeight() const;

        uint8_t getCenterX() const;
        uint8_t getCenterY() const;

    private:
        void applyInvertDirection();
        void applyChangeSize();

    public:
        int8_t x;
        int8_t y;

    private:
        uint8_t side;
        uint8_t size;
        uint8_t inverted;

        uint8_t transition;
        uint8_t transitionFrameCounter;

        uint8_t sprite_ids[2];
        hw::Pcd8544::Sprite sprites[2];
    };
}
