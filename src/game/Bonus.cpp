#include "Bonus.h"

namespace game
{
    constexpr uint8_t SWITCH_STATE_FRAME_COUNT = 8 * 30; // 8 sec

    void Bonus::setup()
    {
        static uint8_t gfx_bonus_top_left[8] = {0x00, 0x00, 0x10, 0x28, 0x48, 0x90, 0xA0, 0xC0};
        static uint8_t gfx_bonus_top_right[8] = {0xC0, 0xA0, 0x90, 0x48, 0x28, 0x10, 0x00, 0x00};
        static uint8_t gfx_bonus_bottom_left[8] = {0x00, 0x7E, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0x80};
        static uint8_t gfx_bonus_bottom_right[8] = {0x80, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0x7E, 0x00};

        state = !State::ENABLED;
        switch_state_counter = SWITCH_STATE_FRAME_COUNT;

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

    void Bonus::update()
    {
        if (switch_state_counter > 0)
        {
            switch_state_counter--;
            if (switch_state_counter == 0)
            {
                if (state == State::ENABLED)
                {
                    off();
                }
                else
                {
                    on();
                }
            }
        }
    }

    void Bonus::on()
    {
        state = State::ENABLED;
        switch_state_counter = SWITCH_STATE_FRAME_COUNT;

        sprites[0].flags = PCD8544::sprite_t::Flag::ENABLED;
        sprites[1].flags = PCD8544::sprite_t::Flag::ENABLED;
        sprites[2].flags = PCD8544::sprite_t::Flag::ENABLED;
        sprites[3].flags = PCD8544::sprite_t::Flag::ENABLED;

        PCD8544::set_sprite(sprite_ids[0], sprites[0]);
        PCD8544::set_sprite(sprite_ids[1], sprites[1]);
        PCD8544::set_sprite(sprite_ids[2], sprites[2]);
        PCD8544::set_sprite(sprite_ids[3], sprites[3]);
    }

    void Bonus::off()
    {
        state = !State::ENABLED;
        switch_state_counter = SWITCH_STATE_FRAME_COUNT;

        sprites[0].flags = !PCD8544::sprite_t::Flag::ENABLED;
        sprites[1].flags = !PCD8544::sprite_t::Flag::ENABLED;
        sprites[2].flags = !PCD8544::sprite_t::Flag::ENABLED;
        sprites[3].flags = !PCD8544::sprite_t::Flag::ENABLED;

        PCD8544::set_sprite(sprite_ids[0], sprites[0]);
        PCD8544::set_sprite(sprite_ids[1], sprites[1]);
        PCD8544::set_sprite(sprite_ids[2], sprites[2]);
        PCD8544::set_sprite(sprite_ids[3], sprites[3]);
    }

    Bonus::Effect Bonus::getEffect() const
    {
        return rand() & 1 ? Effect::INVERT_COMMAND_LEFT : Effect::INVERT_COMMAND_RIGHT;
    }
}
