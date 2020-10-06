#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/world/model/Entity.hpp>

#include <utility>

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
 * Marks this entity as active.
 */
void Entity::activate()
{
    active_ = true;
}

/**
 * Marks this entity as inactive.
 */
void Entity::deactivate()
{
    active_ = false;
}

/**
 * Flags this entity for an update of a specific type.
 * @param mask  The update type.
 */
void Entity::flagUpdate(UpdateMask mask)
{
    updateMask_ |= static_cast<uint32_t>(mask);
}

/**
 * Resets the update flags.
 */
void Entity::resetUpdateFlags()
{
    updateMask_ = 0;
}

/**
 * Sets the direction this entity is facing.
 * @param direction The direction to face.
 */
void Entity::setDirection(uint16_t direction)
{
    if (direction_ == direction)
        return;
    direction_ = direction;
    flagUpdate(UpdateMask::Movement);
}

/**
 * Sets the motion value of this entity.
 * @param motion    The motion value.
 */
void Entity::setMotion(uint8_t motion)
{
    if (motion_ == motion)
        return;
    motion_ = motion;
    flagUpdate(UpdateMask::Movement);
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

    // Flag this entity for a movement update
    flagUpdate(UpdateMask::Movement);
}

/**
 * Sets an attribute.
 * @param attribute The attribute key.
 * @param value     The value to set.
 */
void Entity::setAttribute(Attribute attribute, std::any value)
{
    attributes_[attribute] = std::move(value);
}

/**
 * Clears an attribute.
 * @param attribute The attribute key.
 */
void Entity::clearAttribute(Attribute attribute)
{
    attributes_.erase(attribute);
}