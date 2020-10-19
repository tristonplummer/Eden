#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/world/model/EntityType.hpp>

#include <fstream>
#include <memory>
#include <vector>

namespace shaiya::game
{
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
        void add(std::shared_ptr<Entity> entity) const;

        /**
         * Removes an entity from this map.
         * @param entity    The entity to remove.
         */
        void remove(std::shared_ptr<Entity> entity) const;

        /**
         * Attempts to get an entity with a specified id and type.
         * @param pos   The position to start the search at.
         * @param id    The entity id.
         * @param type  The entity type to search for.
         * @return      The entity instance.
         */
        template<typename T>
        std::shared_ptr<T> get(Position& pos, size_t id, EntityType type)
        {
            return std::dynamic_pointer_cast<T>(get(pos, id, type));
        }

        /**
         * Attempts to get an entity with a specified id and type.
         * @param pos   The position to start the search at.
         * @param id    The entity id.
         * @param type  The entity type to search for.
         * @return      The entity instance.
         */
        std::shared_ptr<Entity> get(Position& pos, size_t id, EntityType type);

        /**
         * Gets the cells in a neighbouring radius of a position.
         * @param position  The position.
         * @return          The neighbouring cells.
         */
        [[nodiscard]] std::vector<std::shared_ptr<MapCell>> getNeighbouringCells(Position& position) const;

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
         * Get a cell in the map based on a position.
         * @param position  The position.
         * @return          The map cell.
         */
        [[nodiscard]] std::shared_ptr<MapCell> getCell(Position& position) const;

        /**
         * Get a cell index  based on a position.
         * @param position  The position.
         * @return          The map cell.
         */
        [[nodiscard]] size_t getCellIndex(Position& position) const;

        /**
         * Adjusts a position to fit into the boundaries of this map.
         * @param position  The position to adjust.
         */
        void adjustPosition(Position& position) const;

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