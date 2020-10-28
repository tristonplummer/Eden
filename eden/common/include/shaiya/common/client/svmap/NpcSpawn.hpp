#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/client/svmap/NpcSpawnPosition.hpp>

namespace shaiya::client
{
    /**
     * Represents an NPC spawn entry.
     */
    struct NpcSpawn
    {
        /**
         * The type of the npc.
         */
        uint32_t type{};

        /**
         * The type id of the npc.
         */
        uint32_t typeId{};

        /**
         * The number of spawns in this map.
         */
        uint32_t quantity{};

        /**
         * The positions that this npc is spawned at.
         */
        std::vector<NpcSpawnPosition> positions{};
    } PACKED;
}