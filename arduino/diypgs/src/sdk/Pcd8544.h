#pragma once

#include "Arduino.h"

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
        uint8_t x;
        uint8_t y;
        uint8_t alpha;
        uint8_t *data;
    };

public:
    Pcd8544(const uint8_t dcPin, const uint8_t resetPin);

public:
    void setup() const;
    void render() const;

public:
    static void set_sprite(const uint8_t id, const Sprite sprite);

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
};