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

#define BUFFER_SIZE (6 * 84)

uint8_t buffer[BUFFER_SIZE];

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
  digitalWrite(DC, HIGH); // DC pin is high for data
  SPI.transfer(buffer, BUFFER_SIZE);

  SPI.endTransaction();
  digitalWrite(SS, HIGH); // Deactivate
}

void draw_8x8(uint8_t x, uint8_t y, uint8_t *gfx)
{
  uint8_t mod0 = y % 8;
  int16_t y0 = (y / 8) * 84;
  for (int8_t i = 0; i < 8; i++)
  {
    buffer[y0 + x + i] |= gfx[i] << mod0;
  }

  uint8_t mod1 = 8 - mod0;
  int16_t y1 = y0 + 84;
  for (int8_t i = 0; i < 8; i++)
  {
    buffer[y1 + x + i] |= gfx[i] >> mod1;
  }
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
}

void loop()
{
  clear_buffer();

  draw_8x8(32, 8, gfx_ball);

  draw_8x8(0, 2, gfx_paddle_left_top);
  draw_8x8(0, 10, gfx_paddle_left_body);
  draw_8x8(0, 18, gfx_paddle_left_bottom);

  draw_8x8(0, 36, gfx_paddle_left_mini);

  draw_8x8(76, 24, gfx_paddle_right_top);
  draw_8x8(76, 32, gfx_paddle_right_bottom);

  draw_8x8(40, 24, gfx_bonus_top_left);
  draw_8x8(48, 24, gfx_bonus_top_right);
  draw_8x8(40, 32, gfx_bonus_bottom_left);
  draw_8x8(48, 32, gfx_bonus_bottom_right);

  display_buffer();

  delay(50);
}
