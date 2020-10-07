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
 * Synchronises this actor's stats, by calculating the bonuses from their
 * active buffs and worn equipment.
 */
void Actor::syncStats()
{
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
    flagUpdate(UpdateFlag::Appearance);
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
    flagUpdate(UpdateFlag::Appearance);
}