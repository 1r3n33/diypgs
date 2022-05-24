#include "Collision.h"

namespace sdk
{
    namespace
    {
        constexpr CollisionResult NO_COLLISION = {0, 0, 0, 0};
    }

    CollisionResult ComputeCollision(uint8_t circle_center_x,
                                     uint8_t circle_center_y,
                                     uint8_t circle_radius,
                                     uint8_t rect_center_x,
                                     uint8_t rect_center_y,
                                     uint8_t rect_half_width,
                                     uint8_t rect_half_height)
    {
        // Check if circle penetrates in a rectangle axis
        int8_t dx = int8_t(circle_center_x) - int8_t(rect_center_x);
        int16_t abs_dx = dx >= 0 ? dx : -dx;

        int8_t dy = int8_t(circle_center_y) - int8_t(rect_center_y);
        int16_t abs_dy = dy >= 0 ? dy : -dy;

        int8_t depth_x = int16_t(rect_half_width + circle_radius) - abs_dx;
        int8_t depth_y = int16_t(rect_half_height + circle_radius) - abs_dy;

        // Early out
        if (depth_x <= 0)
            return NO_COLLISION;
        if (depth_y <= 0)
            return NO_COLLISION;

        // Check if circle penetrates in a rectangle corner
        int16_t dist_x = int16_t(abs_dx - rect_half_width);
        int16_t dist_y = int16_t(abs_dy - rect_half_height);

        int16_t squared_x = dist_x * dist_x;
        int16_t squared_y = dist_y * dist_y;

        // Corner contact has priority over axis contact
        if ((squared_x + squared_y) <= int16_t(circle_radius * circle_radius))
        {
            return CollisionResult{CollisionResult::Axis::X | CollisionResult::Axis::Y,
                                   1,
                                   0,
                                   uint8_t((dy < 0 ? 0 : 2) | (dx < 0 ? 0 : 1))};
        }
        // Axis contact
        else if (depth_x > 0 && abs_dy <= int16_t(rect_half_height))
        {
            return CollisionResult{CollisionResult::Axis::Y, int8_t(dx < 0 ? depth_x : -depth_x), dy, 0};
        }
        else if (depth_y > 0 && abs_dx <= int16_t(rect_half_width))
        {
            return CollisionResult{CollisionResult::Axis::X, int8_t(dy < 0 ? depth_y : -depth_y), dx, 0};
        }

        return NO_COLLISION;
    }
}
