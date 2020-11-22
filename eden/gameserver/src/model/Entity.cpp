#include <shaiya/game/model/Entity.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

#include <cmath>
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
 * Processes the tick for this entity.
 */
void Entity::tick()
{
}

/**
 * Sets the id for this entity.
 * @param id    The new id.
 */
void Entity::setId(size_t id)
{
    id_ = id;
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
void Entity::flagUpdate(UpdateFlag mask)
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
 * @param radians   The direction to face, in radians.
 */
void Entity::setDirection(float radians)
{
    uint16_t direction = -radians * (180.0 / M_PI);
    setDirection(direction);
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
    flagUpdate(UpdateFlag::Movement);
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
    flagUpdate(UpdateFlag::Movement);
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

    // Get the world maps
    auto& maps = world_.maps();

    // This entity
    auto entity = shared_from_this();

    // Remove the entity from the current map.
    auto current = maps.forId(position_.map());
    if (current != nullptr)
        current->remove(entity);

    // Update the position
    position_ = position;

    // Add the entity to the new map
    auto next = maps.forId(position_.map());
    if (next != nullptr)
        next->add(entity);
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

/**
 * Checks if this entity can be observed by another.
 * @param other The entity trying to observe this entity.
 * @return      If the other entity can observe us.
 */
bool Entity::observable(Entity& other)
{
    return position_.isWithinViewportDistance(other.position());
}

/**
 * Gets the current map of this entity.
 * @return  The current map.
 */
std::shared_ptr<Map> Entity::map() const
{
    return world_.maps().forId(position_.map());
}