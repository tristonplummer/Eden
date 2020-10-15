#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for the character selection packet.
     */
    constexpr auto CharacterSelectOpcode = 0x0104;

    /**
     * Represents a request for a session to select and enter the game world as a character.
     */
    struct CharacterSelectionRequest
    {
        /**
         * The opcode of the character selection request.
         */
        uint16_t opcode{ CharacterSelectOpcode };

        /**
         * The character id that was requested.
         */
        uint32_t charId{ 0 };
    } PACKED;

    /**
     * Represents the response to a character selection request.
     */
    struct CharacterSelectionResponse
    {
        /**
         * The opcode of the character selection request.
         */
        uint16_t opcode{ CharacterSelectOpcode };

        /**
         * If the selection was successful
         */
        NexonBoolean success{ false };

        /**
         * The players own character id.
         */
        uint32_t charId{ 0 };
    } PACKED;
}