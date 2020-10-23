#pragma once
#include <shaiya/game/world/sync/ClientSynchronizer.hpp>

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
         * @param characters    The vector containing the player characters.
         */
        void synchronize(std::vector<std::shared_ptr<Player>> characters) override;

    private:

        /**
         * Synchronizes a character.
         * @param character     The character to synchronize
         */
        void syncCharacter(Player& character);
    };
}