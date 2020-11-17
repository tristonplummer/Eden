#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing adding experience to a player.
     */
    constexpr auto PlayerAddExperienceOpcode = 0x0207;

    /**
     * Informs a player that they have gained experience.
     */
    struct PlayerAddExperience
    {
        /**
         * The opcode for informing a player about their experience.
         */
        uint16_t opcode{ PlayerAddExperienceOpcode };

        /**
         * The number of experience that was awarded to the player.
         */
        uint32_t experience{};

        /**
         * The perfect-party bonus.
         */
        uint32_t partyBonus{};
    } PACKED;
}