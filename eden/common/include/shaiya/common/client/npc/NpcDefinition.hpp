#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::client
{
    /**
     * Represents the definition of an NPC.
     */
    struct NpcDefinition
    {
        /**
         * The type of the npc.
         */
        uint8_t type{};

        /**
         * The type id of the npc.
         */
        uint16_t typeId{};
    } PACKED;
}