#pragma once
#include <shaiya/game/sync/ClientSynchronizer.hpp>

namespace shaiya::game
{
    /**
     * The parallel client synchronizer is designed to synchronize entities in parallel, and is intended to be used
     * on machines that have multiple cores/processors.
     */
    class ParallelClientSynchronizer: public ClientSynchronizer
    {
    public:
        /**
         * Synchronizes the state of the clients with the stat of the server.
         * @param players    The vector containing the player characters.
         */
        void synchronize(std::vector<std::shared_ptr<Player>> players) override;

    private:
        /**
         * Synchronizes a player.
         * @param player     The player to synchronize
         */
        void syncCharacter(Player& player);
    };
}