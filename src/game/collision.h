#ifndef COLLISION_H
#define COLLISION_H

const uint16_t COLLISION_NONE = 0x0000;
const uint16_t COLLISION_AXIS_X = 0x0100;
const uint16_t COLLISION_AXIS_Y = 0x0200;

// Compute circle to rect collision.
//
// Return collision data:
// - col_data & 0xFF00 =>
//   - 0x0000: no collision
//
//   - 0x0100: x axis collision
//     - col_data & 0x00FF => dx
//
//   - 0x0200: y axis collision
//     - col_data & 0x00FF => dy
//
//   - 0x0300: CORNERS
//     - col_data & 0x00FF =>
//       - 0x00 => top-left corner
//       - 0x01 => top-right corner
//       - 0x10 => bottom-left corner
//       - 0x11 => bottom-right corner
//
uint16_t compute_collision(uint8_t circle_center_x,
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
        return COLLISION_NONE;
    if (abs_dy > int16_t(rect_half_height + circle_radius))
        return COLLISION_NONE;

    int16_t squared_x = (abs_dx - int16_t(rect_half_width)) * (abs_dx - int16_t(rect_half_width));
    int16_t squared_y = (abs_dy - int16_t(rect_half_height)) * (abs_dy - int16_t(rect_half_height));

    if ((squared_x + squared_y) <= int16_t(circle_radius * circle_radius))
    {
        return COLLISION_AXIS_X | COLLISION_AXIS_Y | (dy < 0 ? 0x00 : 0x10) | (dx < 0 ? 0x00 : 0x01);
    }
    else if (abs_dx <= int16_t(rect_half_width + circle_radius) && abs_dy <= int16_t(rect_half_height))
    {
        return COLLISION_AXIS_Y | uint8_t(dy);
    }
    else if (abs_dy <= int16_t(rect_half_height + circle_radius) && abs_dx <= int16_t(rect_half_width))
    {
        return COLLISION_AXIS_X | uint8_t(dx);
    }

    return COLLISION_NONE;
}

#endif // COLLISION_H
