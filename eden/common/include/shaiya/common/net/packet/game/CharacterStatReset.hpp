#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode representing an update to a character's allocated stats.
     */
    constexpr auto CharacterStatResetOpcode = 0x0214;

    /**
     * Represents a reset to the character's stats.
     */
    struct CharacterStatReset
    {
        /**
         * The opcode of the stat stat reset.
         */
        uint16_t opcode{ CharacterStatResetOpcode };

        /**
         * If the stat reset was a success. (Why wouldn't it be, if we were sending this).
         */
        bool success{ true };

        /**
         * The number of stat points the character has available for allocation.
         */
        uint16_t statpoints{ 0 };

        /**
         * The number of points that are allocated into strength.
         */
        uint16_t strength{ 0 };

        /**
         * The number of points that are allocated into reaction.
         */
        uint16_t reaction{ 0 };

        /**
         * The number of points that are allocated into intelligence.
         */
        uint16_t intelligence{ 0 };

        /**
         * The number of points that are allocated into wisdom.
         */
        uint16_t wisdom{ 0 };

        /**
         * The number of points that are allocated into dexterity.
         */
        uint16_t dexterity{ 0 };

        /**
         * The number of points that are allocated into luck.
         */
        uint16_t luck{ 0 };
    } PACKED;
}