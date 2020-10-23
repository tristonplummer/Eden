#pragma once
#include <shaiya/game/Forward.hpp>

#include <vector>

namespace shaiya::game
{
    /**
     * The client synchronizer manages the update sequence that keeps clients synchronized with the in-game world.
     */
    class ClientSynchronizer
    {
    public:
        /**
         * Synchronizes the state of the clients with the stat of the server.
         * @param characters    The vector containing the player characters.
         */
        virtual void synchronize(std::vector<std::shared_ptr<Player>> characters) = 0;
    };
}