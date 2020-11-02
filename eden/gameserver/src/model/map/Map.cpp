#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/npc/Npc.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/model/map/MapCell.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

#include <cassert>
#include <cfenv>
#include <cmath>
#include <iostream>
#include <yaml-cpp/yaml.h>

using namespace shaiya::game;

/**
 * The size of a cell (16x16).
 */
constexpr auto CELL_SIZE = 16;

/**
 * Initialises this map.
 * @param world The world instance.
 */
Map::Map(GameWorldService& world): world_(world)
{
}

/**
 * Loads this map by populating the cells, and parsing the heightmap and objects.
 * @param stream    The input stream.
 */
void Map::load(std::ifstream& stream)
{
    auto yaml = YAML::Load(stream);

    // Read the id, and the size
    id_   = yaml["id"].as<uint16_t>();
    size_ = yaml["size"].as<size_t>();

    // Calculate the cell rows and columns
    // Cells always fit perfectly into a map, and map sizes are only ever 1024x1024 or 2048x2048.
    rowCount_    = size_ / CELL_SIZE;
    columnCount_ = rowCount_;

    // The total number of cells
    auto totalCells = rowCount_ * columnCount_;
    cells_.resize(totalCells);

    // Generate the cells
    for (auto row = 0; row < rowCount_; row++)
    {
        for (auto column = 0; column < columnCount_; column++)
        {
            auto idx       = row + (column * rowCount_);
            cells_.at(idx) = std::make_shared<MapCell>(*this, row, column);
        }
    }
}

/**
 * Loads an initial npc spawn for this map.
 * @param stream    The input stream.
 */
void Map::loadNpc(std::ifstream& stream)
{
    auto yaml   = YAML::Load(stream);
    auto spawns = yaml["npcs"];

    for (auto&& spawn: spawns)
    {
        auto data      = spawn["npc"];
        uint8_t type   = data["type"].as<int>();
        uint8_t typeId = data["typeid"].as<int>();

        // Loop over the positions
        for (auto&& position: data["positions"])
        {
            auto x   = position["x"].as<float>();
            auto y   = position["y"].as<float>();
            auto z   = position["z"].as<float>();
            auto dir = position["dir"].as<float>(0.00);

            auto pos = Position(id_, x, y, z);

            auto* def   = new NpcDefinition();
            def->type   = type;
            def->typeId = typeId;

            auto npc = std::make_shared<Npc>(*def, world_);
            npc->setPosition(pos);
            npc->setDirection(dir);

            world_.registerNpc(std::move(npc));
        }
    }
}

/**
 * Loads an initial mob spawn for this map.
 * @param stream    The input stream
 */
void Map::loadMob(std::ifstream& stream)
{
    auto yaml    = YAML::Load(stream);
    auto entries = yaml["mobs"];

    for (auto&& entry: entries)
    {
        auto spawn = entry["spawn"];

        auto areas = spawn["area"];
        auto area  = areas.begin();

        auto first  = *area++;
        auto second = *area;

        Position bottomLeft(id_, first["x"].as<float>(), first["y"].as<float>(), first["z"].as<float>());
        Position topRight(id_, second["x"].as<float>(), second["y"].as<float>(), second["z"].as<float>());
        Area spawnArea(bottomLeft, topRight);

        auto spawns = spawn["spawns"];
        for (auto&& mobSpawn: spawns)
        {
            auto id       = mobSpawn["id"].as<int>();
            auto quantity = mobSpawn["quantity"].as<int>();

            auto* def = new client::MobDefinition();
            def->id   = id;

            for (auto i = 0; i < quantity; i++)
            {
                auto mob = std::make_shared<Mob>(*def, spawnArea, world_);
                mob->setPosition(mob->spawnArea().randomPoint());

                world_.registerMob(mob);
            }
        }
    }
}

/**
 * Loads the world file for the map.
 * @param path  The path to the world
 */
void Map::loadWorld(const std::string& path)
{
    worldFile_ = client::World(path);
}

/**
 * Processes the tick for this map.
 */
void Map::tick() const
{
    // Tick all active entities in this map.
    for (auto& entity: getLiveEntities())
    {
        if (entity->type() == EntityType::Mob)
            (std::dynamic_pointer_cast<Mob>(entity))->tick();
        else
            entity->tick();
    }
}

/**
 * Adds an entity to this map.
 * @param entity    The entity to add.
 */
void Map::add(const std::shared_ptr<Entity>& entity)
{
    // Adjust the position where needed
    adjustPosition(entity->position());

    // Get the cell to place this entity into.
    auto cell = getCell(entity->position());
    assert(cell);
    cell->addEntity(entity);

    // If the added entity was a player
    if (entity->type() == EntityType::Player)
    {
        auto& neighbours = cell->neighbours();
        for (auto&& neighbour: neighbours)
            aliveCells_.insert({ neighbour->row(), neighbour->column() });
    }
}

