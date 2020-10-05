#include <shaiya/game/world/model/Entity.hpp>
#include <shaiya/game/world/model/map/MapCell.hpp>

using namespace shaiya::game;

/**
 * Adds an entity to this cell.
 * @param entity    The entity to add.
 */
void MapCell::addEntity(const std::shared_ptr<Entity>& entity)
{
    entities_.push_back(entity);
}