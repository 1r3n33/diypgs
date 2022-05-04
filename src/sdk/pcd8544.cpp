#include "Arduino.h"
#include "pcd8544.h"
#include "SPI.h"

#define RST_PIN 12
#define DC_PIN 10

SPISettings spi_settings(16 * 1000000, MSBFIRST, SPI_MODE0);

#define MAX_SPRITE_COUNT 16

PCD8544::sprite_t sprites[MAX_SPRITE_COUNT] = {0};

#define BUFFER_SIZE (6 * PCD8544::SCREEN_WIDTH)

uint8_t buffer[BUFFER_SIZE] = {0};

void draw_8x8(uint8_t x, uint8_t y, uint8_t *data)
{
    uint8_t mod0 = y % 8;
    uint8_t mod1 = 8 - mod0;

    int16_t y0 = (y / 8) * PCD8544::SCREEN_WIDTH;
    int16_t y1 = y0 + PCD8544::SCREEN_WIDTH;

    uint8_t *buf0 = buffer + y0 + x;
    uint8_t *buf1 = buf0 + PCD8544::SCREEN_WIDTH;

    uint8_t *ptr0 = data;
    uint8_t *ptr1 = ptr0;

    *buf0++ |= (*ptr0++ << mod0);
    *buf0++ |= (*ptr0++ << mod0);
    *buf0++ |= (*ptr0++ << mod0);
    *buf0++ |= (*ptr0++ << mod0);
    *buf0++ |= (*ptr0++ << mod0);
    *buf0++ |= (*ptr0++ << mod0);
    *buf0++ |= (*ptr0++ << mod0);
    *buf0++ |= (*ptr0++ << mod0);

    *buf1++ |= (*ptr1++ >> mod1);
    *buf1++ |= (*ptr1++ >> mod1);
    *buf1++ |= (*ptr1++ >> mod1);
    *buf1++ |= (*ptr1++ >> mod1);
    *buf1++ |= (*ptr1++ >> mod1);
    *buf1++ |= (*ptr1++ >> mod1);
    *buf1++ |= (*ptr1++ >> mod1);
    *buf1++ |= (*ptr1++ >> mod1);
}

void draw_8x8_clip(int8_t x, int8_t y, uint8_t *data)
{
    int8_t left = max(0, x);
    int8_t right = min(PCD8544::SCREEN_WIDTH, x + 8);

    int8_t mod0 = y % 8;
    int8_t mod1 = 8 - mod0;

    int16_t y0 = (y / 8) * PCD8544::SCREEN_WIDTH;
    int16_t y1 = y0 + PCD8544::SCREEN_WIDTH;

    uint8_t *buf0 = buffer + y0 + left;
    uint8_t *buf1 = buf0 + PCD8544::SCREEN_WIDTH;

    uint8_t *ptr0 = data + (left - x);
    uint8_t *ptr1 = ptr0;

    for (int8_t i = left; i < right; i++)
    {
        *buf0++ |= (*ptr0++ << mod0);
    }

    for (int8_t i = left; i < right; i++)
    {
        *buf1++ |= (*ptr1++ >> mod1);
    }
}

void draw_sprite(uint8_t i)
{
    PCD8544::sprite_t *sprite = sprites + i;
    switch (sprite->flags)
    {
    case PCD8544::sprite_t::Flag::ENABLED:
        draw_8x8(sprite->x, sprite->y, sprite->data);
        break;

    case PCD8544::sprite_t::Flag::ENABLED | PCD8544::sprite_t::Flag::XCLIP:
        draw_8x8_clip(sprite->x, sprite->y, sprite->data);
        break;
    }
}

void clear_buffer()
{
    for (int16_t i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = 0;
    }
}

void display_buffer()
{
    digitalWrite(SS, LOW); // Activate
    SPI.beginTransaction(spi_settings);

    // Draw buffer
    digitalWrite(DC_PIN, HIGH); // DC pin is high for data
    SPI.transfer(buffer, BUFFER_SIZE);

    SPI.endTransaction();
    digitalWrite(SS, HIGH); // Deactivate
}

void PCD8544::set_sprite(uint8_t id, PCD8544::sprite_t sprite)
{
    sprites[id] = sprite;
}

void PCD8544::setup()
{
    pinMode(DC_PIN, OUTPUT);
    pinMode(RST_PIN, OUTPUT);

    SPI.begin(); // set SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high

    digitalWrite(RST_PIN, LOW);
    digitalWrite(RST_PIN, HIGH);

    digitalWrite(SS, LOW); // Activate

    digitalWrite(DC_PIN, LOW); // DC pin is low for commands
    SPI.beginTransaction(spi_settings);
    SPI.transfer(0x21); // LCD extended commands
    SPI.transfer(0xB8); // set LCD Vop (contrast)
    SPI.transfer(0x04); // set temp coefficient
    SPI.transfer(0x14); // LCD bias mode 1:40
    SPI.transfer(0x20); // LCD basic commands
    SPI.transfer(0x0C); // normal display mode
    SPI.transfer(0x40); // set y to 0
    SPI.transfer(0x80); // set x to 0
    SPI.endTransaction();

    digitalWrite(SS, HIGH); // Deactivate

    clear_buffer();
    display_buffer();
}

void PCD8544::render()
{
    clear_buffer();

    for (uint8_t i = 0; i < MAX_SPRITE_COUNT; i++)
    {
        draw_sprite(i);
    }

    display_buffer();
}