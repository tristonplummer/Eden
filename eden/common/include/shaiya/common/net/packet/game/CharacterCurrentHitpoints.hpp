#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing a player about their current health, mana and stamina
     */
    constexpr auto CurrentHitpointsOpcode = 0x0521;

    /**
     * Represents an incoming normal chat message.
     */
    struct CharacterCurrentHitpoints
    {
        /**
         * The opcode of the health update.
         */
        uint16_t opcode{ CurrentHitpointsOpcode };

        /**
         * The hitpoints of the character.
         */
        uint32_t hitpoints{ 0 };

        /**
         * The mana of the character.
         */
        uint32_t mana{ 0 };

        /**
         * The stamina of the character.
         */
        uint32_t stamina{ 0 };
    } PACKED;
}