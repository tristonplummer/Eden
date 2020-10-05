#pragma once
#include <shaiya/game/world/model/actor/character/Character.hpp>

namespace shaiya::game
{
    /**
     * Handles the synchronization of characters, for the current character. This task loads and removes characters from
     * the viewport based on their visibility and position relative to the character, and also notifies the character of
     * appearance changes for observable characters.
     */
    class CharacterSynchronizationTask
    {
    public:
        /**
         * Initialise the synchronization task.
         * @param character The character we're currently synchronizing.
         */
        explicit CharacterSynchronizationTask(Character& character);

        /**
         * Synchronizes the character.
         */
        void sync();

    private:
        /**
         * Adds a character to the current character's viewport.
         * @param other The character to add.
         */
        void addCharacter(const Character& other);

        /**
         * Removes a character from the current character's viewport.
         * @param other The character to remove.
         */
        void removeCharacter(const Character& other);

        /**
         * Update the appearance of a character, for the current character.
         * @param other The character to update.
         */
        void updateAppearance(const Character& other);

        /**
         * Updates the movement of a character, for the current character.
         * @param other The character to update.
         */
        void updateMovement(const Character& other);

        /**
         * Updates the movement state of a character, for the current character.
         * @param other The character to update.
         */
        void updateMovementState(const Character& other);

        /**
         * Checks if a character is observable to the current character.
         * @param other The other character.
         * @return      If the other character can be observed.
         */
        bool observable(const Character& other);

        /**
         * The character we're currently synchronizing.
         */
        Character& character_;
    };
}