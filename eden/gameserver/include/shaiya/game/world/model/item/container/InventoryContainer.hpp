#pragma once
#include <shaiya/game/world/model/item/container/ItemContainer.hpp>

namespace shaiya::game
{
    /**
     * A specialised container that acts as an actor's inventory.
     */
    class InventoryContainer: public ItemContainer
    {
    public:
        /**
         * Initialises this inventory container.
         */
        explicit InventoryContainer();

        /**
         * Initialises this inventory container.
         * @param inventory The inventory to copy from.
         */
        InventoryContainer(const InventoryContainer& inventory);

        /**
         * Sets the amount of gold in the inventory.
         * @param gold  The amount of gold.
         */
        void setGold(size_t gold);

        /**
         * Gets the amount of gold in the inventory.
         * @return  The gold.
         */
        [[nodiscard]] size_t gold() const
        {
            return gold_;
        }

    private:
        /**
         * The amount of gold held by the actor.
         */
        size_t gold_{ 0 };
    };
}