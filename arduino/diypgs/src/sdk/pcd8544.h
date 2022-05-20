#ifndef PCD8544_H
#define PCD8544_H

#include "Arduino.h"

class PCD8544
{
public:
    static const uint8_t SCREEN_WIDTH = 84;
    static const uint8_t SCREEN_HEIGHT = 48;

    struct sprite_t
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

    static void set_sprite(uint8_t id, sprite_t sprite);

    static void setup();
    static void render();
};

#endif
