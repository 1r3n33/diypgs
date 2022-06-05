#include "Arduino.h"
#include "Pcd8544.h"
#include "SPI.h"

namespace hw
{
    namespace
    {
        // https://en.wikipedia.org/wiki/Ordered_dithering
        constexpr uint8_t BAYER_MATRIX[17][8] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                 {0x88, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00},
                                                 {0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00},
                                                 {0xAA, 0x00, 0x22, 0x00, 0xAA, 0x00, 0x22, 0x00},
                                                 {0xAA, 0x00, 0xAA, 0x00, 0xAA, 0x00, 0xAA, 0x00},
                                                 {0xAA, 0x44, 0xAA, 0x00, 0xAA, 0x44, 0xAA, 0x00},
                                                 {0xAA, 0x44, 0xAA, 0x11, 0xAA, 0x44, 0xAA, 0x11},
                                                 {0xAA, 0x55, 0xAA, 0x11, 0xAA, 0x55, 0xAA, 0x11},
                                                 {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55},
                                                 {0xEE, 0x55, 0xAA, 0x55, 0xEE, 0x55, 0xAA, 0x55},
                                                 {0xEE, 0x55, 0xBB, 0x55, 0xEE, 0x55, 0xBB, 0x55},
                                                 {0xFF, 0x55, 0xBB, 0x55, 0xFF, 0x55, 0xBB, 0x55},
                                                 {0xFF, 0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF, 0x55},
                                                 {0xFF, 0xDD, 0xFF, 0x55, 0xFF, 0xDD, 0xFF, 0x55},
                                                 {0xFF, 0xDD, 0xFF, 0x77, 0xFF, 0xDD, 0xFF, 0x77},
                                                 {0xFF, 0xFF, 0xFF, 0x77, 0xFF, 0xFF, 0xFF, 0x77},
                                                 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

        const SPISettings SPI_SETTINGS(F_CPU, MSBFIRST, SPI_MODE0);

        Pcd8544::Sprite sprites[Pcd8544::MAX_SPRITE_COUNT] = {{0, 0, 0, 0, 0}};

        constexpr uint16_t BUFFER_SIZE = (6 * Pcd8544::SCREEN_WIDTH);

        uint8_t background[BUFFER_SIZE];

        uint8_t buffer[BUFFER_SIZE];

        void draw_8x8(const uint8_t x, const uint8_t y, const uint8_t *data)
        {
            uint8_t mod0 = y % 8;
            uint8_t mod1 = 8 - mod0;

            int16_t y0 = (y / 8) * Pcd8544::SCREEN_WIDTH;

            uint8_t *buf0 = buffer + y0 + x;
            uint8_t *buf1 = buf0 + Pcd8544::SCREEN_WIDTH;

            const uint8_t *ptr0 = data;
            const uint8_t *ptr1 = ptr0;

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

        void draw_8x8_alpha(const uint8_t x, const uint8_t y, const uint8_t alpha, const uint8_t *data)
        {
            uint8_t mod0 = y % 8;
            uint8_t mod1 = 8 - mod0;

            int16_t y0 = (y / 8) * Pcd8544::SCREEN_WIDTH;

            uint8_t *buf0 = buffer + y0 + x;
            uint8_t *buf1 = buf0 + Pcd8544::SCREEN_WIDTH;

            const uint8_t *ptr0 = data;
            const uint8_t *ptr1 = ptr0;

            *buf0++ |= ((*ptr0++ << mod0) & BAYER_MATRIX[alpha][0]);
            *buf0++ |= ((*ptr0++ << mod0) & BAYER_MATRIX[alpha][1]);
            *buf0++ |= ((*ptr0++ << mod0) & BAYER_MATRIX[alpha][2]);
            *buf0++ |= ((*ptr0++ << mod0) & BAYER_MATRIX[alpha][3]);
            *buf0++ |= ((*ptr0++ << mod0) & BAYER_MATRIX[alpha][4]);
            *buf0++ |= ((*ptr0++ << mod0) & BAYER_MATRIX[alpha][5]);
            *buf0++ |= ((*ptr0++ << mod0) & BAYER_MATRIX[alpha][6]);
            *buf0++ |= ((*ptr0++ << mod0) & BAYER_MATRIX[alpha][7]);

            *buf1++ |= ((*ptr1++ >> mod1) & BAYER_MATRIX[alpha][0]);
            *buf1++ |= ((*ptr1++ >> mod1) & BAYER_MATRIX[alpha][1]);
            *buf1++ |= ((*ptr1++ >> mod1) & BAYER_MATRIX[alpha][2]);
            *buf1++ |= ((*ptr1++ >> mod1) & BAYER_MATRIX[alpha][3]);
            *buf1++ |= ((*ptr1++ >> mod1) & BAYER_MATRIX[alpha][4]);
            *buf1++ |= ((*ptr1++ >> mod1) & BAYER_MATRIX[alpha][5]);
            *buf1++ |= ((*ptr1++ >> mod1) & BAYER_MATRIX[alpha][6]);
            *buf1++ |= ((*ptr1++ >> mod1) & BAYER_MATRIX[alpha][7]);
        }

        void draw_8x8_clip(const int8_t x, const int8_t y, const uint8_t *data)
        {
            int8_t left = max(0, x);
            int8_t right = min(Pcd8544::SCREEN_WIDTH, x + 8);

            int8_t mod0 = y % 8;
            int8_t mod1 = 8 - mod0;

            int16_t y0 = (y / 8) * Pcd8544::SCREEN_WIDTH;

            uint8_t *buf0 = buffer + y0 + left;
            uint8_t *buf1 = buf0 + Pcd8544::SCREEN_WIDTH;

            const uint8_t *ptr0 = data + (left - x);
            const uint8_t *ptr1 = ptr0;

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
            const Pcd8544::Sprite *sprite = sprites + i;
            switch (sprite->flags)
            {
            case Pcd8544::Sprite::Flag::ENABLED:
                draw_8x8(sprite->x, sprite->y, sprite->data);
                break;

            case Pcd8544::Sprite::Flag::ENABLED | Pcd8544::Sprite::Flag::ALPHA:
                draw_8x8_alpha(sprite->x, sprite->y, sprite->alpha, sprite->data);
                break;

            case Pcd8544::Sprite::Flag::ENABLED | Pcd8544::Sprite::Flag::XCLIP:
                draw_8x8_clip(sprite->x, sprite->y, sprite->data);
                break;
            }
        }
    }

