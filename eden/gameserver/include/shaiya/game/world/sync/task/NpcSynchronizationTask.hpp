#pragma once
#include <shaiya/game/world/model/actor/npc/Npc.hpp>

namespace shaiya::game
{
    /**
     * Handles the synchronization of npcs, for the current character. This task is responsible for processing the update
     * flags of each npc, such as if they have moved, or attack another actor.
     */
    class NpcSynchronizationTask
    {
    public:
        /**
         * Initialise the synchronization task.
         * @param character The character we're currently synchronizing.
         */
        explicit NpcSynchronizationTask(Character& character);

        /**
         * Synchronizes the character.
         */
        void sync();

        /**
         * Adds an NPC to the current character's viewport.
         * @param other The NPC to add.
         */
        void addNpc(const Npc& other);

        /**
         * Removes an NPC from the current character's viewport.
         * @param other The npc to remove.
         */
        void removeNpc(const Npc& other);

    private:
        /**
         * The character we're currently synchronizing.
         */
        Character& character_;
    };
}