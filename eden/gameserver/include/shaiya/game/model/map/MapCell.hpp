#pragma once
#include <shaiya/game/Forward.hpp>

#include <memory>
#include <vector>

namespace shaiya::game
{
    /**
     * Represents a 16x16 cell of a map. This is so that when processing localised events, we only need to search
     * small areas to find players that can observe or be affected by such event.
     */
    class MapCell
    {
    public:
        /**
         * Initialises this map cell.
         * @param map       The map instance.
         * @param row       The row
         * @param column    The column
         */
        MapCell(Map& map, size_t row, size_t column);

        /**
         * Adds an entity to this cell.
         * @param entity    The entity to add.
         */
        void addEntity(const std::shared_ptr<Entity>& entity);

        /**
         * Removes an entity from this cell.
         * @param entity    The entity to remove.
         */
        void removeEntity(const std::shared_ptr<Entity>& entity);

        /**
         * Gets the neighbouring cells.
         * @return  The neighbouring cells.
         */
        const std::vector<std::shared_ptr<MapCell>>& neighbours();

        /**
         * Gets the entities in this cell.
         * @return  The entities.
         */
        [[nodiscard]] const std::vector<std::shared_ptr<Entity>>& entities() const
        {
            return entities_;
        }

    private:
        /**
         * The entities that exist inside this cell.
         */
        std::vector<std::shared_ptr<Entity>> entities_;

        /**
         * The vector of neighbouring cells.
         */
        std::vector<std::shared_ptr<MapCell>> neighbours_;

        /**
         * The map instance.
         */
        Map& map_;

        /**
         * The row.
         */
        size_t row_{};

        /**
         * The column
         */
        size_t column_{};
    };
}