    Pcd8544::Pcd8544(const uint8_t dcPin, const uint8_t resetPin, const uint8_t ssPin) : DC_PIN(dcPin),
                                                                                         RESET_PIN(resetPin),
                                                                                         SS_PIN(ssPin)
    {
    }

    void Pcd8544::setup() const
    {
        pinMode(DC_PIN, OUTPUT);
        pinMode(RESET_PIN, OUTPUT);

        SPI.begin(); // Initializes the SPI bus by setting SCK, MOSI, and SS (not SS_PIN) to outputs, pulling SCK and MOSI low, and SS (not SS_PIN) high.
        pinMode(SS_PIN, OUTPUT);

        digitalWrite(RESET_PIN, LOW);
        digitalWrite(RESET_PIN, HIGH);

        digitalWrite(SS_PIN, LOW); // Activate

        digitalWrite(DC_PIN, LOW); // DC pin is low for commands
        SPI.beginTransaction(SPI_SETTINGS);
        SPI.transfer(0x21); // LCD extended commands
        SPI.transfer(0xB8); // set LCD Vop (contrast)
        SPI.transfer(0x04); // set temp coefficient
        SPI.transfer(0x14); // LCD bias mode 1:40
        SPI.transfer(0x20); // LCD basic commands
        SPI.transfer(0x0C); // normal display mode
        SPI.transfer(0x40); // set y to 0
        SPI.transfer(0x80); // set x to 0
        SPI.endTransaction();

        digitalWrite(SS_PIN, HIGH); // Deactivate

        clearBackground();
        clearBuffer();

        displayBuffer();
    }

    void Pcd8544::render() const
    {
        clearBuffer();

        for (uint8_t i = 0; i < MAX_SPRITE_COUNT; i++)
        {
            draw_sprite(i);
        }

        displayBuffer();
    }

    void Pcd8544::set_sprite(const uint8_t id, const Pcd8544::Sprite sprite)
    {
        sprites[id] = sprite;
    }

    void Pcd8544::clearBackground() const
    {
        for (uint16_t i = 0; i < BUFFER_SIZE; i++)
        {
            background[i] = 0;
        }
    }

    void Pcd8544::setBackground(const uint8_t *background_) const
    {
        for (uint16_t i = 0; i < BUFFER_SIZE; i++)
        {
            background[i] = background_[i];
        }
    }

    void Pcd8544::clearBuffer() const
    {
        for (uint16_t i = 0; i < BUFFER_SIZE; i++)
        {
            buffer[i] = background[i];
        }
    }

    void Pcd8544::displayBuffer() const
    {
        digitalWrite(SS_PIN, LOW); // Activate
        SPI.beginTransaction(SPI_SETTINGS);

        // Draw buffer
        digitalWrite(DC_PIN, HIGH); // DC pin is high for data
        SPI.transfer(buffer, BUFFER_SIZE);

        SPI.endTransaction();
        digitalWrite(SS_PIN, HIGH); // Deactivate
    }
}
