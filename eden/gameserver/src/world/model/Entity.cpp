#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/world/model/Entity.hpp>

using namespace shaiya::game;

/**
 * Creates this entity instance.
 * @param world The game world.
 */
Entity::Entity(GameWorldService& world): world_(world)
{
}

/**
 * Initialises this entity.
 */
void Entity::init()
{
}

/**
 * Sets the position of this entity.
 * @param position  The position.
 */
void Entity::setPosition(Position position)
{
    // If the positions are equal, do nothing
    if (position_ == position)
        return;

    // If the new position is just a height change, we wont bother finding a new cell.
    if (position_.y() == position.y())
    {
        position_ = position;
        return;
    }

    // Get the world maps
    auto& maps = world_.maps();

    // This entity
    auto entity = shared_from_this();

    // Remove the entity from the current map.
    auto current = maps.forId(position_.map());
    current->remove(entity);

    // Update the position
    position_ = position;

    // Add the entity to the new map
    auto next = maps.forId(position_.map());
    next->add(entity);
}