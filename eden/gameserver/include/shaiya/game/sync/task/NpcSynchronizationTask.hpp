#pragma once
#include <shaiya/game/Forward.hpp>

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
        explicit NpcSynchronizationTask(Player& character);

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
         * Process the update flags for an NPC.
         * @param other The NPC to update for this character.
         */
        void processUpdateFlags(const Npc& other);

        /**
         * Updates the movement of an NPC, for the current character.
         * @param other The NPC to update.
         */
        void updateMovement(const Npc& other);

        /**
         * The character we're currently synchronizing.
         */
        Player& character_;
    };
}