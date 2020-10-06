#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for a regular player sending a chat message.
     */
    constexpr auto PlayerChatMessageOpcode = 0x1101;

    /**
     * The opcode for an admin sending a chat message.
     */
    constexpr auto AdminChatMessageOpcode = 0xF101;

    /**
     * The maximum length of a chat message.
     */
    constexpr auto MaxChatLength = 128;

    /**
     * Represents an incoming normal chat message.
     */
    struct CharacterChatMessage
    {
        /**
         * The opcode of the chat message (can be either 0x1101 or 0xF101)
         */
        uint16_t opcode{ PlayerChatMessageOpcode };

        /**
         * The length of the chat message.
         */
        uint8_t length{ 0 };

        /**
         * The chat message
         */
        VariableString<MaxChatLength> message;
    } PACKED;

    /**
     * Represents an outgoing normal chat message, to inform others that a character has
     * said something.
     */
    struct CharacterChatMessageUpdate
    {
        /**
         * The normal chat message opcode.
         */
        uint16_t opcode{ PlayerChatMessageOpcode };

        /**
         * The character id of the sender
         */
        uint32_t sender{0};

        /**
         * The length of the chat message
         */
        uint8_t length{ 0 };

        /**
         * The chat message
         */
        VariableString<MaxChatLength> message;
    } PACKED;
}