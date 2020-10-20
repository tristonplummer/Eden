#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode representing that an item should be removed from the character's inventory.
     */
    constexpr auto CharacterRemoveItemRequestOpcode = 0x0206;

    /**
     * Represents a request for a player to drop an item on the ground.
     */
    struct CharacterRemoveItemRequest
    {
        /**
         * The opcode of the drop item request.
         */
        uint16_t opcode{ CharacterRemoveItemRequestOpcode };

        /**
         * The page to drop the item from.
         */
        uint8_t page{ 0 };

        /**
         * The slot to drop the item from.
         */
        uint8_t slot{ 0 };

        /**
         * The amount of the item to drop.
         */
        uint8_t quantity{ 0 };
    } PACKED;

    /**
     * Represents an item that was removed from the character's inventory.
     */
    struct CharacterRemoveItem
    {
        /**
         * The opcode of the drop item request.
         */
        uint16_t opcode{ CharacterRemoveItemRequestOpcode };

        /**
         * The page to drop the item from.
         */
        uint8_t page{ 0 };

        /**
         * The slot to drop the item from.
         */
        uint8_t slot{ 0 };

        /**
         * The type of the item in this slot.
         */
        uint8_t type{ 0 };

        /**
         * The type id of the item in this slot.
         */
        uint8_t typeId{ 0 };

        /**
         * The remaining amount of the item in this slot.
         */
        uint8_t quantity{ 0 };
    } PACKED;
}