#include "SPI.h"

#define RST 12
#define DC 10

SPISettings spi_settings(16 * 1000000, MSBFIRST, SPI_MODE0);

uint8_t gfx_ball[8] = {0x3C, 0x7A, 0xFD, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C};

uint8_t gfx_paddle_left_top[8] = {0xF0, 0x0C, 0xF2, 0xFA, 0xFD, 0xFD, 0xFD, 0xFF};
uint8_t gfx_paddle_left_body[8] = {0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t gfx_paddle_left_bottom[8] = {0x0F, 0x30, 0x4F, 0x5F, 0xBF, 0xBF, 0xBF, 0xFF};

uint8_t gfx_paddle_right_top[8] = {0xFF, 0xFD, 0xFD, 0xFD, 0xFA, 0xF2, 0x0C, 0xF0};
uint8_t gfx_paddle_right_body[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF};
uint8_t gfx_paddle_right_bottom[8] = {0xFF, 0xBF, 0xBF, 0xBF, 0x5F, 0x4F, 0x30, 0x0F};

uint8_t gfx_paddle_left_mini[8] = {0x3C, 0x42, 0x5A, 0x5A, 0x99, 0xBD, 0xBD, 0xBD};

uint8_t gfx_bonus_top_left[8] = {0x00, 0x00, 0x10, 0x28, 0x48, 0x90, 0xA0, 0xC0};
uint8_t gfx_bonus_top_right[8] = {0xC0, 0xA0, 0x90, 0x48, 0x28, 0x10, 0x00, 0x00};

uint8_t gfx_bonus_bottom_left[8] = {0x00, 0x7E, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0x80};
uint8_t gfx_bonus_bottom_right[8] = {0x80, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0x7E, 0x00};

#define SCREEN_WIDTH 84

#define MAX_SPRITE_COUNT 16

struct sprite_t
{
  uint8_t flags;
  uint8_t x;
  uint8_t y;
  uint8_t *data;
} sprites[MAX_SPRITE_COUNT] = {0};

#define BUFFER_SIZE (6 * SCREEN_WIDTH)

uint8_t buffer[BUFFER_SIZE] = {0};

void clear_buffer()
{
  for (int16_t i = 0; i < BUFFER_SIZE; i++)
  {
    buffer[i] = 0;
  }
}

void draw_8x8(uint8_t x, uint8_t y, uint8_t *data)
{
  uint8_t mod0 = y % 8;
  int16_t y0 = (y / 8) * SCREEN_WIDTH;
  for (int8_t i = 0; i < 8; i++)
  {
    buffer[y0 + x + i] |= data[i] << mod0;
  }

  uint8_t mod1 = 8 - mod0;
  int16_t y1 = y0 + SCREEN_WIDTH;
  for (int8_t i = 0; i < 8; i++)
  {
    buffer[y1 + x + i] |= data[i] >> mod1;
  }
}

void draw_sprite(uint8_t i)
{
  sprite_t *sprite = sprites + i;
  if (sprite->flags & 1)
  {
    draw_8x8(sprite->x, sprite->y, sprite->data);
  }
}

void display_buffer()
{
  digitalWrite(SS, LOW); // Activate
  SPI.beginTransaction(spi_settings);

  // Draw buffer
  digitalWrite(DC, HIGH); // DC pin is high for data
  SPI.transfer(buffer, BUFFER_SIZE);

  SPI.endTransaction();
  digitalWrite(SS, HIGH); // Deactivate
}

void setup()
{
  pinMode(DC, OUTPUT);
  pinMode(RST, OUTPUT);

  SPI.begin(); // set SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high

  digitalWrite(RST, LOW);
  digitalWrite(RST, HIGH);

  digitalWrite(SS, LOW); // Activate

  digitalWrite(DC, LOW); // DC pin is low for commands
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

  // Init sprites
  sprites[0] = {0x01, 32, 8, gfx_ball};

  sprites[1] = {0x01, 0, 2, gfx_paddle_left_top};
  sprites[2] = {0x01, 0, 10, gfx_paddle_left_body};
  sprites[3] = {0x01, 0, 18, gfx_paddle_left_bottom};

  sprites[4] = {0x01, 0, 36, gfx_paddle_left_mini};

  sprites[5] = {0x01, 76, 24, gfx_paddle_right_top};
  sprites[6] = {0x01, 76, 32, gfx_paddle_right_bottom};

  sprites[7] = {0x01, 40, 24, gfx_bonus_top_left};
  sprites[8] = {0x01, 48, 24, gfx_bonus_top_right};
  sprites[9] = {0x01, 40, 32, gfx_bonus_bottom_left};
  sprites[10] = {0x01, 48, 32, gfx_bonus_bottom_right};
}

void loop()
{
  clear_buffer();

  for (uint8_t i = 0; i < MAX_SPRITE_COUNT; i++)
  {
    draw_sprite(i);
  }

  display_buffer();

  delay(50);
}
