#include "World.h"

namespace game
{
    World::World(const hw::Pcd8544 &pcd8544,
                 const hw::Buzzer &buzzer,
                 const hw::Gamepad &gamepad) : graphics(pcd8544),
                                               audio(buzzer),
                                               controller(gamepad),
                                               nextWorld(this)
    {
    }

    World::~World()
    {
    }

    void World::setNextWorld(World *world)
    {
        nextWorld = world;
    }

    World *World::getNextWorld() const
    {
        return nextWorld;
    }
}
