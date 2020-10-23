#pragma once
#include <shaiya/game/Forward.hpp>

namespace shaiya::game
{
    /**
     * Represents a player's action bar.
     */
    class ActionBar
    {
    public:
        /**
         * Initialise the action bar for a player.
         * @param player The player instance.
         */
        explicit ActionBar(Player& player);

        /**
         * Informs the character about the state of their action bar.
         */
        void sync();

    private:
        /**
         * The player who owns this action bar.
         */
        Player& player_;
    };
}