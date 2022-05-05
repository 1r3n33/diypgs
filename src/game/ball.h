#ifndef BALL_H
#define BALL_H

#include "../sdk/pcd8544.h"
#include "collision.h"

uint8_t gfx_ball[8] = {0x3C, 0x7A, 0xFD, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C};

// Ball speeds
// Computed by ./tools/vec2fixed16 5 40 30 0.05
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
int16_t speed_inc_x[8] = {17, 16, 12, 7, 0, 0, 0, 0};
int16_t speed_inc_y[8] = {0, 7, 12, 16, 17, 0, 0, 0};
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
  enum Result : uint8_t
  {
    None = 0x0,
    Score = 0x01,
    Collision = 0x02,
  };

  int16_t x;
  int16_t y;

private:
  int16_t dx;
  int16_t dy;

  uint8_t speed_mul;

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

    // Go left (Player)
    dx = -speed_x[0];
    dy = speed_y[0];
    speed_mul = 1;

    no_collision_timer = 0;

    sprite = {PCD8544::sprite_t::Flag::ENABLED | PCD8544::sprite_t::Flag::XCLIP, uint8_t(x), uint8_t(y), gfx_ball};

    sprite.x = x >> 8;
    sprite.y = y >> 8;

    PCD8544::set_sprite(0, sprite);
  }

  uint8_t update()
  {
    uint8_t res = Result::None;

    x += dx;
    y += dy;

    // Handle collisions.
    if (x < -16 * 256)
    {
      x = -x;
      dx = -dx;
      res = Result::Score;
    }

    int16_t max_x = ((PCD8544::SCREEN_WIDTH + 8) << 8) | 0xFF;
    if (x > max_x)
    {
      x = -x + (2 * max_x);
      dx = -dx;
      res = Result::Score;
    }

    if (y < 0)
    {
      y = -y;
      dy = -dy;
      res = Result::Collision;
    }

    int16_t max_y = ((PCD8544::SCREEN_HEIGHT - 8) << 8) | 0xFF;
    if (y > max_y)
    {
      y = -y + (2 * max_y);
      dy = -dy;
      res = Result::Collision;
    }

    sprite.x = x >> 8;
    sprite.y = y >> 8;

    PCD8544::set_sprite(0, sprite);

    if (no_collision_timer)
    {
      no_collision_timer--;
    }

    return res;
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
      speed_mul++;
      no_collision_timer = 32;
      d = (col_data & 0x00FF);
      id = d < 0 ? x_reflect_id[-d] : x_reflect_id[d];
      dx = d < 0 ? -(speed_x[id] + (speed_mul * speed_inc_x[id])) : (speed_x[id] + (speed_mul * speed_inc_x[id]));
      dy = dy < 0 ? (speed_y[id] + (speed_mul * speed_inc_y[id])) : -(speed_y[id] + (speed_mul * speed_inc_y[id]));
      return;

    case COLLISION_AXIS_Y:
      speed_mul++;
      no_collision_timer = 32;
      d = (col_data & 0x00FF);
      id = d < 0 ? y_reflect_id[-d] : y_reflect_id[d];
      dx = dx < 0 ? (speed_x[id] + (speed_mul * speed_inc_x[id])) : -(speed_x[id] + (speed_mul * speed_inc_x[id]));
      dy = d < 0 ? -(speed_y[id] + (speed_mul * speed_inc_y[id])) : (speed_y[id] + (speed_mul * speed_inc_y[id]));
      return;

    case COLLISION_AXIS_X | COLLISION_AXIS_Y:
      speed_mul++;
      no_collision_timer = 32;
      switch (col_data & 0x00FF)
      {
      case 0x00:
        dx = -(speed_x[3] + (speed_mul * speed_inc_x[3]));
        dy = -(speed_y[3] + (speed_mul * speed_inc_y[3]));
        return;

      case 0x01:
        dx = (speed_x[3] + (speed_mul * speed_inc_x[3]));
        dy = -(speed_y[3] + (speed_mul * speed_inc_y[3]));
        return;

      case 0x10:
        dx = -(speed_x[3] + (speed_mul * speed_inc_x[3]));
        dy = (speed_y[3] + (speed_mul * speed_inc_y[3]));
        return;

      case 0x11:
        dx = (speed_x[3] + (speed_mul * speed_inc_x[3]));
        dy = (speed_y[3] + (speed_mul * speed_inc_y[3]));
        return;
      }
    }
  }
};

#endif
