#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing a player that a mob has entered their viewport.
     */
    constexpr auto MobEnteredViewportOpcode = 0x0601;

    /**
     * Informs a player that an NPC has entered their opcode.
     */
    struct MobEnteredViewport
    {
        /**
         * The opcode of this notification
         */
        uint16_t opcode{ MobEnteredViewportOpcode };

        /**
         * The mob's unique id
         */
        uint32_t id{ 0 };

        /**
         * If the mob just respawned
         */
        bool isRespawn{ false };

        /**
         * The id of the mob.
         */
        uint16_t mobId{};

        /**
         * The x coordinate of the mob.
         */
        float x{ 0 };

        /**
         * The z coordinate of the mob.
         */
        float z{ 0 };
    } PACKED;
}