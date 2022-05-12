#ifndef BONUS_H
#define BONUS_H

#include "../sdk/pcd8544.h"

uint8_t gfx_bonus_top_left[8] = {0x00, 0x00, 0x10, 0x28, 0x48, 0x90, 0xA0, 0xC0};
uint8_t gfx_bonus_top_right[8] = {0xC0, 0xA0, 0x90, 0x48, 0x28, 0x10, 0x00, 0x00};
uint8_t gfx_bonus_bottom_left[8] = {0x00, 0x7E, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0x80};
uint8_t gfx_bonus_bottom_right[8] = {0x80, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0x7E, 0x00};

class Bonus
{
private:
    uint8_t state;
    int8_t x;
    int8_t y;
    uint8_t sprite_ids[4];
    PCD8544::sprite_t sprites[4];

public:
    Bonus()
    {
    }

    void setup()
    {
        state = 0;
        x = (PCD8544::SCREEN_WIDTH / 2) - 8;
        y = (PCD8544::SCREEN_HEIGHT / 2) - 12;

        sprite_ids[0] = 6;
        sprite_ids[1] = 7;
        sprite_ids[2] = 8;
        sprite_ids[3] = 9;

        sprites[0] = {!PCD8544::sprite_t::Flag::ENABLED, uint8_t(x + 0), uint8_t(y + 0), gfx_bonus_top_left};
        sprites[1] = {!PCD8544::sprite_t::Flag::ENABLED, uint8_t(x + 8), uint8_t(y + 0), gfx_bonus_top_right};
        sprites[2] = {!PCD8544::sprite_t::Flag::ENABLED, uint8_t(x + 0), uint8_t(y + 8), gfx_bonus_bottom_left};
        sprites[3] = {!PCD8544::sprite_t::Flag::ENABLED, uint8_t(x + 8), uint8_t(y + 8), gfx_bonus_bottom_right};

        PCD8544::set_sprite(sprite_ids[0], sprites[0]);
        PCD8544::set_sprite(sprite_ids[1], sprites[1]);
        PCD8544::set_sprite(sprite_ids[2], sprites[2]);
        PCD8544::set_sprite(sprite_ids[3], sprites[3]);
    }

    void update()
    {
        if (state)
        {
        }
    }

    void on()
    {
        state = 1;

        sprites[0].flags = PCD8544::sprite_t::Flag::ENABLED;
        sprites[1].flags = PCD8544::sprite_t::Flag::ENABLED;
        sprites[2].flags = PCD8544::sprite_t::Flag::ENABLED;
        sprites[3].flags = PCD8544::sprite_t::Flag::ENABLED;

        PCD8544::set_sprite(sprite_ids[0], sprites[0]);
        PCD8544::set_sprite(sprite_ids[1], sprites[1]);
        PCD8544::set_sprite(sprite_ids[2], sprites[2]);
        PCD8544::set_sprite(sprite_ids[3], sprites[3]);
    }

    void off()
    {
        state = 0;

        sprites[0].flags = !PCD8544::sprite_t::Flag::ENABLED;
        sprites[1].flags = !PCD8544::sprite_t::Flag::ENABLED;
        sprites[2].flags = !PCD8544::sprite_t::Flag::ENABLED;
        sprites[3].flags = !PCD8544::sprite_t::Flag::ENABLED;

        PCD8544::set_sprite(sprite_ids[0], sprites[0]);
        PCD8544::set_sprite(sprite_ids[1], sprites[1]);
        PCD8544::set_sprite(sprite_ids[2], sprites[2]);
        PCD8544::set_sprite(sprite_ids[3], sprites[3]);
    }
};

#endif