#include "Paddle.h"

namespace game
{
    namespace
    {
        constexpr uint8_t GFX_PADDLE_LEFT_TOP[8] = {0xF0, 0x0C, 0xF2, 0xFA, 0xFD, 0xFD, 0xFD, 0xFF};
        constexpr uint8_t GFX_PADDLE_LEFT_BOTTOM[8] = {0x0F, 0x30, 0x4F, 0x5F, 0xBF, 0xBF, 0xBF, 0xFF};

        constexpr uint8_t GFX_PADDLE_RIGHT_TOP[8] = {0xFF, 0xFD, 0xFD, 0xFD, 0xFA, 0xF2, 0x0C, 0xF0};
        constexpr uint8_t GFX_PADDLE_RIGHT_BOTTOM[8] = {0xFF, 0xBF, 0xBF, 0xBF, 0x5F, 0x4F, 0x30, 0x0F};

        constexpr uint8_t GFX_SMALL_PADDLE_LEFT[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x7E, 0xFF};
        constexpr uint8_t GFX_SMALL_PADDLE_RIGHT[8] = {0xFF, 0x7E, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00};
    }

    void Paddle::setup(const Side side_)
    {
        side = side_;

        x = (side == Side::LEFT) ? 0 : (hw::Pcd8544::SCREEN_WIDTH - 8);
        y = (hw::Pcd8544::SCREEN_HEIGHT - 16) / 2;

        size = Size::NORMAL;
        inverted = 0;

        sprite_ids[0] = (side == Side::LEFT) ? 4 : 5;
        sprite_ids[1] = (side == Side::LEFT) ? 6 : 7;

        sprites[0] = {hw::Pcd8544::Sprite::Flag::ENABLED, x, y, 0xFF, (side == Side::LEFT) ? GFX_PADDLE_LEFT_TOP : GFX_PADDLE_RIGHT_TOP};
        sprites[1] = {hw::Pcd8544::Sprite::Flag::ENABLED, x, int8_t(y + 8), 0xFF, (side == Side::LEFT) ? GFX_PADDLE_LEFT_BOTTOM : GFX_PADDLE_RIGHT_BOTTOM};

        hw::Pcd8544::set_sprite(sprite_ids[0], sprites[0]);
        hw::Pcd8544::set_sprite(sprite_ids[1], sprites[1]);
    }

    void Paddle::move(const int8_t dy)
    {
        y = inverted ? y - dy : y + dy;
        y = max(y, 0);

        const uint8_t maxY = hw::Pcd8544::SCREEN_HEIGHT - getHeight();
        y = min(y, maxY);

        sprites[0].y = y;
        sprites[1].y = y + 8;

        hw::Pcd8544::set_sprite(sprite_ids[0], sprites[0]);
        hw::Pcd8544::set_sprite(sprite_ids[1], sprites[1]);
    }

    void Paddle::target(const uint8_t target)
    {
        const uint8_t halfHeight = getHeight() / 2;
        const uint8_t paddleCenter = y + halfHeight;
        if (paddleCenter < target)
        {
            move(+1);
        }
        else if (paddleCenter > target)
        {
            move(-1);
        }
    }

    void Paddle::invert()
    {
        inverted = !inverted;
    }

    // Decide the new size based on the paddle y pos.
    void Paddle::changeSize()
    {
        if (size == Size::NORMAL)
        {
            size = Size::SMALL;

            y += 4;

            sprites[0].y = y;
            sprites[0].data = (side == Side::LEFT) ? GFX_SMALL_PADDLE_LEFT : GFX_SMALL_PADDLE_RIGHT;

            sprites[1].flags &= ~hw::Pcd8544::Sprite::ENABLED;

            hw::Pcd8544::set_sprite(sprite_ids[0], sprites[0]);
            hw::Pcd8544::set_sprite(sprite_ids[1], sprites[1]);
        }
        else
        {
            size = Size::NORMAL;

            y = max(0, y - 4);

            sprites[0].y = y;
            sprites[0].data = (side == Side::LEFT) ? GFX_PADDLE_LEFT_TOP : GFX_PADDLE_RIGHT_TOP;

            sprites[1].y = y + 8;
            sprites[1].flags |= hw::Pcd8544::Sprite::ENABLED;

            hw::Pcd8544::set_sprite(sprite_ids[0], sprites[0]);
            hw::Pcd8544::set_sprite(sprite_ids[1], sprites[1]);
        }
    }

    uint8_t Paddle::getWidth() const
    {
        return (size == Size::NORMAL) ? 8 : 4;
    }

    uint8_t Paddle::getHeight() const
    {
        return (size == Size::NORMAL) ? 16 : 8;
    }

    uint8_t Paddle::getCenterX() const
    {
        const uint8_t offset = (size == Size::NORMAL) ? 4 : ((side == Side::LEFT) ? 6 : 2);
        return x + offset;
    }

    uint8_t Paddle::getCenterY() const
    {
        const uint8_t offset = (size == Size::NORMAL) ? 8 : 4;
        return y + offset;
    }
}
