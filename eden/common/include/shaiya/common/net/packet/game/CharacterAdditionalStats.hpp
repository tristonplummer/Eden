#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for sending a character's additional stats to them.
     */
    constexpr auto CharacterAdditionalStatsOpcode = 0x0526;

    /**
     * Represents an update about a character's additional stats.
     */
    struct CharacterAdditionalStats
    {
        /**
         * The opcode for a a character's stats.
         */
        uint16_t opcode{ CharacterAdditionalStatsOpcode };

        /**
         * The additional strength of the character.
         */
        uint32_t strength{ 0 };

        /**
         * The reaction of the character.
         */
        uint32_t reaction{ 0 };

        /**
         * The intelligence of the character.
         */
        uint32_t intelligence{ 0 };

        /**
         * The wisdom of the character.
         */
        uint32_t wisdom{ 0 };

        /**
         * The dexterity of the character.
         */
        uint32_t dexterity{ 0 };

        /**
         * The luck of the character
         */
        uint32_t luck{ 0 };

        /**
         * The minimum attack of the character. If the character is an Archer, this is their minimum ranged attack power -
         * otherwise, this is the minimum physical attack power.
         */
        uint32_t minAttack{ 0 };

        /**
         * The maximum attack of the character. If the character is an Archer, this is their maximum ranged attack power -
         * otherwise, this is the maximum physical attack power.
         */
        uint32_t maxAttack{ 0 };

        /**
         * The minimum magical attack of the character.
         */
        uint32_t minMagicAttack{ 0 };

        /**
         * The maximum magical attack of the character.
         */
        uint32_t maxMagicAttack{ 0 };

        /**
         * The physical defense of the character.
         */
        uint32_t defense{ 0 };

        /**
         * The magical resistance of the character.
         */
        uint32_t resistance{ 0 };
    } PACKED;
}