#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing a player that a mob has left their viewport.
     */
    constexpr auto MobLeftViewportOpcode = 0x0602;

    /**
     * Informs the player that a mob has left their viewport.
     */
    struct MobLeftViewport
    {
        /**
         * The opcode of this notification.
         */
        uint16_t opcode{ MobLeftViewportOpcode };

        /**
         * The id of the mob to remove from the viewport.
         */
        uint32_t id{ 0 };
    } PACKED;
}