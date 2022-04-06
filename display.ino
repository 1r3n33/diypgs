#include "SPI.h"

#define RST 12
#define DC 10

SPISettings spi_settings(16 * 1000000, MSBFIRST, SPI_MODE0);

uint8_t sphere_gfx[8] = {0x3C, 0x7A, 0xFD, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C};

uint8_t paddle_left_top_gfx[8] = {0xF0, 0x0C, 0xF2, 0xFA, 0xFD, 0xFD, 0xFD, 0xFF};
uint8_t paddle_left_body_gfx[8] = {0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t paddle_left_bottom_gfx[8] = {0x0F, 0x30, 0x4F, 0x5F, 0xBF, 0xBF, 0xBF, 0xFF};

uint8_t paddle_right_top_gfx[8] = {0xFF, 0xFD, 0xFD, 0xFD, 0xFA, 0xF2, 0x0C, 0xF0};
uint8_t paddle_right_body_gfx[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF};
uint8_t paddle_right_bottom_gfx[8] = {0xFF, 0xBF, 0xBF, 0xBF, 0x5F, 0x4F, 0x30, 0x0F};

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
  SPI.endTransaction();
  digitalWrite(SS, HIGH); // Deactivate

  // Clear display data ram
  digitalWrite(SS, LOW); // Activate

  digitalWrite(DC, LOW); // DC pin is low for commands
  SPI.beginTransaction(spi_settings);
  SPI.transfer(0x20); // LCD basic commands
  SPI.transfer(0x0C); // normal display mode
  SPI.transfer(0x40); // y 0
  SPI.transfer(0x80); // x 0
  SPI.endTransaction();

  digitalWrite(DC, HIGH); // DC pin is high for data
  SPI.beginTransaction(spi_settings);
  for (int i = 0; i < 48 * 84; i++)
  {
    SPI.transfer(0x00);
  }
  SPI.endTransaction();

  digitalWrite(SS, HIGH); // Deactivate
}

void draw_8x8(uint8_t x, uint8_t y, uint8_t *gfx)
{
  digitalWrite(DC, LOW); // DC pin is low for commands
  SPI.beginTransaction(spi_settings);
  SPI.transfer(0x20);     // LCD basic commands
  SPI.transfer(0x0C);     // normal display mode
  SPI.transfer(0x40 | y); // y
  SPI.transfer(0x80 | x); // x
  SPI.endTransaction();

  digitalWrite(DC, HIGH); // DC pin is high for data
  SPI.beginTransaction(spi_settings);

  uint8_t buf[8];
  memcpy(buf, gfx, 8);
  SPI.transfer(buf, 8);

  SPI.endTransaction();
}

void loop()
{
  // Draw sprites
  digitalWrite(SS, LOW); // Activate

  draw_8x8(32, 1, sphere_gfx);

  draw_8x8(0, 0, paddle_left_top_gfx);
  draw_8x8(0, 1, paddle_left_body_gfx);
  draw_8x8(0, 2, paddle_left_bottom_gfx);

  draw_8x8(76, 3, paddle_right_top_gfx);
  draw_8x8(76, 4, paddle_right_bottom_gfx);

  digitalWrite(SS, HIGH); // Deactivate
}
