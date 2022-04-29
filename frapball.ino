#include "src/controller/controller.h"
#include "src/pcd8544/pcd8544.h"

#include "src/game/ball.h"
#include "src/game/circular_buffer.h"
#include "src/game/collision.h"
#include "src/game/paddle.h"

Controller gamepad;
Ball ball;
CircularBuffer ball_pos;
Paddle left;
Paddle right;

void setup()
{
  PCD8544::setup();

  // Bind buttons up & down to pins 2 & 3.
  gamepad.setup(2, 3, 0xFF, 0xFF, 0xFF, 0xFF);

  ball.setup();
  ball_pos.setup((ball.y >> 8) + 4);

  left.setup(1);
  right.setup(0);
}

void loop()
{
  ball.update();

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

  // Player controls the left paddle
  const uint8_t button_pressed = gamepad.get();
  if (button_pressed & Controller::BUTTON_UP)
    left.move(-1);
  if (button_pressed & Controller::BUTTON_DOWN)
    left.move(+1);

  // The right paddle targets the ball y pos with 8 frames of lag.
  ball_pos.push((ball.y >> 8) + 4);
  const uint8_t pos = ball_pos.pop(8);
  right.target(pos);

  PCD8544::render();
  delay(30);
}
