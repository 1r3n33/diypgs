#ifndef BALL_H
#define BALL_H

#include "../pcd8544/pcd8544.h"

uint8_t gfx_ball[8] = {0x3C, 0x7A, 0xFD, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C};

// speeds for 60fps 40px/s at 3pi/8, 2pi/8, pi/8
int16_t speed_x[4] = {65, 121, 158, 0};
int16_t speed_y[4] = {158, 121, 65, 0};

class Ball
{
public:
  int16_t x;
  int16_t y;

private:
  PCD8544::sprite_t sprite;

public:
  Ball()
  {
  }

  void reset()
  {
    x = ((PCD8544::SCREEN_WIDTH - 8) / 2) << 8;
    y = ((PCD8544::SCREEN_HEIGHT - 8) / 2) << 8;

    sprite = {0x01, uint8_t(x), uint8_t(y), gfx_ball};

    sprite.x = x >> 8;
    sprite.y = y >> 8;

    PCD8544::set_sprite(0, sprite);
  }

  uint8_t update(uint8_t speed_id)
  {
    uint8_t collision = false;

    int16_t sx = speed_x[speed_id];
    int16_t sy = speed_y[speed_id];

    x += sx;
    y += sy;

    // Handle collisions.
    // max_x & max_y are slightly out of bound but give good results on screen.
    if (x < 0)
    {
      x = -x;
      speed_x[speed_id] = -speed_x[speed_id];
      collision = 1;
    }

    int16_t max_x = ((PCD8544::SCREEN_WIDTH - 8) << 8) | 0xFF;
    if (x > max_x)
    {
      x = -x + (2 * max_x);
      speed_x[speed_id] = -speed_x[speed_id];
      collision = 1;
    }

    if (y < 0)
    {
      y = -y;
      speed_y[speed_id] = -speed_y[speed_id];
      collision = 1;
    }

    int16_t max_y = ((PCD8544::SCREEN_HEIGHT - 8) << 8) | 0xFF;
    if (y > max_y)
    {
      y = -y + (2 * max_y);
      speed_y[speed_id] = -speed_y[speed_id];
      collision = 1;
    }

    sprite.x = x >> 8;
    sprite.y = y >> 8;

    PCD8544::set_sprite(0, sprite);

    return collision;
  }
};

#endif
