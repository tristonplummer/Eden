#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode representing a player selecting a mob as their target.
     */
    constexpr auto TargetSelectMobOpcode = 0x0305;

    /**
     * Represents a request to select a mob as a target.
     */
    struct TargetSelectMob
    {
        /**
         * The opcode of this request
         */
        uint16_t opcode{ TargetSelectMobOpcode };

        /**
         * The mob's unique id
         */
        uint32_t id{ 0 };
    } PACKED;
}