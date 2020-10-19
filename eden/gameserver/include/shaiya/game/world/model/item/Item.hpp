#pragma once
#include <shaiya/common/client/item/ItemDefinition.hpp>

#include <cmath>
#include <cstdint>

namespace shaiya::game
{
    /**
     * Represents an item held by an actor in the game world.
     */
    class Item
    {
    public:
        /**
         * Initialises an item by it's definition.
         * @param id    The item definition
         */
        explicit Item(const shaiya::client::ItemDefinition& definition);

        /**
         * Set the item count.
         * @param count The number in this stack.
         */
        void setCount(size_t count);

        /**
         * Gets the item count.
         * @return  The number of items in this stack.
         */
        [[nodiscard]] size_t count() const
        {
            return count_;
        }

        /**
         * Gets the item durability.
         * @return  The item durability.
         */
        [[nodiscard]] uint16_t durability() const
        {
            return durability_;
        }

        /**
         * Get the item id of this item.
         * @return  The id.
         */
        [[nodiscard]] uint32_t itemId() const
        {
            return definition_.id;
        }

        /**
         * Get the type of this item.
         * @return  The type.
         */
        [[nodiscard]] uint8_t type() const
        {
            return definition_.type;
        }

        /**
         * Get the type id of this item.
         * @return  The type id.
         */
        [[nodiscard]] uint8_t typeId() const
        {
            return definition_.typeId;
        }

        /**
         * Gets the definition of this item.
         * @return  The item definition.
         */
        [[nodiscard]] const shaiya::client::ItemDefinition& definition() const
        {
            return definition_;
        }

    private:
        /**
         * The item definition
         */
        const shaiya::client::ItemDefinition& definition_;

        /**
         * The item count.
         */
        uint32_t count_{ 1 };

        /**
         * The item durability
         */
        uint32_t durability_{ 0 };
    };
}