#include "src/hw/Buzzer.h"
#include "src/hw/Gamepad.h"
#include "src/hw/Pcd8544.h"

#include "src/sdk/Collision.h"

#include "src/game/Game.h"
#include "src/game/Titlescreen.h"

namespace
{
  constexpr uint32_t USEC_PER_FRAME = 1000000 / 30; // 30 Hz

  const hw::Pcd8544 graphics(5, 6, 12);
  const hw::Buzzer audio(4);
  const hw::Gamepad controller(10, 7, 8, 9, 3, 2);

  game::Titlescreen titlescreen(graphics, audio, controller);
  game::Game game_(graphics, audio, controller);

  game::World *currentWorld = nullptr;
}

void setup()
{
  srand(micros());

  graphics.setup();
  audio.setup();
  controller.setup();

  titlescreen.setNextWorld(&game_);
  game_.setNextWorld(&titlescreen);

  currentWorld = &titlescreen;
  currentWorld->setup();
}

void loop()
{
  const uint32_t start = micros();

  game::World::State state = currentWorld->update();
  graphics.render();

  game::World *nextWorld = nullptr;
  switch (state)
  {
  case game::World::State::NEXT:
    nextWorld = currentWorld->getNextWorld();
    nextWorld->setup();
    currentWorld = nextWorld;
    break;

  default:
    break;
  }

  // Target 30 fps.
  // Cannot use delayMicroseconds because the largest value that will produce an accurate delay is 16383.
  // https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
  const uint32_t elapsed = micros() - start;
  if (elapsed < USEC_PER_FRAME)
  {
    delay((USEC_PER_FRAME - elapsed) / 1000);
  }
}
