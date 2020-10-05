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