#include "src/pcd8544/pcd8544.h"

#include "src/game/ball.h"
#include "src/game/collision.h"
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

  y_buffer[y_index] = (ball.y >> 8) + 4;

  uint8_t yl = (y_index + Y_BUFFER_SIZE - 8) % Y_BUFFER_SIZE; // 8 frames of lag
  uint8_t yr = (y_index + Y_BUFFER_SIZE - 4) % Y_BUFFER_SIZE; // 4 frames of lag

  left.target(y_buffer[yl]);
  right.target(y_buffer[yr]);

  y_index = (y_index + 1) % Y_BUFFER_SIZE;

  PCD8544::render();
  delay(30);
}
