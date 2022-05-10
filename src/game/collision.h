#ifndef COLLISION_H
#define COLLISION_H

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

static const CollisionResult NO_COLLISION = {0, 0, 0, 0};

CollisionResult compute_collision(uint8_t circle_center_x,
                                  uint8_t circle_center_y,
                                  uint8_t circle_radius,
                                  uint8_t rect_center_x,
                                  uint8_t rect_center_y,
                                  uint8_t rect_half_width,
                                  uint8_t rect_half_height)
{
    int8_t dx = int8_t(circle_center_x) - int8_t(rect_center_x);
    int16_t abs_dx = dx >= 0 ? dx : -dx;

    int8_t dy = int8_t(circle_center_y) - int8_t(rect_center_y);
    int16_t abs_dy = dy >= 0 ? dy : -dy;

    if (abs_dx > int16_t(rect_half_width + circle_radius))
        return NO_COLLISION;
    if (abs_dy > int16_t(rect_half_height + circle_radius))
        return NO_COLLISION;

    int16_t squared_x = (abs_dx - int16_t(rect_half_width)) * (abs_dx - int16_t(rect_half_width));
    int16_t squared_y = (abs_dy - int16_t(rect_half_height)) * (abs_dy - int16_t(rect_half_height));

    if ((squared_x + squared_y) <= int16_t(circle_radius * circle_radius))
    {
        return CollisionResult{CollisionResult::Axis::X | CollisionResult::Axis::Y,
                               0,
                               0,
                               uint8_t((dy < 0 ? 0 : 2) | (dx < 0 ? 0 : 1))};
    }
    else if (abs_dx <= int16_t(rect_half_width + circle_radius) && abs_dy <= int16_t(rect_half_height))
    {
        return CollisionResult{CollisionResult::Axis::Y, 0, dy, 0};
    }
    else if (abs_dy <= int16_t(rect_half_height + circle_radius) && abs_dx <= int16_t(rect_half_width))
    {
        return CollisionResult{CollisionResult::Axis::X, 0, dx, 0};
    }

    return NO_COLLISION;
}

#endif // COLLISION_H
