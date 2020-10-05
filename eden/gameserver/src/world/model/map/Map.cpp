#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/model/map/Map.hpp>

#include <glog/logging.h>

#include <array>
#include <cassert>
#include <cfenv>
#include <cmath>

using namespace shaiya::game;

/**
 * The size of a cell (16x16).
 */
constexpr auto CELL_SIZE = 16;

/**
 * The observable radius from a center cell.
 */
constexpr auto OBSERVABLE_CELL_RADIUS = 3;

/**
 * The offset to read a map's size type from, if it is a field.
 */
constexpr auto FIELD_MAP_SIZE_OFFSET = 5;

/**
 * This represents the size type in a field, which can either be 4 (standard size), or 8 (large size).
 */
constexpr auto STANDARD_SIZE_TYPE = 4;

/**
 * The size of a standard map.
 */
constexpr auto STANDARD_MAP_SIZE = 1024;

/**
 * The size of a large map.
 */
constexpr auto LARGE_MAP_SIZE = 2048;

/**
 * Loads this map by populating the cells, and parsing the heightmap and objects.
 * @param stream    The input stream.
 * @param length    The length of the stream.
 */
void Map::load(std::ifstream& stream, size_t length)
{
    std::array<char, 3> header{ 0 };  // The map type (either FLD or DUN).
    stream.read(header.data(), header.size());
    std::string type(header.data(), header.size());

    // Seek back to the beginning of the stream.
    stream.seekg(std::ios::beg);

    // Parse the map depending on the type
    if (type == "FLD")
        parseField(stream);
    else if (type == "DUN")
        parseDungeon(stream);

    // Calculate the cell rows and columns
    // Cells always fit perfectly into a map, and map sizes are only ever 1024x1024 or 2048x2048.
    rowCount_    = size_ / CELL_SIZE;
    columnCount_ = rowCount_;

    // The total number of cells
    auto totalCells = rowCount_ * columnCount_;
    cells_.resize(totalCells);

    // Generate the cells
    for (auto i = 0; i < totalCells; i++)
        cells_.at(i) = std::make_shared<MapCell>();
}

/**
 * Parses this map as a field.
 * @param stream    The input stream.
 */
void Map::parseField(std::ifstream& stream)
{
    // In a field type map, the size is the 5th byte.
    char sizeType;
    stream.seekg(FIELD_MAP_SIZE_OFFSET);
    stream.read(&sizeType, sizeof(sizeType));

    // Set the size  of the map based on the size type
    size_ = sizeType == STANDARD_SIZE_TYPE ? STANDARD_MAP_SIZE : LARGE_MAP_SIZE;
}

/**
 * Parses this map as a dungeon.
 * @param stream    The input stream.
 */
void Map::parseDungeon(std::ifstream& stream)
{
    size_ = STANDARD_MAP_SIZE;
}

/**
 * Adds an entity to this map.
 * @param entity    The entity to add.
 */
void Map::add(const std::shared_ptr<Entity>& entity) const
{
    auto cell = getCell(entity->position());
    assert(cell);
    cell->addEntity(entity);
}

/**
 * Removes an entity from this map.
 * @param entity    The entity to remove.
 */
void Map::remove(const std::shared_ptr<Entity>& entity) const
{
    auto cell = getCell(entity->position());
    assert(cell);
    cell->removeEntity(entity);
}

/**
 * Get a cell in the map based on a position.
 * @param position  The position.
 * @return          The map cell.
 */
std::shared_ptr<MapCell> Map::getCell(const Position& position) const
{
    return cells_.at(getCellIndex(position));
}

/**
 * Get a cell index  based on a position.
 * @param position  The position.
 * @return          The map cell.
 */
size_t Map::getCellIndex(const Position& position) const
{
    // Set the rounding mode
    std::fesetround(FE_TOWARDZERO);

    // The x and z coordinates
    auto x = std::min(static_cast<size_t>(std::nearbyint(position.x())), size_);
    auto z = std::min(static_cast<size_t>(std::nearbyint(position.z())), size_);

    // The row and column
    auto row    = x / CELL_SIZE;
    auto column = z / CELL_SIZE;
    return row + (column * rowCount_);
}

/**
 * Gets the cells in a neighbouring radius of a position.
 * @param position  The position.
 * @return          The neighbouring cells.
 */
std::vector<std::shared_ptr<MapCell>> Map::getNeighbouringCells(const Position& position) const
{
    // The vector of cells
    std::vector<std::shared_ptr<MapCell>> cells;

    // The x and z coordinates.
    auto x = std::min(static_cast<size_t>(std::nearbyint(position.x())), size_);
    auto z = std::min(static_cast<size_t>(std::nearbyint(position.z())), size_);

    // The position's row and column
    auto centerRow    = x / CELL_SIZE;
    auto centerColumn = z / CELL_SIZE;

    // The maximum row and column
    auto maxRow    = centerRow + OBSERVABLE_CELL_RADIUS;
    auto maxColumn = centerColumn + OBSERVABLE_CELL_RADIUS;

    // Loop through the cell rows and columns
    for (auto row = centerRow - OBSERVABLE_CELL_RADIUS; row < maxRow; row++)
    {
        for (auto column = centerColumn - OBSERVABLE_CELL_RADIUS; column < maxColumn; column++)
        {
            auto id = row + (column * rowCount_);
            if (id >= 0 && id < cells_.size())
            {
                cells.push_back(cells_.at(id));
            }
        }
    }
    return cells;
}