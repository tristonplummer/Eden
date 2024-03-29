#pragma once
#include <shaiya/game/Forward.hpp>

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
        explicit CharacterSynchronizationTask(Player& character);

        /**
         * Synchronizes the character.
         */
        void sync();

        /**
         * Adds a character to the current character's viewport.
         * @param other The character to add.
         */
        void addCharacter(const Player& other);

        /**
         * Removes a character from the current character's viewport.
         * @param other The character to remove.
         */
        void removeCharacter(const Player& other);

    private:
        /**
         * Process the update flags for a character.
         * @param other The character to update for this character.
         */
        void processUpdateFlags(const Player& other);

        /**
         * Update the appearance of a character, for the current character.
         * @param other The character to update.
         */
        void updateAppearance(const Player& other);

        /**
         * Updates the movement of a character, for the current character.
         * @param other The character to update.
         */
        void updateMovement(const Player& other);

        /**
         * Updates the movement state of a character, for the current character.
         * @param other The character to update.
         */
        void updateMovementState(const Player& other);

        /**
         * Updates the chat of a character, for the other character.
         * @param other The character to update.
         */
        void updateChat(const Player& other);

        /**
         * The character we're currently synchronizing.
         */
        Player& character_;
    };
}