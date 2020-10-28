#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::client
{
    /**
     * Represents the position that an NPC is spawned at.
     */
    struct NpcSpawnPosition
    {
        /**
         * The position.
         */
        Shaiya3DVector position{};

        /**
         * The direction that the NPC is facing, in radians.
         */
        float dir{};
    } PACKED;
}