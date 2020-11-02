#include <shaiya/game/model/Entity.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/model/map/MapCell.hpp>

using namespace shaiya::game;

/**
 * The observable radius from a center cell.
 */
constexpr auto OBSERVABLE_CELL_RADIUS = 3;

/**
 * Initialises this map cell.
 * @param map       The map instance.
 * @param row       The row
 * @param column    The column
 */
MapCell::MapCell(Map& map, size_t row, size_t column): map_(map), row_(row), column_(column)
{
}

/**
 * Adds an entity to this cell.
 * @param entity    The entity to add.
 */
void MapCell::addEntity(const std::shared_ptr<Entity>& entity)
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

/**
 * Gets the neighbouring cells.
 * @return  The neighbouring cells.
 */
const std::vector<std::shared_ptr<MapCell>>& MapCell::neighbours()
{
    if (!neighbours_.empty())  // If neighbours is already initialised, just return the reference
        return neighbours_;

    // The minimum row and column
    auto minRow    = row_ - OBSERVABLE_CELL_RADIUS;
    auto minColumn = column_ - OBSERVABLE_CELL_RADIUS;

    // The max row and column
    auto maxRow    = row_ + OBSERVABLE_CELL_RADIUS;
    auto maxColumn = column_ + OBSERVABLE_CELL_RADIUS;

    // Loop through the cell rows and columns
    for (auto row = minRow; row < maxRow; row++)
    {
        for (auto column = minColumn; column < maxColumn; column++)
        {
            neighbours_.push_back(map_.getCell(row, column));
        }
    }

    // Find the neighbouring cells based on a radius
    return neighbours_;
}
