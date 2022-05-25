#pragma once

#include "World.h"

namespace game
{
    class Titlescreen : public World
    {
    public:
        Titlescreen(const hw::Pcd8544 &graphics, const hw::Buzzer &audio, const hw::Gamepad &controller);

    public:
        void setup() override;
        World::State update() override;
    };
}
