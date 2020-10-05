#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for updating a character's movement state.
     */
    constexpr auto CharacterMovementStateOpcode = 0x0506;

    /**
     * Represents a state of movement.
     */
    enum class MovementState : uint8_t
    {
        Standing,
        Sitting,
        Jumping,
        Backflip
    };

    /**
     * Represents an update to a character's movement state.
     */
    struct MovementStateUpdate
    {
        /**
         * The movement state update opcode.
         */
        uint16_t opcode{ CharacterMovementStateOpcode };

        /**
         * The new movement state value.
         */
        MovementState state{ MovementState::Standing };
    } PACKED;

    /**
     * Represents a notification about another character's movement state.
     */
    struct MovementStateNotification
    {
        /**
         * The movement state update opcode.
         */
        uint16_t opcode{ CharacterMovementStateOpcode };

        /**
         * The id of the character.
         */
        uint32_t id{ 0 };

        /**
         * The new movement state value.
         */
        MovementState state{ MovementState::Standing };
    } PACKED;
}