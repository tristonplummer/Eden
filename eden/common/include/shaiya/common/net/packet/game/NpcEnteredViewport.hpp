#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing a player than an NPC has entered their viewport.
     */
    constexpr auto NpcEnteredViewportOpcode = 0x0E01;

    /**
     * Informs a player that an NPC has entered their opcode.
     */
    struct NpcEnteredViewport
    {
        /**
         * The opcode of this notification
         */
        uint16_t opcode{ NpcEnteredViewportOpcode };

        /**
         * The npc's unique id
         */
        uint32_t id{ 0 };

        /**
         * The npc's type
         */
        uint8_t type{ 0 };

        /**
         * The npc's type id.
         */
        uint16_t typeId{ 0 };

        /**
         * The x coordinate of the npc.
         */
        float x{ 0 };

        /**
         * The y coordinate of the npc.
         */
        float y{ 0 };

        /**
         * The z coordinate of the npc.
         */
        float z{ 0 };

        /**
         * The direction that the npc is facing.
         */
        uint16_t direction{ 0 };
    } PACKED;
}