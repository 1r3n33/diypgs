#pragma once

#include "../hw/Buzzer.h"
#include "../hw/Gamepad.h"
#include "../hw/Pcd8544.h"

#include "ball.h"
#include "Bonus.h"
#include "circular_buffer.h"
#include "paddle.h"

namespace game
{
    class World
    {
    public:
        // State is used to communicate back with the main loop.
        // Each update() calls will return the next state of the current world.
        enum class State : uint8_t
        {
            CONTINUE, // Keep calling update() on the current world.
            NEXT,     // Current world is over, set the next world.
        };

    public:
        World(const hw::Pcd8544 &graphics, const hw::Buzzer &audio, const hw::Gamepad &controller);

    protected:
        virtual ~World();

    public:
        virtual void setup() = 0;
        virtual State update() = 0;

    public:
        void setNextWorld(World *world);
        World *getNextWorld() const;

    protected:
        const hw::Pcd8544 &graphics;
        const hw::Buzzer &audio;
        const hw::Gamepad &controller;
        World *nextWorld;
    };
}
