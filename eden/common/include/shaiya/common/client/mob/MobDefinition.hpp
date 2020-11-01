#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::client
{
    /**
     * Represents the definition of a mob.
     */
    struct MobDefinition
    {
        /**
         * The id of the mob.
         */
        uint16_t id{};
    } PACKED;
}