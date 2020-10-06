#pragma once
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
         * Initialises an item by an item id.
         * @param id    The item id.
         */
        explicit Item(uint32_t id);

        /**
         * Initialises an item by a type and type id.
         * @param type      The item type.
         * @param typeId    The type id.
         */
        Item(uint8_t type, uint8_t typeId);

        /**
         * Gets the item count.
         * @return  The number of items in this stack.
         */
        [[nodiscard]] uint8_t count() const
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
            return id_;
        }

        /**
         * Get the type of this item.
         * @return  The type.
         */
        [[nodiscard]] uint8_t type() const
        {
            return std::floor(id_ / 1000);
        }

        /**
         * Get the type id of this item.
         * @return  The type id.
         */
        [[nodiscard]] uint8_t typeId() const
        {
            return std::floor(id_ - (type() * 1000));
        }

    private:
        /**
         * The item id.
         */
        uint32_t id_{ 0 };

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