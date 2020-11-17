#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for a player levelling up.
     */
    constexpr auto PlayerLevelUpOpcode = 0x0508;

    /**
     * The opcode for another player levelling up.
     */
    constexpr auto OtherPlayerLevelledUp = 0x051E;

    /**
     * Informs a player that they have levelled up.
     */
    struct PlayerLevelUp
    {
        /**
         * The opcode for informing a player about their experience.
         */
        uint16_t opcode{ PlayerLevelUpOpcode };

        /**
         * The id of the player that has levelled up.
         */
        uint32_t id{};

        /**
         * The new level of the player.
         */
        uint16_t level{};

        /**
         * The amount of statpoints the user has available to them.
         */
        uint16_t statpoints{};

        /**
         * The amount of skillpoints the user has available to them.
         */
        uint16_t skillpoints{};

        /**
         * The experience of the previous level?
         */
        uint32_t prevExp{};

        /**
         * The experience required to level up again.
         */
        uint32_t nextExp{};
    } PACKED;
}