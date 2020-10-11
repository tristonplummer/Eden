#pragma once
#include <shaiya/game/world/model/Entity.hpp>
#include <shaiya/game/world/model/item/Item.hpp>

#include <memory>

namespace shaiya::game
{
    /**
     * Represents an item that was dropped on the ground.
     */
    class GroundItem: public Entity
    {
    public:
        /**
         * Creates an item that is to be dropped on the ground.
         * @param item  The item instance.
         * @param world The world instance.
         */
        GroundItem(std::shared_ptr<Item> item, GameWorldService& world);

        /**
         * Gets the item held by this ground item.
         * @return  The item instance.
         */
        [[nodiscard]] std::shared_ptr<Item> item() const
        {
            return item_;
        }

    private:
        /**
         * The item instance held by this ground item.
         */
        std::shared_ptr<Item> item_;
    };
}