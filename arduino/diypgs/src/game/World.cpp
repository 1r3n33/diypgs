#include "World.h"

namespace game
{
    World::World(const hw::Pcd8544 &pcd8544,
                 const hw::Buzzer &buzzer,
                 const hw::Gamepad &gamepad) : graphics(pcd8544),
                                               audio(buzzer),
                                               controller(gamepad)
    {
    }

    void World::setup()
    {
        ball.setup();
        bonus.setup();
        ball_pos.setup((ball.y >> 8) + 4);
        left.setup(1);
        right.setup(0);
    }

    void World::update()
    {
        // Update ball (includes wall collision checks).
        const uint8_t collision = ball.update();
        if (collision & Ball::Result::Collision)
        {
            audio.play(hw::Buzzer::Note::C5, 50);
        }
        if (collision & Ball::Result::Score)
        {
            audio.play(hw::Buzzer::Note::C3, 200);
            delay(500);
            ball.setup();
            bonus.setup();
            ball_pos.setup((ball.y >> 8) + 4);
            graphics.render();
            delay(500);
            return;
        }

        // Paddle collision checks.
        const sdk::CollisionResult left_paddle_col = sdk::ComputeCollision((ball.x >> 8) + 4,
                                                                           (ball.y >> 8) + 4,
                                                                           4,
                                                                           left.x + 4,
                                                                           left.y + 8,
                                                                           4,
                                                                           8);
        if (left_paddle_col.axis)
        {
            ball.post_collision_update(left_paddle_col);
            audio.play(hw::Buzzer::Note::C6, 50);
        }

        const sdk::CollisionResult right_paddle_col = sdk::ComputeCollision((ball.x >> 8) + 4,
                                                                            (ball.y >> 8) + 4,
                                                                            4,
                                                                            right.x + 4,
                                                                            right.y + 8,
                                                                            4,
                                                                            8);
        if (right_paddle_col.axis)
        {
            ball.post_collision_update(right_paddle_col);
            audio.play(hw::Buzzer::Note::C6, 50);
        }

        // Bonus update
        // Must be before player controls update because bonus can affect player controls.
        bonus.update();
        const sdk::CollisionResult bonus_col = sdk::ComputeCollision((ball.x >> 8) + 4,
                                                                     (ball.y >> 8) + 4,
                                                                     4,
                                                                     (bonus.center_x >> 8),
                                                                     (bonus.center_y >> 8) + 4,
                                                                     7,
                                                                     4);
        if (bonus_col.axis && bonus.state == game::Bonus::State::ENABLED)
        {
            audio.play(hw::Buzzer::Note::C7, 50);

            const game::Bonus::Effect effect = bonus.capture();
            switch (effect)
            {
            case game::Bonus::Effect::INVERT_COMMAND_LEFT:
                left.invert();
                break;
            case game::Bonus::Effect::INVERT_COMMAND_RIGHT:
                right.invert();
                break;
            case game::Bonus::Effect::ACCELERATE_BALL:
                ball.accelerate();
                break;
            default:
                break;
            }
        }

        // Player controls the left paddle.
        const uint8_t button_pressed = controller.get();
        if (button_pressed & hw::Gamepad::Button::UP)
            left.move(-1);
        if (button_pressed & hw::Gamepad::Button::DOWN)
            left.move(+1);

        // The right paddle targets the ball y pos with 8 frames of lag.
        ball_pos.push((ball.y >> 8) + 4);
        const uint8_t pos = ball_pos.pop(8);
        right.target(pos);
    }
}
