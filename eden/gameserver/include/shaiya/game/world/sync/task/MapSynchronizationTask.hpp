#pragma once
#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/model/item/GroundItem.hpp>

namespace shaiya::game
{
    /**
     * Handles the synchronization of the map for the current character. This includes displaying and removing ground items,
     * as well as graphic effects.
     */
    class MapSynchronizationTask
    {
    public:
        /**
         * Initialise the synchronization task.
         * @param character The character we're currently synchronizing.
         */
        explicit MapSynchronizationTask(Character& character);

        /**
         * Synchronizes the character.
         */
        void sync();

    private:
        /**
         * The character we're currently synchronizing.
         */
        Character& character_;
    };
}