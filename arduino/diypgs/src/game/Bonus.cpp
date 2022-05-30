#include "Bonus.h"

namespace game
{
    namespace
    {
        constexpr uint8_t DISABLED_FRAME_COUNT = 8 * 30; // 8 sec
        constexpr uint8_t ENABLED_FRAME_COUNT = 8 * 30;  // 8 sec

        constexpr uint8_t WINDOW_MIN_X = (hw::Pcd8544::SCREEN_WIDTH / 2) - (hw::Pcd8544::SCREEN_WIDTH / 4);
        constexpr uint8_t WINDOW_MAX_X = (hw::Pcd8544::SCREEN_WIDTH / 2) + (hw::Pcd8544::SCREEN_WIDTH / 4);
        constexpr uint8_t WINDOW_MIN_Y = (hw::Pcd8544::SCREEN_HEIGHT / 2) - (hw::Pcd8544::SCREEN_HEIGHT / 4);
        constexpr uint8_t WINDOW_MAX_Y = (hw::Pcd8544::SCREEN_HEIGHT / 2) + (hw::Pcd8544::SCREEN_HEIGHT / 4);

        constexpr sdk::fixed16_t HALF_HORIZONTAL_MOTION = sdk::fixed16_t(8);
        constexpr sdk::fixed16_t HALF_VERTICAL_MOTION = sdk::fixed16_t(2);

        constexpr sdk::fixed16_t HORIZONTAL_SPEED = sdk::fixed16_t(0, 32);
        constexpr sdk::fixed16_t VERTICAL_SPEED = sdk::fixed16_t(0, 32);

        constexpr uint8_t GFX_BONUS_TOP_LEFT[8] = {0x00, 0x00, 0x10, 0x28, 0x48, 0x90, 0xA0, 0xC0};
        constexpr uint8_t GFX_BONUS_TOP_RIGHT[8] = {0xC0, 0xA0, 0x90, 0x48, 0x28, 0x10, 0x00, 0x00};
        constexpr uint8_t GFX_BONUS_BOTTOM_LEFT[8] = {0x00, 0x7E, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0x80};
        constexpr uint8_t GFX_BONUS_BOTTOM_RIGHT[8] = {0x80, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0x7E, 0x00};
    }

    void Bonus::setup()
    {
        state = State::DISABLED;
        switch_state_counter = DISABLED_FRAME_COUNT;

        origin.x = sdk::fixed16_t(hw::Pcd8544::SCREEN_WIDTH / 2);
        origin.y = sdk::fixed16_t(hw::Pcd8544::SCREEN_HEIGHT / 2);
        center = origin;
        dxdy.x = HORIZONTAL_SPEED;
        dxdy.y = VERTICAL_SPEED;

        sprite_ids[0] = 6;
        sprite_ids[1] = 7;
        sprite_ids[2] = 8;
        sprite_ids[3] = 9;

        const sdk::Vec2f16 corner = center - sdk::Vec2f16(sdk::fixed16_t(8), sdk::fixed16_t(8));

        sprites[0] = {hw::Pcd8544::Sprite::Flag::ENABLED | hw::Pcd8544::Sprite::Flag::ALPHA, corner.x.toInt8(), corner.y.toInt8(), 0, GFX_BONUS_TOP_LEFT};
        sprites[1] = {hw::Pcd8544::Sprite::Flag::ENABLED | hw::Pcd8544::Sprite::Flag::ALPHA, center.x.toInt8(), corner.y.toInt8(), 0, GFX_BONUS_TOP_RIGHT};
        sprites[2] = {hw::Pcd8544::Sprite::Flag::ENABLED | hw::Pcd8544::Sprite::Flag::ALPHA, corner.x.toInt8(), center.y.toInt8(), 0, GFX_BONUS_BOTTOM_LEFT};
        sprites[3] = {hw::Pcd8544::Sprite::Flag::ENABLED | hw::Pcd8544::Sprite::Flag::ALPHA, center.x.toInt8(), center.y.toInt8(), 0, GFX_BONUS_BOTTOM_RIGHT};

        hw::Pcd8544::set_sprite(sprite_ids[0], sprites[0]);
        hw::Pcd8544::set_sprite(sprite_ids[1], sprites[1]);
        hw::Pcd8544::set_sprite(sprite_ids[2], sprites[2]);
        hw::Pcd8544::set_sprite(sprite_ids[3], sprites[3]);
    }

    void Bonus::update()
    {
        center += dxdy;

        if (center.x > origin.x + HALF_HORIZONTAL_MOTION)
        {
            dxdy.x = -dxdy.x;
        }
        if (center.x < origin.x - HALF_HORIZONTAL_MOTION)
        {
            dxdy.x = -dxdy.x;
        }

        if (center.y > origin.y + HALF_VERTICAL_MOTION)
        {
            dxdy.y = -dxdy.y;
        }
        if (center.y < origin.y - HALF_VERTICAL_MOTION)
        {
            dxdy.y = -dxdy.y;
        }

        const sdk::Vec2f16 corner = center - sdk::Vec2f16(sdk::fixed16_t(8), sdk::fixed16_t(8));

        sprites[0].x = corner.x.toInt8();
        sprites[0].y = corner.y.toInt8();
        sprites[1].x = center.x.toInt8();
        sprites[1].y = corner.y.toInt8();
        sprites[2].x = corner.x.toInt8();
        sprites[2].y = center.y.toInt8();
        sprites[3].x = center.x.toInt8();
        sprites[3].y = center.y.toInt8();

        switch (state)
        {
        case State::DISABLED:
            sprites[0].alpha = 0;
            sprites[1].alpha = 0;
            sprites[2].alpha = 0;
            sprites[3].alpha = 0;

            if (switch_state_counter > 0)
            {
                switch_state_counter--;
                if (switch_state_counter == 0)
                {
                    state = State::FADE_IN;
                    switch_state_counter = 16;

                    origin.x = sdk::fixed16_t(WINDOW_MIN_X + (rand() % (WINDOW_MAX_X - WINDOW_MIN_X)));
                    origin.y = sdk::fixed16_t(WINDOW_MIN_Y + (rand() % (WINDOW_MAX_Y - WINDOW_MIN_Y)));
                    center = origin;
                }
            }
            break;

        case State::FADE_IN:
            sprites[0].alpha = 16 - switch_state_counter;
            sprites[1].alpha = 16 - switch_state_counter;
            sprites[2].alpha = 16 - switch_state_counter;
            sprites[3].alpha = 16 - switch_state_counter;

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

        hw::Pcd8544::set_sprite(sprite_ids[0], sprites[0]);
        hw::Pcd8544::set_sprite(sprite_ids[1], sprites[1]);
        hw::Pcd8544::set_sprite(sprite_ids[2], sprites[2]);
        hw::Pcd8544::set_sprite(sprite_ids[3], sprites[3]);
    }

    Bonus::Effect Bonus::capture()
    {
        sprites[0].alpha = 0;
        sprites[1].alpha = 0;
        sprites[2].alpha = 0;
        sprites[3].alpha = 0;

        hw::Pcd8544::set_sprite(sprite_ids[0], sprites[0]);
        hw::Pcd8544::set_sprite(sprite_ids[1], sprites[1]);
        hw::Pcd8544::set_sprite(sprite_ids[2], sprites[2]);
        hw::Pcd8544::set_sprite(sprite_ids[3], sprites[3]);

        state = State::DISABLED;
        switch_state_counter = DISABLED_FRAME_COUNT;

        const uint8_t id = rand() % uint8_t(Effect::COUNT);
        return Effect(id);
    }
}
