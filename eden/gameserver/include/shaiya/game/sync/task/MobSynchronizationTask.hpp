#pragma once
#include <shaiya/game/Forward.hpp>

namespace shaiya::game
{
    /**
     * Handles the synchronization of mobs, for the current character. This task is responsible for processing the update
     * flags of each mob, such as if they have moved, or attack another actor.
     */
    class MobSynchronizationTask
    {
    public:
        /**
         * Initialise the synchronization task.
         * @param character The character we're currently synchronizing.
         */
        explicit MobSynchronizationTask(Player& character);

        /**
         * Synchronizes the character.
         */
        void sync();

        /**
         * Adds a mob to the current character's viewport.
         * @param other The mob to add.
         */
        void addMob(const Mob& other);

        /**
         * Removes a mob from the current character's viewport.
         * @param other The mob to remove.
         */
        void removeMob(const Mob& other);

    private:
        /**
         * Process the update flags for a mob.
         * @param other The mob to update for this character.
         */
        void processUpdateFlags(const Mob& other);

        /**
         * Updates the movement of an mob, for the current character.
         * @param other The mob to update.
         */
        void updateMovement(const Mob& other);

        /**
         * Updates the combat state of a mob.
         * @param other The mob to update.
         */
        void updateCombat(const Mob& other);

        /**
         * Handles the death of a mob.
         * @param other The mob to update.
         */
        void updateDeath(const Mob& other);

        /**
         * The character we're currently synchronizing.
         */
        Player& character_;
    };
}