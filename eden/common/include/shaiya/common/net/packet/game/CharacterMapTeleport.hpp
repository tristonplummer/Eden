#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for informing a player that they have teleported.
     */
    constexpr auto CharacterMapTeleportOpcode = 0x020B;

    /**
     * Represents an update about a player that has teleported.
     */
    struct CharacterMapTeleport
    {
        /**
         * The opcode for the teleportation update.
         */
        uint16_t opcode{ CharacterMapTeleportOpcode };

        /**
         * The character id.
         */
        uint32_t id{ 0 };

        /**
         * The destination map
         */
        uint16_t map{ 0 };

        /**
         * The destination x coordinate
         */
        float x{ 0 };

        /**
         * The destination y coordinate
         */
        float y{ 0 };

        /**
         * The destination z coordinate
         */
        float z{ 0 };
    } PACKED;
}