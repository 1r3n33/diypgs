#pragma once

#include "Arduino.h"

namespace sdk
{
    struct CollisionResult
    {
        enum Axis : uint8_t
        {
            NONE = 0x0,
            X = 0x1,
            Y = 0x2,
        };

        enum Corner : uint8_t
        {
            TOP_LEFT = 0,
            TOP_RIGHT = 1,
            BOTTOM_LEFT = 2,
            BOTTOM_RIGHT = 3,
        };

        const uint8_t axis;   // collision axis
        const int8_t depth;   // penetration inside the collided axis. positive is for right and bottom direction.
        const int8_t dist;    // distance from the non-collided axis. positive is for right and bottom position.
        const uint8_t corner; // corner id when collided on both axis
    };

    CollisionResult ComputeCollision(uint8_t circle_center_x,
                                     uint8_t circle_center_y,
                                     uint8_t circle_radius,
                                     uint8_t rect_center_x,
                                     uint8_t rect_center_y,
                                     uint8_t rect_half_width,
                                     uint8_t rect_half_height);
}
