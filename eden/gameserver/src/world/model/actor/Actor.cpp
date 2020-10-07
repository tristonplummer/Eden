#include <shaiya/game/world/model/actor/Actor.hpp>

using namespace shaiya::game;

/**
 * Initialises this actor.
 * @param world The world instance.
 */
Actor::Actor(GameWorldService& world): Entity(world)
{
}

/**
 * Initialises this actor.
 */
void Actor::init()
{
    // Initialise the base entity
    Entity::init();
}

/**
 * Sets the name of this actor.
 * @param name  The name value.
 */
void Actor::setName(const std::string& name)
{
    if (name_ == name)
        return;
    name_ = name;
    flagUpdate(UpdateMask::Appearance);
}

/**
 * Sets the class of the actor.
 * @param job   The class value.
 */
void Actor::setJob(shaiya::ShaiyaClass job)
{
    if (class_ == job)
        return;
    class_ = job;
    flagUpdate(UpdateMask::Appearance);
}