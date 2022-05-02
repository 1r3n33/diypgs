#include "src/sdk/buzzer.h"
#include "src/sdk/gamepad.h"
#include "src/sdk/pcd8544.h"

#include "src/game/ball.h"
#include "src/game/circular_buffer.h"
#include "src/game/collision.h"
#include "src/game/paddle.h"

Buzzer sound;
Gamepad controller;

Ball ball;
CircularBuffer ball_pos;
Paddle left;
Paddle right;

void setup()
{
  PCD8544::setup();

  // Bind speaker to pin 8.
  sound.setup(8);

  // Bind buttons up & down to pins 2 & 3.
  controller.setup(2, 3, 0xFF, 0xFF, 0xFF, 0xFF);

  ball.setup();
  ball_pos.setup((ball.y >> 8) + 4);

  left.setup(1);
  right.setup(0);
}

void loop()
{
  const uint32_t start = micros();

  // Update ball (includes wall collision checks).
  const uint8_t collision = ball.update();
  if (collision > 0)
  {
    sound.play(Buzzer::NOTE_C5, 50);
  }

  // Paddle collision checks.
  uint16_t col_data = 0;
  col_data |= compute_collision((ball.x >> 8) + 4,
                                (ball.y >> 8) + 4,
                                4,
                                left.x + 4,
                                left.y + 8,
                                4,
                                8);

  col_data |= compute_collision((ball.x >> 8) + 4,
                                (ball.y >> 8) + 4,
                                4,
                                right.x + 4,
                                right.y + 8,
                                4,
                                8);
  ball.post_collision_update(col_data);
  if (col_data > 0)
  {
    sound.play(Buzzer::NOTE_C6, 50);
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
