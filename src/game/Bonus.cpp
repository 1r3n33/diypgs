#include "Bonus.h"

namespace game
{
    constexpr uint8_t DISABLED_FRAME_COUNT = 8 * 30; // 8 sec
    constexpr uint8_t ENABLED_FRAME_COUNT = 4 * 30;  // 4 sec

    void Bonus::setup()
    {
        static uint8_t gfx_bonus_top_left[8] = {0x00, 0x00, 0x10, 0x28, 0x48, 0x90, 0xA0, 0xC0};
        static uint8_t gfx_bonus_top_right[8] = {0xC0, 0xA0, 0x90, 0x48, 0x28, 0x10, 0x00, 0x00};
        static uint8_t gfx_bonus_bottom_left[8] = {0x00, 0x7E, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0x80};
        static uint8_t gfx_bonus_bottom_right[8] = {0x80, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0x7E, 0x00};

        state = State::DISABLED;
        switch_state_counter = DISABLED_FRAME_COUNT;

        x = (PCD8544::SCREEN_WIDTH / 2) - 8;
        y = (PCD8544::SCREEN_HEIGHT / 2) - 12;

        sprite_ids[0] = 6;
        sprite_ids[1] = 7;
        sprite_ids[2] = 8;
        sprite_ids[3] = 9;

        sprites[0] = {PCD8544::sprite_t::Flag::ENABLED | PCD8544::sprite_t::Flag::ALPHA, uint8_t(x + 0), uint8_t(y + 0), 0, gfx_bonus_top_left};
        sprites[1] = {PCD8544::sprite_t::Flag::ENABLED | PCD8544::sprite_t::Flag::ALPHA, uint8_t(x + 8), uint8_t(y + 0), 0, gfx_bonus_top_right};
        sprites[2] = {PCD8544::sprite_t::Flag::ENABLED | PCD8544::sprite_t::Flag::ALPHA, uint8_t(x + 0), uint8_t(y + 8), 0, gfx_bonus_bottom_left};
        sprites[3] = {PCD8544::sprite_t::Flag::ENABLED | PCD8544::sprite_t::Flag::ALPHA, uint8_t(x + 8), uint8_t(y + 8), 0, gfx_bonus_bottom_right};

        PCD8544::set_sprite(sprite_ids[0], sprites[0]);
        PCD8544::set_sprite(sprite_ids[1], sprites[1]);
        PCD8544::set_sprite(sprite_ids[2], sprites[2]);
        PCD8544::set_sprite(sprite_ids[3], sprites[3]);
    }

    void Bonus::update()
    {
        switch (state)
        {
        case State::DISABLED:
            sprites[0].alpha = 0;
            sprites[1].alpha = 0;
            sprites[2].alpha = 0;
            sprites[3].alpha = 0;

            PCD8544::set_sprite(sprite_ids[0], sprites[0]);
            PCD8544::set_sprite(sprite_ids[1], sprites[1]);
            PCD8544::set_sprite(sprite_ids[2], sprites[2]);
            PCD8544::set_sprite(sprite_ids[3], sprites[3]);

            if (switch_state_counter > 0)
            {
                switch_state_counter--;
                if (switch_state_counter == 0)
                {
                    state = State::FADE_IN;
                    switch_state_counter = 16;
                }
            }
            break;

        case State::FADE_IN:
            sprites[0].alpha = 16 - switch_state_counter;
            sprites[1].alpha = 16 - switch_state_counter;
            sprites[2].alpha = 16 - switch_state_counter;
            sprites[3].alpha = 16 - switch_state_counter;

            PCD8544::set_sprite(sprite_ids[0], sprites[0]);
            PCD8544::set_sprite(sprite_ids[1], sprites[1]);
            PCD8544::set_sprite(sprite_ids[2], sprites[2]);
            PCD8544::set_sprite(sprite_ids[3], sprites[3]);

            if (switch_state_counter > 0)
            {
                switch_state_counter--;
                if (switch_state_counter == 0)
                {
                    state = State::ENABLED;
                    switch_state_counter = ENABLED_FRAME_COUNT;
                }
            }
            break;

        case State::ENABLED:
            sprites[0].alpha = 16;
            sprites[1].alpha = 16;
            sprites[2].alpha = 16;
            sprites[3].alpha = 16;

            PCD8544::set_sprite(sprite_ids[0], sprites[0]);
            PCD8544::set_sprite(sprite_ids[1], sprites[1]);
            PCD8544::set_sprite(sprite_ids[2], sprites[2]);
            PCD8544::set_sprite(sprite_ids[3], sprites[3]);

            if (switch_state_counter > 0)
            {
                switch_state_counter--;
                if (switch_state_counter == 0)
                {
                    state = State::FADE_OUT;
                    switch_state_counter = 16;
                }
            }
            break;

        case State::FADE_OUT:
            sprites[0].alpha = switch_state_counter;
            sprites[1].alpha = switch_state_counter;
            sprites[2].alpha = switch_state_counter;
            sprites[3].alpha = switch_state_counter;

            PCD8544::set_sprite(sprite_ids[0], sprites[0]);
            PCD8544::set_sprite(sprite_ids[1], sprites[1]);
            PCD8544::set_sprite(sprite_ids[2], sprites[2]);
            PCD8544::set_sprite(sprite_ids[3], sprites[3]);

            if (switch_state_counter > 0)
            {
                switch_state_counter--;
                if (switch_state_counter == 0)
                {
                    state = State::DISABLED;
                    switch_state_counter = DISABLED_FRAME_COUNT;
                }
            }
            break;
        }
    }

    Bonus::Effect Bonus::capture()
    {
        sprites[0].alpha = 0;
        sprites[1].alpha = 0;
        sprites[2].alpha = 0;
        sprites[3].alpha = 0;

        PCD8544::set_sprite(sprite_ids[0], sprites[0]);
        PCD8544::set_sprite(sprite_ids[1], sprites[1]);
        PCD8544::set_sprite(sprite_ids[2], sprites[2]);
        PCD8544::set_sprite(sprite_ids[3], sprites[3]);

        state = State::DISABLED;
        switch_state_counter = DISABLED_FRAME_COUNT;

        const uint8_t id = rand() % uint8_t(Effect::COUNT);
        return Effect(id);
    }
}
