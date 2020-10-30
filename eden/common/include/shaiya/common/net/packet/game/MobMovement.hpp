#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing a player about a mob's movement.
     */
    constexpr auto MobMovementOpcode = 0x0603;

    /**
     * Informs a player that a mob has moved.
     */
    struct MobMovement
    {
        /**
         * The opcode of this mob movement update.
         */
        uint16_t opcode{ MobMovementOpcode };

        /**
         * The npc's id
         */
        uint32_t id{ 0 };

        /**
         * If the mob is running
         */
        bool running{ false };

        /**
         * The x-coordinate of the mob
         */
        float x{ 0 };

        /**
         * The z-coordinate of the mob
         */
        float z{ 0 };
    } PACKED;
}