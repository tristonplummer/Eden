#pragma once

namespace shaiya::game
{
    class Character;

    /**
     * Represents a character's action bar.
     */
    class ActionBar
    {
    public:
        /**
         * Initialise the action bar for a character.
         * @param character The character instance.
         */
        explicit ActionBar(Character& character);

        /**
         * Informs the character about the state of their action bar.
         */
        void sync();

    private:
        /**
         * The character who owns this action bar.
         */
        Character& character_;
    };
}