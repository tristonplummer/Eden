#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for the character movement packet.
     */
    constexpr auto CharacterMovementOpcode = 0x0501;

    /**
     * The motion type.
     */
    enum class MotionType : uint8_t
    {
        Stopped = 132,
        Moving  = 129
    };

    /**
     * Represents an update from a character to the server, about their movement.
     */
    struct CharacterMovement
    {
        /**
         * The motion, which indicates if the character has stopped moving, or if there
         * are more movements to follow.
         */
        MotionType motion{ MotionType::Stopped };

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
}