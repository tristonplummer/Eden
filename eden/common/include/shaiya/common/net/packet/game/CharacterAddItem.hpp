#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::game
{
    /**
     * The opcode for an item being added to a player's inventory.
     */
    constexpr auto CharacterAddItemOpcode = 0x0205;

    /**
     * Represents a notification that an item was added to a player's inventory.
     */
    struct CharacterAddItem
    {
        /**
         * The opcode of this update.
         */
        uint16_t opcode{ CharacterAddItemOpcode };

        /**
         * The page that the item was added to.
         */
        uint8_t page{ 0 };

        /**
         * The slot that the item is at.
         */
        uint8_t slot{ 0 };

        /**
         * The type of the item.
         */
        uint8_t type{ 0 };

        /**
         * The type id of the item.
         */
        uint8_t typeId{ 0 };

        /**
         * The number of items in this stack.
         */
        uint8_t count{ 0 };

        /**
         * The endurance of the item.
         */
        uint16_t endurance{ 0 };

        /**
         * TODO: Refactor this.
         */
        uint32_t unknown{ 0 };

        /**
         * The lapis ids that are linked to this item.
         */
        uint32_t lapis[6]{ 0 };

        /**
         * TODO: Refactor this.
         */
        VariableString<23> unknown2;

        /**
         * If the item is dyed.
         */
        bool isDyed{ false };

        /**
         * TODO: Refactor this.
         */
        VariableString<26> unknown3;

        /**
         * The craftname of the item.
         */
        VariableString<21> craftname;
    } PACKED;
}