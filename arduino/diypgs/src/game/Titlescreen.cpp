#include "Titlescreen.h"

namespace game
{
    Titlescreen::Titlescreen(const hw::Pcd8544 &pcd8544,
                             const hw::Buzzer &buzzer,
                             const hw::Gamepad &gamepad) : World(pcd8544,
                                                                 buzzer,
                                                                 gamepad)
    {
    }

    void Titlescreen::setup()
    {
    }

    World::State Titlescreen::update()
    {
        audio.play(hw::Buzzer::Note::C4, 100);
        return World::State::NEXT;
    }
}
