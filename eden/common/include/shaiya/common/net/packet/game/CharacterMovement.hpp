#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for the character movement packet.
     */
    constexpr auto CharacterMovementOpcode = 0x0501;

    /**
     * Represents an update from a character to the server, about their movement.
     */
    struct CharacterMovement
    {
        /**
         * The opcode of the character movement.
         */
        uint16_t opcode{ CharacterMovementOpcode };

        /**
         * The motion, which indicates if the character has stopped moving, or if there
         * are more movements to follow.
         */
        uint8_t motion{ 0 };

        /**
         * The direction the character is facing.
         */
        uint16_t direction{ 0 };

        /**
         * The current x coordinate
         */
        float x{ 0 };

        /**
         * The current y coordinate
         */
        float y{ 0 };

        /**
         * The current z coordinate
         */
        float z{ 0 };
    } PACKED;

    /**
     * Represents an update to a character, about another character's movement.
     */
    struct CharacterMovementUpdate
    {
        /**
         * The opcode for the character movement update.
         */
        uint16_t opcode{ CharacterMovementOpcode };

        /**
         * The id of the character who's movement we are updating.
         */
        uint32_t id{ 0 };

        /**
         * The motion of the character.
         */
        uint8_t motion{ 0 };

        /**
         * The direction the character is facing.
         */
        uint16_t direction{ 0 };

        /**
         * The new x position of the character.
         */
        float x{ 0 };

        /**
         * The new y position of the character.
         */
        float y{ 0 };

        /**
         * The new z position of the character.
         */
        float z{ 0 };
    } PACKED;
}