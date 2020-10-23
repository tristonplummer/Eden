#pragma once

namespace shaiya::game
{
    /**
     * Represents the type of an entity.
     */
    enum class EntityType
    {
        /**
         * A generic entity.
         */
        Entity,

        /**
         * A player character.
         */
        Player,

        /**
         * An item
         */
        Item,

        /**
         * A non-player character
         */
        Npc,
    };
}