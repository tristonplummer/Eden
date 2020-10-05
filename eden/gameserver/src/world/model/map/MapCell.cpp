#include <shaiya/game/world/model/Entity.hpp>
#include <shaiya/game/world/model/map/MapCell.hpp>

using namespace shaiya::game;

/**
 * Adds an entity to this cell.
 * @param entity    The entity to add.
 */
void MapCell::addEntity(std::shared_ptr<Entity> entity)
{
    entities_.push_back(entity);
}

/**
 * Removes an entity from this cell.
 * @param entity    The entity to remove.
 */
void MapCell::removeEntity(const std::shared_ptr<Entity>& entity)
{
    auto pred = [&](const std::shared_ptr<Entity>& element) {
        return element->type() == entity->type() && element->id() == entity->id();
    };
    auto pos = std::find_if(entities_.begin(), entities_.end(), pred);
    if (pos != entities_.end())
    {
        entities_.erase(pos);
    }
}