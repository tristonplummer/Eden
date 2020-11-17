#include <shaiya/game/model/Entity.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
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
    if (entity->type() == EntityType::Player)
    {
        players_.push_back(std::dynamic_pointer_cast<Player>(entity));
        setAlive(true);
    }
    else
    {
        entities_.push_back(entity);
    }
}

/**
 * Removes an entity from this cell.
 * @param entity    The entity to remove.
 */
void MapCell::removeEntity(const std::shared_ptr<Entity>& entity)
{
    if (entity->type() == EntityType::Player)
    {
        auto player = std::dynamic_pointer_cast<Player>(entity);
        auto pos    = std::find(players_.begin(), players_.end(), player);
        if (pos != players_.end())
            players_.erase(pos);

        // Loop over the neighbours of this cell
        bool alive = false;
        for (auto& cell: neighbours())
        {
            auto& players = cell->players();
            if (!players.empty())
            {
                alive = true;
                break;
            }
        }

        // Mark this cell as either alive or dead
        setAlive(alive);
    }
    else
    {
        auto pos = std::find(entities_.begin(), entities_.end(), entity);
        if (pos != entities_.end())
            entities_.erase(pos);
    }
}

/**
 * Sets the alive state of this cell.
 * @param alive The alive state.
 */
void MapCell::setAlive(bool alive)
{
    alive_ = alive;
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
    auto minRow    = std::max(0, (int)row_ - OBSERVABLE_CELL_RADIUS);
    auto minColumn = std::max(0, (int)column_ - OBSERVABLE_CELL_RADIUS);

    // The max row and column
    auto maxRow    = row_ + OBSERVABLE_CELL_RADIUS;
    auto maxColumn = column_ + OBSERVABLE_CELL_RADIUS;

    // Loop through the cell rows and columns
    for (auto row = minRow; row < maxRow; row++)
    {
        for (auto column = minColumn; column < maxColumn; column++)
        {
            auto cell = map_.getCell(row, column);
            if (cell)
            {
                neighbours_.push_back(cell);
            }
        }
    }

    // Find the neighbouring cells based on a radius
    return neighbours_;
}
