#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::net
{
    /**
     * The opcode for a ground item being added to the map.
     */
    constexpr auto GroundItemAddedOpcode = 0x0401;

    /**
     * The opcode for a ground item being removed from the map.
     */
    constexpr auto GroundItemRemovedOpcode = 0x0402;

    /**
     * Represents a notification that an item has been placed onto the ground.
     */
    struct GroundItemAdded
    {
        /**
         * The opcode of this notification.
         */
        uint16_t opcode{ GroundItemAddedOpcode };

        /**
         * The unique id of the ground item.
         */
        uint32_t id{ 0 };

        /**
         * The "kind"? TODO: Refactor this value. Only seems to ever be 1 or 2, and it doesn't affect
         * anything that the client displays.
         */
        uint8_t kind{ 1 };

        /**
         * The type of the item.
         */
        uint8_t type{ 0 };

        /**
         * The type id of the item.
         */
        uint8_t typeId{ 0 };

        /**
         * The number of the item in this ground item. We'll always set this to 0, as the client doesn't need to know.
         */
        uint8_t quantity{ 0 };

        /**
         * The x coordinate of the item.
         */
        float x{ 0 };

        /**
         * The y coordinate of the item.
         */
        float y{ 0 };

        /**
         * The z coordinate of the item.
         */
        float z{ 0 };

        /**
         * If this value is set to the player's own id, then a bright purple beam is displayed for ~30 seconds after the item
         * is first displayed. This is used to make the player aware of valuable item drops.
         */
        uint32_t playerId{ 0 };
    } PACKED;

    /**
     * Represents a notification that an item  has been removed from the ground.
     */
    struct GroundItemRemoved
    {
        /**
         * The opcode of this ground item notification.
         */
        uint16_t opcode{ GroundItemRemovedOpcode };

        /**
         * The id of the ground item that was removed.
         */
        uint32_t id{ 0 };
    } PACKED;
}