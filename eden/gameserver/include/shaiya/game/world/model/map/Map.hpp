#pragma once
#include <shaiya/game/world/model/Position.hpp>
#include <shaiya/game/world/model/map/MapCell.hpp>

#include <fstream>
#include <memory>
#include <vector>

namespace shaiya::game
{
    class Entity;  // Forward declaration of the entity class.

    /**
     * Represents a map in the game world.
     */
    class Map
    {
    public:
        /**
         * Loads this map by populating the cells, and parsing the heightmap and objects.
         * @param stream    The input stream.
         * @param length    The length of the stream.
         */
        void load(std::ifstream& stream, size_t length);

        /**
         * Adds an entity to this map.
         * @param entity    The entity to add.
         */
        void add(const std::shared_ptr<Entity>& entity) const;

        /**
         * Gets the cells in a neighbouring radius of a position.
         * @param position  The position.
         * @return          The neighbouring cells.
         */
        [[nodiscard]] std::vector<std::shared_ptr<MapCell>> getNeighbouringCells(const Position& position) const;

        /**
         * Get a cell in the map based on a position.
         * @param position  The position.
         * @return          The map cell.
         */
        [[nodiscard]] std::shared_ptr<MapCell> getCell(const Position& position) const;

        /**
         * Gets the size of the map.
         * @return  The map size.
         */
        [[nodiscard]] size_t size() const
        {
            return size_;
        }

    private:
        /**
         * Parses this map as a field.
         * @param stream    The input stream.
         */
        void parseField(std::ifstream& stream);

        /**
         * Parses this map as a dungeon.
         * @param stream    The input stream.
         */
        void parseDungeon(std::ifstream& stream);

        /**
         * Get a cell index  based on a position.
         * @param position  The position.
         * @return          The map cell.
         */
        [[nodiscard]] size_t getCellIndex(const Position& position) const;

        /**
         * The size of the map.
         */
        size_t size_{ 0 };

        /**
         * The number of cell rows.
         */
        size_t rowCount_{ 0 };

        /**
         * The number of cell columns.
         */
        size_t columnCount_{ 0 };

        /**
         * The cells of this map.
         */
        std::vector<std::shared_ptr<MapCell>> cells_;
    };
}