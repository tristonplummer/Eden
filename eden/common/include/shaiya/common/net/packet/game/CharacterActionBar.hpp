#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing a character about their action bar.
     */
    constexpr auto CharacterActionBarOpcode = 0x010B;

    /**
     * The maximum number of action bar items.
     */
    constexpr auto MaxActionBarItems = 256;

    /**
     * An item on the action bar.
     */
    struct ActionBarItem
    {
        /**
         * The bar index.
         */
        uint8_t bar{ 0 };

        /**
         * The slot on the action bar.
         */
        uint8_t slot{ 0 };

        /**
         * The type of entry.
         * 0-5      = Inventory item
         * Skill    = 100
         * Emote    = 100+
         */
        uint8_t type{ 0 };

        /**
         * The value. This is the item slot if it's an item, or the skill id
         * if it's a skill.
         */
        uint16_t value{ 0 };
    } PACKED;

    /**
     * Represents an update about a character's action bar.
     */
    struct CharacterActionBarUpdate
    {
        /**
         * The opcode for an action bar update.
         */
        uint16_t opcode{ CharacterActionBarOpcode };

        /**
         * The number of entries in this update.
         */
        uint8_t count{ 0 };

        /**
         * The items on the action bar.
         */
        ActionBarItem items[MaxActionBarItems];
    } PACKED;
}