#include "src/sdk/buzzer.h"
#include "src/sdk/gamepad.h"
#include "src/sdk/pcd8544.h"

#include "src/game/ball.h"
#include "src/game/Bonus.h"
#include "src/game/circular_buffer.h"
#include "src/game/collision.h"
#include "src/game/paddle.h"

using namespace game;

Buzzer sound;
Gamepad controller;

Ball ball;
Bonus bonus;
CircularBuffer ball_pos;
Paddle left;
Paddle right;

void setup()
{
  srand(micros());

  PCD8544::setup();

  // Bind speaker to pin 8.
  sound.setup(8);

  // Bind buttons up & down to pins 2 & 3.
  controller.setup(2, 3, 0xFF, 0xFF, 0xFF, 0xFF);

  ball.setup();

  bonus.setup();

  ball_pos.setup((ball.y >> 8) + 4);

  left.setup(1);
  right.setup(0);
}

void loop()
{
  const uint32_t start = micros();

  // Update ball (includes wall collision checks).
  const uint8_t collision = ball.update();
  if (collision & Ball::Result::Collision)
  {
    sound.play(Buzzer::NOTE_C5, 50);
  }
  if (collision & Ball::Result::Score)
  {
    sound.play(Buzzer::NOTE_C3, 200);
    delay(500);
    ball.setup();
    bonus.setup();
    ball_pos.setup((ball.y >> 8) + 4);
    PCD8544::render();
    delay(500);
    return;
  }

  // Paddle collision checks.
  const CollisionResult left_paddle_col = compute_collision((ball.x >> 8) + 4,
                                                            (ball.y >> 8) + 4,
                                                            4,
                                                            left.x + 4,
                                                            left.y + 8,
                                                            4,
                                                            8);
  if (left_paddle_col.axis)
  {
    ball.post_collision_update(left_paddle_col);
    sound.play(Buzzer::NOTE_C6, 50);
  }

  const CollisionResult right_paddle_col = compute_collision((ball.x >> 8) + 4,
                                                             (ball.y >> 8) + 4,
                                                             4,
                                                             right.x + 4,
                                                             right.y + 8,
                                                             4,
                                                             8);
  if (right_paddle_col.axis)
  {
    ball.post_collision_update(right_paddle_col);
    sound.play(Buzzer::NOTE_C6, 50);
  }

  // Bonus update
  // Must be before player controls update because bonus can affect player controls.
  bonus.update();
  const CollisionResult bonus_col = compute_collision((ball.x >> 8) + 4,
                                                      (ball.y >> 8) + 4,
                                                      4,
                                                      (bonus.center_x >> 8),
                                                      (bonus.center_y >> 8) + 4,
                                                      7,
                                                      4);
  if (bonus_col.axis && bonus.state == Bonus::State::ENABLED)
  {
    sound.play(Buzzer::NOTE_C7, 50);

    const Bonus::Effect effect = bonus.capture();
    switch (effect)
    {
    case Bonus::Effect::INVERT_COMMAND_LEFT:
      left.invert();
      break;
    case Bonus::Effect::INVERT_COMMAND_RIGHT:
      right.invert();
      break;
    case Bonus::Effect::ACCELERATE_BALL:
      ball.accelerate();
      break;
    default:
      break;
    }
  }

  // Player controls the left paddle.
  const uint8_t button_pressed = controller.get();
  if (button_pressed & Gamepad::BUTTON_UP)
    left.move(-1);
  if (button_pressed & Gamepad::BUTTON_DOWN)
    left.move(+1);

  // The right paddle targets the ball y pos with 8 frames of lag.
  ball_pos.push((ball.y >> 8) + 4);
  const uint8_t pos = ball_pos.pop(8);
  right.target(pos);

  PCD8544::render();

  // Target 30 fps.
  // Cannot use delayMicroseconds because the largest value that will produce an accurate delay is 16383.
  // https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
  const uint32_t elapsed = micros() - start;
  delay((33333 - elapsed) / 1000);
}