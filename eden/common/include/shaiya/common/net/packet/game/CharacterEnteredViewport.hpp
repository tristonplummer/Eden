#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for signifying that a character entered the viewport
     */
    constexpr auto CharacterEnteredViewportOpcode = 0x0201;

    /**
     * Represents a message informing a player that a character has entered their viewport.
     */
    struct CharacterEnteredViewport
    {
        /**
         * The opcode of the viewport notification
         */
        uint16_t opcode{ CharacterEnteredViewportOpcode };

        /**
         * The id of the character that entered the viewport.
         */
        uint32_t id{ 0 };

        /**
         * The privilege level of the character.
         */
        uint8_t privilege{ 0 };

        /**
         * The direction of the character.
         */
        uint16_t direction{ 0 };

        /**
         * The x coordinate of the character.
         */
        float x{ 0 };

        /**
         * The y coordinate of the character.
         */
        float y{ 0 };

        /**
         * The z coordinate of the character.
         */
        float z{ 0 };

        /**
         * The guild id
         */
        uint32_t guild{ 0 };

        /**
         * The mount id
         */
        uint32_t mount{ 0 };
    } PACKED;

    /**
     * Represents a notification that the client has loaded the current viewport.
     */
    struct ViewportLoadedNotification
    {
        /**
         * The opcode for the viewport load notification.
         */
        uint16_t opcode{ CharacterEnteredViewportOpcode };
    } PACKED;
}