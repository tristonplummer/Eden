#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/net/packet/game/CharacterItem.hpp>

namespace shaiya::net
{
    /**
     * The opcode for a character moving an item in an inventory / equipment container.
     */
    constexpr auto CharacterMoveItemOpcode = 0x0204;

    /**
     * This is very similiar in structure to the CharacterItemUnit struct defined in CharacterItem.hpp, but here the item
     * count appears after the type id, rather than after the lapis.
     */
    struct CharacterMoveItemUnit
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
         * The number of items in this stack.
         */
        uint8_t count{ 0 };

        /**
         * The current durability of the item.
         */
        uint16_t durability{ 0 };

        /**
         * TODO: Refactor this
         */
        VariableString<23> unknown;

        /**
         * If this item is dyed.
         */
        bool dyed{ false };

        /**
         * TODO: Refactor this.
         */
        VariableString<26> unk;

        /**
         * The type ids of the lapis that are socketed into this item.
         */
        std::array<uint32_t, LapisCapacity> lapis{ 0 };

        /**
         * An ASCII representation of the orange stats.
         */
        VariableString<21> craftname;
    } PACKED;

    /**
     * Represents a request for a character to move items between their inventory and equipment.
     */
    struct CharacterMoveItemRequest
    {
        /**
         * The opcode representing a character that wants to move an item in a container.
         */
        uint16_t opcode{ CharacterMoveItemOpcode };

        /**
         * The source page (0 = Equipment, 1-5 = Inventory)
         */
        uint8_t sourcePage{ 0 };

        /**
         * The source slot in the page.
         */
        uint8_t sourceSlot{ 0 };

        /**
         * The destination page.
         */
        uint8_t destPage{ 0 };

        /**
         * The destination slot in the page.
         */
        uint8_t destSlot{ 0 };
    } PACKED;

    /**
     * Updates that character and informs them that items have been moved between containers.
     */
    struct CharacterMoveItemUpdate
    {
        /**
         * The opcode representing the item move update.
         */
        uint16_t opcode{ CharacterMoveItemOpcode };

        /**
         * The item at the source position.
         */
        CharacterMoveItemUnit source;

        /**
         * The item at the destination position.
         */
        CharacterMoveItemUnit destination;
    } PACKED;
}