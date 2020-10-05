#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for signifying that a character left the viewport
     */
    constexpr auto CharacterLeftViewportOpcode = 0x0202;

    /**
     * Represents a message informing a player that a character has left their viewport.
     */
    struct CharacterLeftViewport
    {
        /**
         * The opcode of the viewport notification
         */
        uint16_t opcode{ CharacterLeftViewportOpcode };

        /**
         * The id of the character that left the viewport.
         */
        uint32_t id{ 0 };
    } PACKED;
}