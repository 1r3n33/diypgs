#pragma once

#include "Arduino.h"

namespace hw
{
    class Pcd8544
    {
    public:
        struct Sprite
        {
            enum Flag : uint8_t
            {
                ENABLED = 0x01,
                XCLIP = 0x02,
                ALPHA = 0x04,
            };

            uint8_t flags;
            int8_t x;
            int8_t y;
            uint8_t alpha;
            const uint8_t *data;
        };

    public:
        Pcd8544(const uint8_t dcPin, const uint8_t resetPin, const uint8_t ssPin);

    public:
        void setup() const;
        void render() const;

    public:
        static void set_sprite(const uint8_t id, const Sprite sprite);

    public:
        void clearBackground() const;
        void setBackground(const uint8_t *background) const;

    private:
        void clearBuffer() const;
        void displayBuffer() const;

    public:
        static constexpr uint8_t SCREEN_WIDTH = 84;
        static constexpr uint8_t SCREEN_HEIGHT = 48;
        static constexpr uint8_t MAX_SPRITE_COUNT = 16;

    private:
        const uint8_t DC_PIN;
        const uint8_t RESET_PIN;
        const uint8_t SS_PIN;
    };
}
