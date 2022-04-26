#include "src/pcd8544/pcd8544.h"

#include "src/game/ball.h"
#include "src/game/collision.h"
#include "src/game/paddle.h"
#include "src/game/ybuffer.h"

const uint8_t PIN_BUTTON_UP = 2;
const uint8_t PIN_BUTTON_DOWN = 3;

Ball ball;
Paddle left(0x1);
Paddle right(0x0);

void setup()
{
  pinMode(PIN_BUTTON_UP, INPUT);
  pinMode(PIN_BUTTON_DOWN, INPUT);

  PCD8544::setup();
  ball.reset();
  left.move(0);
  right.move(0);

  for (uint8_t i = 0; i < Y_BUFFER_SIZE; i++)
  {
    y_buffer[i] = (ball.y >> 8) + 4;
  }
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
  uint8_t button_up_pressed = digitalRead(PIN_BUTTON_UP) == HIGH;
  uint8_t button_down_pressed = digitalRead(PIN_BUTTON_DOWN) == HIGH;
  if (button_up_pressed)
    left.move(-1);
  if (button_down_pressed)
    left.move(+1);

  // The right paddle targets the ball y pos with a few frames of lag.
  y_buffer[y_index] = (ball.y >> 8) + 4;
  uint8_t yl = (y_index + Y_BUFFER_SIZE - 8) % Y_BUFFER_SIZE;  // 8 frames of lag
  uint8_t yr = (y_index + Y_BUFFER_SIZE - 14) % Y_BUFFER_SIZE; // 14 frames of lag
  y_index = (y_index + 1) % Y_BUFFER_SIZE;

  right.target(y_buffer[yr]);

  PCD8544::render();
  delay(30);
}