/**
 * Removes an entity from this map.
 * @param entity    The entity to remove.
 */
void Map::remove(const std::shared_ptr<Entity>& entity)
{
    // Adjust the position where needed
    adjustPosition(entity->position());

    // Get the cell to remove this entity from
    auto cell = getCell(entity->position());
    assert(cell);
    cell->removeEntity(entity);

    // If the cell is dead, remove it from the set
    if (!cell->alive())
    {
        auto& neighbours = cell->neighbours();
        for (auto&& neighbour: neighbours)
        {
            if (!neighbour->alive())
                aliveCells_.erase({ neighbour->row(), neighbour->column() });
        }
    }
}

/**
 * Attempts to get an entity with a specified id and type.
 * @param pos   The position to start the search at.
 * @param id    The entity id.
 * @param type  The entity type to search for.
 * @return      The entity instance.
 */
std::shared_ptr<Entity> Map::get(Position& pos, size_t id, EntityType type) const
{
    auto neighbours = getNeighbouringCells(pos);
    for (auto&& cell: neighbours)
    {
        for (auto&& entity: cell->entities())
        {
            if (entity->type() != type)
                continue;
            if (entity->id() == id)
                return std::shared_ptr<Entity>(entity);
        }
    }
    return nullptr;
}

/**
 * Get a cell in the map based on a position.
 * @param position  The position.
 * @return          The map cell.
 */
std::shared_ptr<MapCell> Map::getCell(Position& position) const
{
    // Adjust the position where needed
    adjustPosition(position);
    return cells_.at(getCellIndex(position));
}

/**
 * Get a cell in the map based on a row and column.
 * @param row       The cell row.
 * @param column    The cell column.
 * @return          The map cell.
 */
std::shared_ptr<MapCell> Map::getCell(size_t row, size_t column) const
{
    auto idx = row + (column * rowCount_);
    return cells_.at(idx);
}

/**
 * Get a cell index  based on a position.
 * @param position  The position.
 * @return          The map cell.
 */
size_t Map::getCellIndex(Position& position) const
{
    // Adjust the position where needed
    adjustPosition(position);

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
const std::vector<std::shared_ptr<MapCell>>& Map::getNeighbouringCells(Position& position) const
{
    // Adjust the position where needed
    adjustPosition(position);

    // The vector of cells
    std::vector<std::shared_ptr<MapCell>> cells;

    // The x and z coordinates.
    auto x = std::min(static_cast<size_t>(std::nearbyint(position.x())), size_);
    auto z = std::min(static_cast<size_t>(std::nearbyint(position.z())), size_);

    // The position's row and column
    auto row    = x / CELL_SIZE;
    auto column = z / CELL_SIZE;

    // The cell at the specific position
    auto cell = getCell(row, column);
    return cell->neighbours();
}

/**
 * Gets the entities that are located in neighbouring cells.
 * @param position  The position.
 * @param type      The entity type to search for.
 * @return          The neighbouring entities.
 */
std::vector<std::shared_ptr<Entity>> Map::getNeighbouringEntities(Position& position, EntityType type) const
{
    std::vector<std::shared_ptr<Entity>> entities;
    const auto& cells = getNeighbouringCells(position);
    for (auto&& cell: cells)
    {
        const auto& neighbours = cell->entities();
        for (auto&& entity: neighbours)
        {
            if (entity->active() && entity->type() == type)
            {
                entities.push_back(entity);
            }
        }
    }
    return entities;
}

/**
 * Gets all live entities on this map.
 * @return  The live entities.
 */
std::vector<std::shared_ptr<Entity>> Map::getLiveEntities() const
{
    std::vector<std::shared_ptr<Entity>> entities;
    for (auto [row, column]: aliveCells_)
    {
        auto cell = getCell(row, column);
        if (!cell)
            continue;

        for (auto& entity: cell->entities())
            entities.push_back(entity);
    }

    return entities;
}

/**
 * Adjusts a position to fit into the boundaries of this map.
 * @param position  The position to adjust.
 */
void Map::adjustPosition(Position& position) const
{
    // If the coordinates are less than 0, adjust them to 0.
    if (position.x() < 0)
        position.setX(0);
    if (position.y() < 0)
        position.setY(0);
    if (position.z() < 0)
        position.setZ(0);

    // If the coordinates are greater than or equal to the map size, adjust them to the map size - 1
    if (position.x() >= size_)
        position.setX(size_ - 1);
    if (position.z() >= size_)
        position.setZ(size_ - 1);
}