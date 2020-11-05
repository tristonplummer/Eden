#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode representing either a request or an update about the state of a mob.
     */
    constexpr auto MobStateOpcode = 0x0304;

    /**
     * Represents a request to retrieve the state of a mob.
     */
    struct RequestMobState
    {
        /**
         * The opcode of this request
         */
        uint16_t opcode{ MobStateOpcode };

        /**
         * The mob's unique id
         */
        uint32_t id{ 0 };
    } PACKED;

    /**
     * Represents the mob's state.
     */
    struct MobState
    {
        /**
         * The opcode of this update
         */
        uint16_t opcode{ MobStateOpcode };

        /**
         * The mob's unique id
         */
        uint32_t id{ 0 };

        /**
         * The current hitpoints of the mob.
         */
        uint32_t hitpoints{ 0 };

        /**
         * The attack speed of the mob.
         */
        uint8_t attackSpeed{ 0 };

        /**
         * The movement speed of the mob.
         */
        uint8_t movementSpeed{ 0 };
    } PACKED;
}