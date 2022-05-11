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
// compute_collision(...) returns the distance from the non-collided axis.
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

  uint8_t no_collision_counter;

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

    no_collision_counter = 0;

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

    if (no_collision_counter)
    {
      no_collision_counter--;
    }

    return res;
  }

  void post_collision_update(const CollisionResult res)
  {
    static const uint8_t NO_COLLISION_COUNT = 8;
    static const uint8_t MAX_SPEED_MUL = 50;

    int8_t d, id;
    if (no_collision_counter > 0)
    {
      return;
    }

    switch (res.axis)
    {
    case CollisionResult::Axis::NONE:
      return;

    case CollisionResult::Axis::X:
      speed_mul = min(speed_mul + 1, MAX_SPEED_MUL);
      no_collision_counter = NO_COLLISION_COUNT;
      d = res.dist;
      id = d < 0 ? x_reflect_id[-d] : x_reflect_id[d];
      dx = d < 0 ? -(speed_x[id] + (speed_mul * speed_inc_x[id])) : (speed_x[id] + (speed_mul * speed_inc_x[id]));
      dy = dy < 0 ? (speed_y[id] + (speed_mul * speed_inc_y[id])) : -(speed_y[id] + (speed_mul * speed_inc_y[id]));
      y = res.depth < 0 ? y + (uint8_t(-res.depth) << 8) : y - (uint8_t(res.depth) << 8);
      break;

    case CollisionResult::Axis::Y:
      speed_mul = min(speed_mul + 1, MAX_SPEED_MUL);
      no_collision_counter = NO_COLLISION_COUNT;
      d = res.dist;
      id = d < 0 ? y_reflect_id[-d] : y_reflect_id[d];
      dx = dx < 0 ? (speed_x[id] + (speed_mul * speed_inc_x[id])) : -(speed_x[id] + (speed_mul * speed_inc_x[id]));
      dy = d < 0 ? -(speed_y[id] + (speed_mul * speed_inc_y[id])) : (speed_y[id] + (speed_mul * speed_inc_y[id]));
      x = res.depth < 0 ? x + (uint8_t(-res.depth) << 8) : x - (uint8_t(res.depth) << 8);
      break;

    case CollisionResult::Axis::X | CollisionResult::Axis::Y:
      speed_mul = min(speed_mul + 1, MAX_SPEED_MUL);
      no_collision_counter = NO_COLLISION_COUNT;
      switch (res.corner)
      {
      case CollisionResult::Corner::TOP_LEFT:
        dx = -(speed_x[3] + (speed_mul * speed_inc_x[3]));
        dy = -(speed_y[3] + (speed_mul * speed_inc_y[3]));
        break;

      case CollisionResult::Corner::TOP_RIGHT:
        dx = (speed_x[3] + (speed_mul * speed_inc_x[3]));
        dy = -(speed_y[3] + (speed_mul * speed_inc_y[3]));
        break;

      case CollisionResult::Corner::BOTTOM_LEFT:
        dx = -(speed_x[3] + (speed_mul * speed_inc_x[3]));
        dy = (speed_y[3] + (speed_mul * speed_inc_y[3]));
        break;

      case CollisionResult::Corner::BOTTOM_RIGHT:
        dx = (speed_x[3] + (speed_mul * speed_inc_x[3]));
        dy = (speed_y[3] + (speed_mul * speed_inc_y[3]));
        break;
      }
      break;
    }

    sprite.x = x >> 8;
    sprite.y = y >> 8;
    PCD8544::set_sprite(0, sprite);
  }
};

#endif
