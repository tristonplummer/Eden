#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/net/packet/game/AttackStatus.hpp>

namespace shaiya::net
{
    /**
     * The opcode representing a mob attacking a player.
     */
    constexpr auto MobDeathOpcode = 0x0606;

    /**
     * Represents the death of a mob.
     */
    struct MobDeath
    {
        /**
         * The opcode of the death.
         */
        uint16_t opcode{ MobDeathOpcode };

        /**
         * The id of the mob that has died.
         */
        uint32_t id{ 0 };

        /**
         * The type of the killer.
         * 1 = Player
         * 2 = Mob
         * 3 = Npc
         */
        uint8_t type{ 1 };

        /**
         * The id of the killer.
         */
        uint32_t killer{ 0 };
    } PACKED;
}