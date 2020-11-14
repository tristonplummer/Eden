#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/net/packet/game/AttackStatus.hpp>

namespace shaiya::net
{
    /**
     * The opcode representing that the attack sequence has started.
     */
    constexpr auto PlayerAttackStartOpcode = 0x0212;

    /**
     * Represents the start of the attack sequence.
     */
    struct PlayerAttackStart
    {
        /**
         * The opcode
         */
        uint16_t opcode{ PlayerAttackStartOpcode };
    } PACKED;
}