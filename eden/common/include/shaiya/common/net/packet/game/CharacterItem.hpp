#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for updating a character about their items.
     */
    constexpr auto CharacterItemOpcode = 0x0106;

    /**
     * The maximum number of items that can be updated as a time.
     */
    constexpr auto CharacterItemUpdateCapacity = 50;

    /**
     * The maximum number of lapis that can be socketed into an ite,
     */
    constexpr auto LapisCapacity = 6;

    /**
     * Represents an item being updated.
     */
    struct CharacterItemUnit
    {
        /**
         * The bag that the item resides in.
         */
        uint8_t bag{ 0 };

        /**
         * The slot that the item resides in.
         */
        uint8_t slot{ 0 };

        /**
         * The item type.
         */
        uint8_t type{ 0 };

        /**
         * The item type id.
         */
        uint8_t typeId{ 0 };

        /**
         * The current durability of the item.
         */
        uint16_t durability{ 0 };

        /**
         * The type ids of the lapis that are socketed into this item.
         */
        std::array<uint8_t, LapisCapacity> lapis{ 0 };

        /**
         * The number of items in this stack.
         */
        uint8_t count{ 0 };

        /**
         * An ASCII representation of the orange stats.
         */
        VariableString<21> craftname;
    };

    /**
     * Represents a message informing a character about the items in their inventory
     */
    struct CharacterItemUpdate
    {
        /**
         * The for updating a character about their items.
         */
        uint16_t opcode{ CharacterItemOpcode };

        /**
         * The number of items in this update.
         */
        uint8_t count{ 0 };

        /**
         * The item units that are being updated.
         */
        std::array<CharacterItemUnit, CharacterItemUpdateCapacity> items;
    } PACKED;
}