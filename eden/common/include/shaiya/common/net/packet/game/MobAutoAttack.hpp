#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/net/packet/game/AttackStatus.hpp>

namespace shaiya::net
{
    /**
     * The opcode representing a mob attacking a player.
     */
    constexpr auto MobAutoAttackOpcode = 0x0605;

    /**
     * Represents an auto attack made by a mob.
     */
    struct MobAutoAttack
    {
        /**
         * The opcode of the attack.
         */
        uint16_t opcode{ MobAutoAttackOpcode };

        /**
         * The status of the attack.
         */
        AttackStatus status{ AttackStatus::Miss };

        /**
         * The id of the attacking mob.
         */
        uint32_t id{ 0 };

        /**
         * The id of the target player.
         */
        uint32_t target{ 0 };

        /**
         * The damage done to the player's hitpoints
         */
        uint16_t hitpointDamage{ 0 };

        /**
         * The damage done to the player's mana
         */
        uint16_t manaDamage{ 0 };

        /**
         * The damage done to the player's stamina
         */
        uint16_t staminaDamage{ 0 };
    } PACKED;
}