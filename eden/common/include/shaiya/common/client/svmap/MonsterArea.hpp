#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/client/svmap/MonsterSpawn.hpp>

#include <vector>

namespace shaiya::client
{
    /**
     * Represents an area that contains monsters.
     */
    struct MonsterArea
    {
        /**
         * The bottom-left of the bounding box.
         */
        Shaiya3DVector bottomLeft{};

        /**
         * The top right of the bounding box.
         */
        Shaiya3DVector topRight{};

        /**
         * The number of spawns.
         */
        uint32_t recordQty{};

        /**
         * The monster spawn entries.
         */
        std::vector<MonsterSpawn> spawns{};
    } PACKED;
}