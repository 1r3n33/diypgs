#pragma once

#include "../hw/Pcd8544.h"

namespace
{
    constexpr uint8_t GFX_PADDLE_LEFT_TOP[8] = {0xF0, 0x0C, 0xF2, 0xFA, 0xFD, 0xFD, 0xFD, 0xFF};
    constexpr uint8_t GFX_PADDLE_LEFT_BOTTOM[8] = {0x0F, 0x30, 0x4F, 0x5F, 0xBF, 0xBF, 0xBF, 0xFF};

    constexpr uint8_t GFX_PADDLE_RIGHT_TOP[8] = {0xFF, 0xFD, 0xFD, 0xFD, 0xFA, 0xF2, 0x0C, 0xF0};
    constexpr uint8_t GFX_PADDLE_RIGHT_BOTTOM[8] = {0xFF, 0xBF, 0xBF, 0xBF, 0x5F, 0x4F, 0x30, 0x0F};
}

class Paddle
{
public:
    int8_t x;
    int8_t y;

private:
    uint8_t inverted;

    uint8_t sprite_ids[2];
    hw::Pcd8544::Sprite sprites[2];

public:
    Paddle()
    {
    }

    void setup(const uint8_t left)
    {
        x = left ? 0 : (hw::Pcd8544::SCREEN_WIDTH - 8);
        y = (hw::Pcd8544::SCREEN_HEIGHT - 16) / 2;

        inverted = 0;

        sprite_ids[0] = left ? 2 : 4;
        sprite_ids[1] = left ? 3 : 5;

        sprites[0] = {hw::Pcd8544::Sprite::Flag::ENABLED, x, y, 0xFF, left ? GFX_PADDLE_LEFT_TOP : GFX_PADDLE_RIGHT_TOP};
        sprites[1] = {hw::Pcd8544::Sprite::Flag::ENABLED, x, int8_t(y + 8), 0xFF, left ? GFX_PADDLE_LEFT_BOTTOM : GFX_PADDLE_RIGHT_BOTTOM};

        hw::Pcd8544::set_sprite(sprite_ids[0], sprites[0]);
        hw::Pcd8544::set_sprite(sprite_ids[1], sprites[1]);
    }

    void move(int8_t dy)
    {
        y = inverted ? y - dy : y + dy;
        if (y < 0)
        {
            y = 0;
        }
        if (y > hw::Pcd8544::SCREEN_HEIGHT - 16)
        {
            y = hw::Pcd8544::SCREEN_HEIGHT - 16;
        }

        sprites[0].y = y;
        sprites[1].y = y + 8;

        hw::Pcd8544::set_sprite(sprite_ids[0], sprites[0]);
        hw::Pcd8544::set_sprite(sprite_ids[1], sprites[1]);
    }

    void target(uint8_t target)
    {
        uint8_t paddle_center = y + 8;
        if (paddle_center < target)
        {
            move(+1);
        }
        else if (paddle_center > target)
        {
            move(-1);
        }
    }

    void invert()
    {
        inverted = !inverted;
    }
};
