#pragma once
#include <memory>
#include <vector>

namespace shaiya::game
{
    class Entity;  // Forward declaration of an entity.

    /**
     * Represents a 16x16 cell of a map. This is so that when processing localised events, we only need to search
     * small areas to find players that can observe or be affected by such event.
     */
    class MapCell
    {
    public:
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
         * Gets the entities in this cell.
         * @return  The entities.
         */
        [[nodiscard]] std::vector<std::shared_ptr<Entity>> entities() const
        {
            return entities_;
        }

    private:
        /**
         * The entities that exist inside this cell.
         */
        std::vector<std::shared_ptr<Entity>> entities_;
    };
}