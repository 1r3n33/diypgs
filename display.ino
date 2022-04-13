#include "pcd8544.h"

uint8_t gfx_ball[8] = {0x3C, 0x7A, 0xFD, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C};

class Ball
{
private:
  int8_t x;
  int8_t y;
  PCD8544::sprite_t sprite;

public:
  Ball()
  {
    x = (PCD8544::SCREEN_WIDTH - 8) / 2;
    y = (PCD8544::SCREEN_HEIGHT - 8) / 2;

    sprite = {0x01, uint8_t(x), uint8_t(y), gfx_ball};
  }

  void move(int8_t dx, int8_t dy)
  {
    x += dx;
    y += dy;

    if (x < 0)
    {
      x = 0;
    }
    if (x > PCD8544::SCREEN_WIDTH - 8)
    {
      x = PCD8544::SCREEN_WIDTH - 8;
    }

    if (y < 0)
    {
      y = 0;
    }
    if (y > PCD8544::SCREEN_HEIGHT - 8)
    {
      y = PCD8544::SCREEN_HEIGHT - 8;
    }

    sprite.x = x;
    sprite.y = y;

    PCD8544::set_sprite(0, sprite);
  }
};

uint8_t gfx_paddle_left_top[8] = {0xF0, 0x0C, 0xF2, 0xFA, 0xFD, 0xFD, 0xFD, 0xFF};
uint8_t gfx_paddle_left_bottom[8] = {0x0F, 0x30, 0x4F, 0x5F, 0xBF, 0xBF, 0xBF, 0xFF};

uint8_t gfx_paddle_right_top[8] = {0xFF, 0xFD, 0xFD, 0xFD, 0xFA, 0xF2, 0x0C, 0xF0};
uint8_t gfx_paddle_right_bottom[8] = {0xFF, 0xBF, 0xBF, 0xBF, 0x5F, 0x4F, 0x30, 0x0F};

class Paddle
{
private:
  int8_t x;
  int8_t y;
  uint8_t sprite_ids[2];
  PCD8544::sprite_t sprites[2];

public:
  Paddle(uint8_t left)
  {
    x = left ? 0 : (PCD8544::SCREEN_WIDTH - 8);
    y = (PCD8544::SCREEN_HEIGHT - 16) / 2;

    sprite_ids[0] = left ? 2 : 4;
    sprite_ids[1] = left ? 3 : 5;

    sprites[0] = {0x01, uint8_t(x), uint8_t(y), left ? gfx_paddle_left_top : gfx_paddle_right_top};
    sprites[1] = {0x01, uint8_t(x), uint8_t(y + 8), left ? gfx_paddle_left_bottom : gfx_paddle_right_bottom};
  }

  void move(int8_t d)
  {
    y += d;
    if (y < 0)
    {
      y = 0;
    }
    if (y > PCD8544::SCREEN_HEIGHT - 16)
    {
      y = PCD8544::SCREEN_HEIGHT - 16;
    }

    sprites[0].y = y;
    sprites[1].y = y + 8;

    PCD8544::set_sprite(sprite_ids[0], sprites[0]);
    PCD8544::set_sprite(sprite_ids[1], sprites[1]);
  }
};

Ball ball;
Paddle left(0x1);
Paddle right(0x0);

void setup()
{
  PCD8544::setup();
  ball.move(0, 0);
  left.move(0);
  right.move(0);
}

uint8_t tick = 10;
int8_t deltas[2] = {1, -1};

void loop()
{
  ball.move(deltas[0], deltas[1]);
  left.move(deltas[0]);
  right.move(deltas[1]);

  tick++;
  if (tick > 20)
  {
    tick = 0;
    deltas[0] = -deltas[0];
    deltas[1] = -deltas[1];
  }

  PCD8544::render();
  delay(100);
}
