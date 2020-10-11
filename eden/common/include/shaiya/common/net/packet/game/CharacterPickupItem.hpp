#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode representing that a player wants to pick up an item from the ground.
     */
    constexpr auto CharacterPickupItemRequestOpcode = 0x0205;

    /**
     * Represents a request that a player wants to pick up an item off the ground.
     */
    struct CharacterPickupItemRequest
    {
        /**
         * The opcode of the pickup request.
         */
        uint16_t opcode{ CharacterPickupItemRequestOpcode };

        /**
         * The id of the item to pick up.
         */
        uint32_t id{ 0 };
    } PACKED;
}