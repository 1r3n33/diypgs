#pragma once

#include "../hw/Pcd8544.h"

#include "../sdk/Collision.h"
#include "../sdk/Vector.h"

namespace
{
  constexpr uint8_t GFX_BALL[8] = {0x3C, 0x7A, 0xFD, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C};

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
  constexpr int16_t speed_x[8] = {341, 315, 241, 131, 0, 0, 0, 0};
  constexpr int16_t speed_y[8] = {0, 131, 241, 315, 341, 0, 0, 0};
  constexpr int16_t speed_inc_x[8] = {17, 16, 12, 7, 0, 0, 0, 0};
  constexpr int16_t speed_inc_y[8] = {0, 7, 12, 16, 17, 0, 0, 0};

  constexpr uint8_t MAX_SPEED_INC = 50;

  inline sdk::fixed16_t ComputeDx(const uint8_t id, const uint8_t inc, const int8_t d)
  {
    const int16_t s = speed_x[id] + (inc * speed_inc_x[id]);
    return sdk::fixed16_t(0, d >= 0 ? s : -s);
  }

  inline sdk::fixed16_t ComputeDy(const uint8_t id, const uint8_t inc, const int8_t d)
  {
    const int16_t s = speed_y[id] + (inc * speed_inc_y[id]);
    return sdk::fixed16_t(0, d >= 0 ? s : -s);
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
  constexpr uint8_t x_reflect_id[8] = {3, 3, 2, 2, 7, 7, 7, 7};
  constexpr uint8_t y_reflect_id[16] = {0, 1, 1, 2, 2, 2, 3, 3, 7, 7, 7, 7, 7, 7, 7, 7};

  // Goal
  constexpr sdk::fixed16_t GOAL_LEFT_POS = sdk::fixed16_t(-16);
  constexpr sdk::fixed16_t GOAL_RIGHT_POS = sdk::fixed16_t((hw::Pcd8544::SCREEN_WIDTH + 16));

  // Bounce
  constexpr sdk::fixed16_t BOUNCE_TOP_POS = sdk::fixed16_t(0);
  constexpr sdk::fixed16_t BOUNCE_BOTTOM_POS = sdk::fixed16_t(((hw::Pcd8544::SCREEN_HEIGHT - 8) + 1));
}

class Ball
{
public:
  enum Flag : uint8_t
  {
    ENABLED = 0x01
  };

  enum Result : uint8_t
  {
    None = 0x0,
    Goal = 0x01,
    Bounce = 0x02,
  };

  uint8_t flags;
  sdk::Vec2f16 pos;

private:
  uint8_t spriteId;

  sdk::Vec2f16 dxdy;

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

  void setup(const uint8_t id)
  {
    flags = 0;

    pos.x = sdk::fixed16_t(0);
    pos.y = sdk::fixed16_t(0);

    speed_id = 0;
    speed_inc = 0;
    speed_dir_x = 0;
    speed_dir_y = 0;

    dxdy.x = sdk::fixed16_t(0);
    dxdy.y = sdk::fixed16_t(0);

    no_collision_counter = 0;

    spriteId = id;
    sprite = {hw::Pcd8544::Sprite::Flag::XCLIP, 0, 0, 0xFF, GFX_BALL};
    hw::Pcd8544::set_sprite(spriteId, sprite);
  }

  void activate(const sdk::Vec2f16 &pos_)
  {
    flags |= Flag::ENABLED;

    pos = pos_;

    const int rnd = rand();

    speed_id = rnd % 4;
    speed_inc = 1;
    speed_dir_x = rnd & 1 ? +1 : -1;
    speed_dir_y = rnd & 1 ? -1 : +1;

    dxdy.x = ComputeDx(speed_id, speed_inc, speed_dir_x);
    dxdy.y = ComputeDy(speed_id, speed_inc, speed_dir_y);

    no_collision_counter = 0;

    sprite.flags |= hw::Pcd8544::Sprite::Flag::ENABLED;
    sprite.x = pos.x.toInt8();
    sprite.y = pos.y.toInt8();

    hw::Pcd8544::set_sprite(spriteId, sprite);
  }

  void deactivate()
  {
    flags &= ~Flag::ENABLED;
    sprite.flags &= ~hw::Pcd8544::Sprite::Flag::ENABLED;

    hw::Pcd8544::set_sprite(spriteId, sprite);
  }

  uint8_t preCollisionUpdate()
  {
    uint8_t res = Result::None;

    pos += dxdy;

    // Check score
    if (pos.x < GOAL_LEFT_POS || pos.x > GOAL_RIGHT_POS)
    {
      return Result::Goal;
    }

    // Check collision
    if (pos.y < BOUNCE_TOP_POS)
    {
      pos.y = -pos.y;
      speed_dir_y = -speed_dir_y;
      dxdy.y = ComputeDy(speed_id, speed_inc, speed_dir_y);
      res = Result::Bounce;
    }

    if (pos.y > BOUNCE_BOTTOM_POS)
    {
      pos.y = -pos.y + (BOUNCE_BOTTOM_POS * 2);
      speed_dir_y = -speed_dir_y;
      dxdy.y = ComputeDy(speed_id, speed_inc, speed_dir_y);
      res = Result::Bounce;
    }

    sprite.x = pos.x.toInt8();
    sprite.y = pos.y.toInt8();
    hw::Pcd8544::set_sprite(spriteId, sprite);

    if (no_collision_counter)
    {
      no_collision_counter--;
    }

    return res;
  }

  void postCollisionUpdate(const sdk::CollisionResult res)
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
      pos.y -= sdk::fixed16_t(res.depth);
      break;

    case sdk::CollisionResult::Axis::Y:
      speed_id = res.dist < 0 ? y_reflect_id[-res.dist] : y_reflect_id[res.dist];
      speed_dir_x = -speed_dir_x;
      speed_dir_y = (res.dist == 0) ? 0 : ((res.dist > 0) ? 1 : -1);
      pos.x -= sdk::fixed16_t(res.depth);
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

    dxdy.x = ComputeDx(speed_id, speed_inc, speed_dir_x);
    dxdy.y = ComputeDy(speed_id, speed_inc, speed_dir_y);

    sprite.x = pos.x.toInt8();
    sprite.y = pos.y.toInt8();
    hw::Pcd8544::set_sprite(spriteId, sprite);

    no_collision_counter = NO_COLLISION_COUNT;
  }

  void accelerate()
  {
    speed_inc = min(speed_inc + 10, MAX_SPEED_INC);
    dxdy.x = ComputeDx(speed_id, speed_inc, speed_dir_x);
    dxdy.y = ComputeDy(speed_id, speed_inc, speed_dir_y);
  }
};
