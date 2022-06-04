#include "Game.h"

namespace game
{
    Game::Game(const hw::Pcd8544 &pcd8544,
               const hw::Buzzer &buzzer,
               const hw::Gamepad &gamepad) : World(pcd8544,
                                                   buzzer,
                                                   gamepad)
    {
    }

    void Game::setup()
    {
        for (uint8_t i = 0; i < MAX_BALL_COUNT; i++)
        {
            balls[i].setup(i);
        }
        balls[0].activate(
            sdk::Vec2f16(
                sdk::fixed16_t((hw::Pcd8544::SCREEN_WIDTH / 2) - 4),
                sdk::fixed16_t((hw::Pcd8544::SCREEN_HEIGHT / 2) - 4)));

        for (uint8_t i = 0; i < MAX_BALL_COUNT; i++)
        {
            ballPositions[i].setup(balls[i].pos.y.toInt8() + 4);
        }

        bonus.setup();

        left.setup(Paddle::Side::LEFT);
        right.setup(Paddle::Side::RIGHT);
    }

    World::State Game::update()
    {
        const uint8_t activeBallCount = updateBalls();
        if (activeBallCount == 0)
        {
            audio.play(hw::Buzzer::Note::C3, 200);

            delay(500);
            setup();
            graphics.render();
            delay(500);

            return World::State::CONTINUE;
        }

        updateBonus();

        updateLeftPaddle();

        updateRightPaddle();

        return World::State::CONTINUE;
    }

    uint8_t Game::updateBalls()
    {
        uint8_t activeBallCount = 0;

        for (uint8_t i = 0; i < MAX_BALL_COUNT; i++)
        {
            if (balls[i].flags & Ball::Flag::ENABLED)
            {
                const uint8_t collision = balls[i].preCollisionUpdate();
                if (collision & Ball::Result::Goal)
                {
                    balls[i].deactivate();
                }
                else
                {
                    activeBallCount++;
                    if (collision & Ball::Result::Bounce)
                    {
                        audio.play(hw::Buzzer::Note::C5, 50);
                    }

                    // Paddle collision checks.
                    const uint8_t leftCenterX = left.getCenterX();
                    const uint8_t leftCenterY = left.getCenterY();
                    const uint8_t leftHalfWidth = left.getWidth() / 2;
                    const uint8_t leftHalfHeight = left.getHeight() / 2;
                    const sdk::CollisionResult left_paddle_col = sdk::ComputeCollision(balls[i].pos.x.toInt8() + 4,
                                                                                       balls[i].pos.y.toInt8() + 4,
                                                                                       4,
                                                                                       leftCenterX,
                                                                                       leftCenterY,
                                                                                       leftHalfWidth,
                                                                                       leftHalfHeight);
                    if (left_paddle_col.axis)
                    {
                        balls[i].postCollisionUpdate(left_paddle_col);
                        audio.play(hw::Buzzer::Note::C6, 50);
                    }

                    const uint8_t rightCenterX = right.getCenterX();
                    const uint8_t rightCenterY = right.getCenterY();
                    const uint8_t rightHalfWidth = right.getWidth() / 2;
                    const uint8_t rightHalfHeight = right.getHeight() / 2;
                    const sdk::CollisionResult right_paddle_col = sdk::ComputeCollision(balls[i].pos.x.toInt8() + 4,
                                                                                        balls[i].pos.y.toInt8() + 4,
                                                                                        4,
                                                                                        rightCenterX,
                                                                                        rightCenterY,
                                                                                        rightHalfWidth,
                                                                                        rightHalfHeight);
                    if (right_paddle_col.axis)
                    {
                        balls[i].postCollisionUpdate(right_paddle_col);
                        audio.play(hw::Buzzer::Note::C6, 50);
                    }
                }
            }

            ballPositions[i].push(balls[i].pos.y.toInt8() + 4);
        }

        return activeBallCount;
    }

    void Game::updateBonus()
    {
        bonus.update();
        for (uint8_t i = 0; i < MAX_BALL_COUNT; i++)
        {
            const sdk::CollisionResult bonus_col = sdk::ComputeCollision(balls[i].pos.x.toInt8() + 4,
                                                                         balls[i].pos.y.toInt8() + 4,
                                                                         4,
                                                                         bonus.center.x.toInt8(),
                                                                         bonus.center.y.toInt8() + 4,
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
                    balls[i].accelerate();
                    break;

                case game::Bonus::Effect::MULTI_BALL:
                    for (uint8_t j = 0; j < MAX_BALL_COUNT; j++)
                    {
                        if ((balls[j].flags & Ball::Flag::ENABLED) == 0)
                        {
                            balls[j].activate(balls[i].pos);
                            break;
                        }
                    }
                    break;

                case game::Bonus::Effect::CHANGE_SIZE_LEFT:
                    left.changeSize();
                    break;

                case game::Bonus::Effect::CHANGE_SIZE_RIGHT:
                    right.changeSize();
                    break;

                default:
                    break;
                }

                break;
            }
        }
    }

    void Game::updateLeftPaddle()
    {
        // Player controls the left paddle.
        const uint8_t button_pressed = controller.get();

        if (button_pressed & hw::Gamepad::Button::UP)
        {
            left.move(-1);
        }

        if (button_pressed & hw::Gamepad::Button::DOWN)
        {
            left.move(+1);
        }
    }

    void Game::updateRightPaddle()
    {
        // The right paddle targets the closest ball y pos with 8 frames of lag.
        int8_t closestBallIndex = -1;
        sdk::fixed16_t greatestX = sdk::fixed16_t(-128);

        for (uint8_t i = 0; i < MAX_BALL_COUNT; i++)
        {
            if (balls[i].flags & Ball::Flag::ENABLED)
            {
                if (balls[i].pos.x > greatestX)
                {
                    greatestX = balls[i].pos.x;
                    closestBallIndex = i;
                }
            }
        }

        if (closestBallIndex >= 0)
        {
            const uint8_t pos = ballPositions[closestBallIndex].pop(8);
            right.target(pos);
        }
    }
}
