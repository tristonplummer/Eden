#pragma once
#include <shaiya/game/Forward.hpp>

namespace shaiya::game
{
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
        explicit ActionBar(Player& character);

        /**
         * Informs the character about the state of their action bar.
         */
        void sync();

    private:
        /**
         * The character who owns this action bar.
         */
        Player& character_;
    };
}