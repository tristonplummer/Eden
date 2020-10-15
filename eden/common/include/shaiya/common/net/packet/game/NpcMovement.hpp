#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing a player about an NPC's movement.
     */
    constexpr auto NpcMovementOpcode = 0x0E03;

    /**
     * Informs a player that an NPC has moved.
     */
    struct NpcMovement
    {
        /**
         * The opcode of this npc movement update.
         */
        uint16_t opcode{ NpcMovementOpcode };

        /**
         * The npc's id
         */
        uint32_t id{ 0 };

        /**
         * The motion of the npc
         */
        uint8_t motion{ 0 };

        /**
         * The x-coordinate of the npc
         */
        float x{ 0 };

        /**
         * The y-coordinate of the npc
         */
        float y{ 0 };

        /**
         * The z-coordinate of the npc
         */
        float z{ 0 };
    } PACKED;
}