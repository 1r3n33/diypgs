#ifndef BALL_H
#define BALL_H

#include "../pcd8544/pcd8544.h"
#include "collision.h"

uint8_t gfx_ball[8] = {0x3C, 0x7A, 0xFD, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C};

// Ball speeds
// Computed by ./tools/vec2fixed16 5 40 30 1
// (it produces 5 vectors with a velocity of 40 pixels/sec for a 30fps display)
//
// [4]
//  x   [3]
//  |    x     [2]
//  |          x
//  |               [1]
//  |              x
//  |
// -+----------------x [0]
//  |
//
int16_t speed_x[8] = {341, 315, 241, 131, 0, 0, 0, 0};
int16_t speed_y[8] = {0, 131, 241, 315, 341, 0, 0, 0};

// Reflect ids
// compute_collision(...) returns the dist from the axis.
// Paddle width is 4, height is 8, so compute_collision(...) returns the following:
//
//   0123
//  -+--+ 7
//   |  | 6
//   |  | 5
//   |  | 4
//   |  | 3 <-- get speeds at y_reflect_ids[3]
//   |  | 2
//   |  | 1
// y-o--+ 0
//   |  |
//   x
//
uint8_t x_reflect_id[8] = {3, 3, 2, 2, 7, 7, 7, 7};
uint8_t y_reflect_id[16] = {0, 1, 1, 2, 2, 2, 3, 3, 7, 7, 7, 7, 7, 7, 7, 7};

class Ball
{
public:
  int16_t x;
  int16_t y;

private:
  int16_t dx;
  int16_t dy;

  int8_t no_collision_timer;

  PCD8544::sprite_t sprite;

public:
  Ball()
  {
  }

  void setup()
  {
    x = ((PCD8544::SCREEN_WIDTH - 8) / 2) << 8;
    y = ((PCD8544::SCREEN_HEIGHT - 8) / 2) << 8;

    dx = speed_x[2];
    dy = speed_y[2];

    no_collision_timer = 0;

    sprite = {0x01, uint8_t(x), uint8_t(y), gfx_ball};

    sprite.x = x >> 8;
    sprite.y = y >> 8;

    PCD8544::set_sprite(0, sprite);
  }

  void update()
  {
    x += dx;
    y += dy;

    // Handle collisions.
    // max_x & max_y are slightly out of bound but give good results on screen.
    if (x < 0)
    {
      x = -x;
      dx = -dx;
    }

    int16_t max_x = ((PCD8544::SCREEN_WIDTH - 8) << 8) | 0xFF;
    if (x > max_x)
    {
      x = -x + (2 * max_x);
      dx = -dx;
    }

    if (y < 0)
    {
      y = -y;
      dy = -dy;
    }

    int16_t max_y = ((PCD8544::SCREEN_HEIGHT - 8) << 8) | 0xFF;
    if (y > max_y)
    {
      y = -y + (2 * max_y);
      dy = -dy;
    }

    sprite.x = x >> 8;
    sprite.y = y >> 8;

    PCD8544::set_sprite(0, sprite);

    if (no_collision_timer)
    {
      no_collision_timer--;
    }
  }

  void post_collision_update(uint16_t col_data)
  {
    int8_t d, id;
    if (no_collision_timer)
    {
      return;
    }

    switch (col_data & 0xFF00)
    {
    case 0:
      return;

    case COLLISION_AXIS_X:
      no_collision_timer = 32;
      d = (col_data & 0x00FF);
      id = d < 0 ? x_reflect_id[-d] : x_reflect_id[d];
      dx = d < 0 ? -speed_x[id] : speed_x[id];
      dy = dy < 0 ? speed_y[id] : -speed_y[id];
      return;

    case COLLISION_AXIS_Y:
      no_collision_timer = 32;
      d = (col_data & 0x00FF);
      id = d < 0 ? y_reflect_id[-d] : y_reflect_id[d];
      dx = dx < 0 ? speed_x[id] : -speed_x[id];
      dy = d < 0 ? -speed_y[id] : speed_y[id];
      return;

    case COLLISION_AXIS_X | COLLISION_AXIS_Y:
      no_collision_timer = 32;
      switch (col_data & 0x00FF)
      {
      case 0x00:
        dx = -speed_x[3];
        dy = -speed_y[3];
        return;

      case 0x01:
        dx = speed_x[3];
        dy = -speed_y[3];
        return;

      case 0x10:
        dx = -speed_x[3];
        dy = speed_y[3];
        return;

      case 0x11:
        dx = speed_x[3];
        dy = speed_y[3];
        return;
      }
    }
  }
};

#endif
