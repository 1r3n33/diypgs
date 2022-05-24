#include "src/hw/Buzzer.h"
#include "src/hw/Gamepad.h"
#include "src/hw/Pcd8544.h"

#include "src/sdk/Collision.h"

#include "src/game/World.h"

const hw::Pcd8544 graphics(10, 12);
const hw::Buzzer audio(8);
const hw::Gamepad controller(2, 3, 0xFF, 0xFF, 0xFF, 0xFF);

game::World world(graphics, audio, controller);

void setup()
{
  srand(micros());

  graphics.setup();
  audio.setup();
  controller.setup();

  world.setup();
}

void loop()
{
  const uint32_t start = micros();

  world.update();

  graphics.render();

  // Target 30 fps.
  // Cannot use delayMicroseconds because the largest value that will produce an accurate delay is 16383.
  // https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
  const uint32_t elapsed = micros() - start;
  delay((33333 - elapsed) / 1000);
}
