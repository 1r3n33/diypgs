#include "src/pcd8544/pcd8544.h"

#include "src/game/ball.h"
#include "src/game/paddle.h"
#include "src/game/ybuffer.h"

Ball ball;
Paddle left(0x1);
Paddle right(0x0);

void setup()
{
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
  ball.update(0);
  y_buffer[y_index] = (ball.y >> 8) + 4;

  uint8_t yl = (y_index + Y_BUFFER_SIZE - 4) % Y_BUFFER_SIZE;  // 4 frames of lag
  uint8_t yr = (y_index + Y_BUFFER_SIZE - 12) % Y_BUFFER_SIZE; // 12 frames of lag

  left.target(y_buffer[yl]);
  right.target(y_buffer[yr]);

  y_index = (y_index + 1) % Y_BUFFER_SIZE;

  PCD8544::render();
  delay(30);
}
