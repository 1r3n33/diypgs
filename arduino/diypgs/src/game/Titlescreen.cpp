#include "Titlescreen.h"

namespace game
{
    namespace
    {
        constexpr uint16_t BACKGROUND_TOP_SIZE = hw::Pcd8544::SCREEN_WIDTH * 2;
        constexpr uint16_t BACKGROUND_LOGO_SIZE = hw::Pcd8544::SCREEN_WIDTH * 2;
        constexpr uint16_t BACKGROUND_BOTTOM_SIZE = hw::Pcd8544::SCREEN_WIDTH * 2;
        constexpr uint16_t BACKGROUND_SIZE = BACKGROUND_TOP_SIZE + BACKGROUND_LOGO_SIZE + BACKGROUND_BOTTOM_SIZE;

        constexpr uint8_t LOGO[BACKGROUND_LOGO_SIZE] = {
            0, 0,
            0, 0, 0, 48, 248, 248, 252, 28,
            30, 14, 14, 8, 120, 254, 254, 238,
            206, 222, 220, 252, 248, 112, 0, 128,
            248, 252, 252, 248, 240, 128, 0, 0,
            254, 255, 255, 199, 199, 199, 254, 252,
            120, 252, 254, 254, 142, 220, 252, 248,
            112, 0, 0, 0, 240, 248, 252, 126,
            252, 224, 128, 0, 224, 248, 252, 24,
            0, 0, 0, 0, 0, 248, 252, 248,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0,
            0, 0,
            0, 0, 0, 0, 1, 127, 255, 127,
            14, 7, 7, 2, 0, 63, 127, 63,
            7, 7, 15, 31, 60, 120, 62, 63,
            31, 19, 56, 59, 31, 31, 62, 120,
            31, 63, 31, 1, 1, 1, 1, 0,
            0, 63, 127, 63, 115, 115, 115, 127,
            126, 62, 28, 63, 31, 7, 14, 12,
            6, 7, 31, 62, 31, 31, 63, 56,
            56, 56, 56, 120, 48, 31, 63, 63,
            56, 56, 56, 56, 16, 0, 0, 0,
            0, 0};
    }

    Titlescreen::Titlescreen(const hw::Pcd8544 &pcd8544,
                             const hw::Buzzer &buzzer,
                             const hw::Gamepad &gamepad) : World(pcd8544,
                                                                 buzzer,
                                                                 gamepad)
    {
    }

    void Titlescreen::setup()
    {
        uint8_t background[BACKGROUND_SIZE];

        uint8_t *top = background;
        uint8_t *logo = top + BACKGROUND_TOP_SIZE;
        uint8_t *bottom = logo + BACKGROUND_LOGO_SIZE;

        for (uint16_t i = 0; i < BACKGROUND_TOP_SIZE; i++)
        {
            top[i] = 0;
        }

        for (uint16_t i = 0; i < BACKGROUND_LOGO_SIZE; i++)
        {
            logo[i] = LOGO[i];
        }

        for (uint16_t i = 0; i < BACKGROUND_BOTTOM_SIZE; i++)
        {
            bottom[i] = 0;
        }

        graphics.setBackground(background);
    }

    World::State Titlescreen::update()
    {
        graphics.render();

        delay(3000);

        graphics.clearBackground();

        return World::State::NEXT;
    }
}
