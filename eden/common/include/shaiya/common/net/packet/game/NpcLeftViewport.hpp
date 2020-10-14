#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing a player that an NPC has left their viewport.
     */
    constexpr auto NpcLeftViewportOpcode = 0x0E02;

    /**
     * Informs the player that an NPC has left their viewport.
     */
    struct NpcLeftViewport
    {
        /**
         * The opcode of this notification.
         */
        uint16_t opcode{ NpcLeftViewportOpcode };

        /**
         * The id of the npc to remove from the viewport.
         */
        uint32_t id{ 0 };
    } PACKED;
}