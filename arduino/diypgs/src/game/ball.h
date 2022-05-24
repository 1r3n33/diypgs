#pragma once

#include "../hw/Pcd8544.h"
#include "../sdk/Collision.h"

namespace
{
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

  constexpr uint8_t MAX_SPEED_INC = 50;

  inline int16_t ComputeDx(const uint8_t id, const uint8_t inc, const int8_t d)
  {
    const int16_t s = speed_x[id] + (inc * speed_inc_x[id]);
    return d >= 0 ? s : -s;
  }

  inline int16_t ComputeDy(const uint8_t id, const uint8_t inc, const int8_t d)
  {
    const int16_t s = speed_y[id] + (inc * speed_inc_y[id]);
    return d >= 0 ? s : -s;
  }

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

  // Score
  constexpr int16_t SCORE_LEFT_POS = -16 * 256;
  constexpr int16_t SCORE_RIGHT_POS = ((hw::Pcd8544::SCREEN_WIDTH + 16) * 256);

  // Collision
  constexpr int16_t COLLISION_TOP_POS = 0;
  constexpr int16_t COLLISION_BOTTOM_POS = ((hw::Pcd8544::SCREEN_HEIGHT - 8) << 8) | 0xFF;
}

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

  uint8_t speed_id;
  uint8_t speed_inc;
  int8_t speed_dir_x;
  int8_t speed_dir_y;

  uint8_t no_collision_counter;

  hw::Pcd8544::Sprite sprite;

public:
  Ball()
  {
  }

  void setup()
  {
    x = ((hw::Pcd8544::SCREEN_WIDTH - 8) / 2) << 8;
    y = ((hw::Pcd8544::SCREEN_HEIGHT - 8) / 2) << 8;

    // Go to the player (left)
    speed_id = 0;
    speed_inc = 1;
    speed_dir_x = -1;
    speed_dir_y = 0;
    dx = ComputeDx(speed_id, speed_inc, speed_dir_x);
    dy = ComputeDy(speed_id, speed_inc, speed_dir_y);

    no_collision_counter = 0;

    sprite = {hw::Pcd8544::Sprite::Flag::ENABLED | hw::Pcd8544::Sprite::Flag::XCLIP, uint8_t(x), uint8_t(y), 0xFF, gfx_ball};

    sprite.x = x >> 8;
    sprite.y = y >> 8;
    hw::Pcd8544::set_sprite(0, sprite);
  }

  uint8_t update()
  {
    uint8_t res = Result::None;

    x += dx;
    y += dy;

    // Check score
    if (x < SCORE_LEFT_POS || x > SCORE_RIGHT_POS)
    {
      return Result::Score;
    }

    // Check collision
    if (y < COLLISION_TOP_POS)
    {
      y = -y;
      speed_dir_y = -speed_dir_y;
      dy = ComputeDy(speed_id, speed_inc, speed_dir_y);
      res = Result::Collision;
    }

    if (y > COLLISION_BOTTOM_POS)
    {
      y = -y + (2 * COLLISION_BOTTOM_POS);
      speed_dir_y = -speed_dir_y;
      dy = ComputeDy(speed_id, speed_inc, speed_dir_y);
      res = Result::Collision;
    }

    sprite.x = x >> 8;
    sprite.y = y >> 8;
    hw::Pcd8544::set_sprite(0, sprite);

    if (no_collision_counter)
    {
      no_collision_counter--;
    }

    return res;
  }

  void post_collision_update(const sdk::CollisionResult res)
  {
    constexpr uint8_t NO_COLLISION_COUNT = 8;

    // Early out if no collision flag is raised or if there is no collision.
    if (no_collision_counter > 0 || res.axis == sdk::CollisionResult::Axis::NONE)
    {
      return;
    }

    switch (res.axis)
    {
    case sdk::CollisionResult::Axis::X:
      speed_id = res.dist < 0 ? x_reflect_id[-res.dist] : x_reflect_id[res.dist];
      speed_dir_y = -speed_dir_y;
      y = res.depth < 0 ? y + (uint8_t(-res.depth) << 8) : y - (uint8_t(res.depth) << 8);
      break;

    case sdk::CollisionResult::Axis::Y:
      speed_id = res.dist < 0 ? y_reflect_id[-res.dist] : y_reflect_id[res.dist];
      speed_dir_x = -speed_dir_x;
      speed_dir_y = (res.dist == 0) ? 0 : ((res.dist > 0) ? 1 : -1);
      x = res.depth < 0 ? x + (uint8_t(-res.depth) << 8) : x - (uint8_t(res.depth) << 8);
      break;

    case sdk::CollisionResult::Axis::X | sdk::CollisionResult::Axis::Y:
      speed_id = 3;
      switch (res.corner)
      {
      case sdk::CollisionResult::Corner::TOP_LEFT:
        speed_dir_x = -1;
        speed_dir_y = -1;
        break;

      case sdk::CollisionResult::Corner::TOP_RIGHT:
        speed_dir_x = +1;
        speed_dir_y = -1;
        break;

      case sdk::CollisionResult::Corner::BOTTOM_LEFT:
        speed_dir_x = -1;
        speed_dir_y = +1;
        break;

      case sdk::CollisionResult::Corner::BOTTOM_RIGHT:
        speed_dir_x = +1;
        speed_dir_y = +1;
        break;
      }
      break;
    }

    speed_inc = min(speed_inc + 1, MAX_SPEED_INC);

    dx = ComputeDx(speed_id, speed_inc, speed_dir_x);
    dy = ComputeDy(speed_id, speed_inc, speed_dir_y);

    sprite.x = x >> 8;
    sprite.y = y >> 8;
    hw::Pcd8544::set_sprite(0, sprite);

    no_collision_counter = NO_COLLISION_COUNT;
  }

  void accelerate()
  {
    speed_inc = min(speed_inc + 10, MAX_SPEED_INC);
    dx = ComputeDx(speed_id, speed_inc, speed_dir_x);
    dy = ComputeDy(speed_id, speed_inc, speed_dir_y);
  }
};
