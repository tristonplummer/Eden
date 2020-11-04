#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/util/EntityContainer.hpp>

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
         * @param players   The vector containing the player characters.
         * @param npcs      The npc container.
         * @param mobs      The mob container.
         */
        virtual void synchronize(std::vector<std::shared_ptr<Player>> players, const EntityContainer<Npc>& npcs,
                                 const EntityContainer<Mob>& mobs) = 0;
    };
}