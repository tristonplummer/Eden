#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/net/packet/game/AttackStatus.hpp>

namespace shaiya::net
{
    /**
     * The opcode that represents a player auto attacking another player.
     */
    constexpr auto PlayerAutoAttackPlayerOpcode = 0x0502;

    /**
     * The opcode that represents a player auto attacking a mob.
     */
    constexpr auto PlayerAutoAttackMobOpcode = 0x0503;

    /**
     * Represents a request to attack an actor.
     */
    struct PlayerAutoAttackRequest
    {
        /**
         * The opcode of the request.
         */
        uint16_t opcode{ PlayerAutoAttackMobOpcode };

        /**
         * The target id.
         */
        uint32_t target{ 0 };
    } PACKED;

    /**
     * Represents an attack from a player.
     */
    struct PlayerAutoAttack
    {
        /**
         * The opcode of the attack
         */
        uint16_t opcode{ PlayerAutoAttackMobOpcode };

        /**
         * The status of the attack
         */
        AttackStatus status{ AttackStatus::Miss };

        /**
         * The id of the player
         */
        uint32_t id{ 0 };

        /**
         * The id of the target actor
         */
        uint32_t target{ 0 };

        /**
         * The damage done to the actor's hitpoints
         */
        uint16_t hitpointDamage{ 0 };

        /**
         * The damage done to the actor's mana
         */
        uint16_t manaDamage{ 0 };

        /**
         * The damage done to the actor's stamina
         */
        uint16_t staminaDamage{ 0 };
    } PACKED;
}