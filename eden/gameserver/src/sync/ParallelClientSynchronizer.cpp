#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/npc/Npc.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/item/GroundItem.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/model/map/MapCell.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/sync/ParallelClientSynchronizer.hpp>
#include <shaiya/game/sync/task/CharacterSynchronizationTask.hpp>
#include <shaiya/game/sync/task/MapSynchronizationTask.hpp>
#include <shaiya/game/sync/task/MobSynchronizationTask.hpp>
#include <shaiya/game/sync/task/NpcSynchronizationTask.hpp>

#include <execution>

using namespace shaiya::game;

/**
 * Synchronizes the state of the clients with the stat of the server.
 * @param players   The vector containing the player characters.
 * @param npcs      The npc container.
 * @param mobs      The mob container.
 */
void ParallelClientSynchronizer::synchronize(std::vector<std::shared_ptr<Player>> players, const EntityContainer<Npc>& npcs,
                                             const EntityContainer<Mob>& mobs)
{
    // Tick each character
    std::for_each(std::execution::par_unseq, players.begin(), players.end(),
                  [&](std::shared_ptr<Player>& player) { player->tick(); });

    // Run the synchroniser for each character, in parallel.
    std::for_each(std::execution::par_unseq, players.begin(), players.end(),
                  [&](std::shared_ptr<Player>& character) { syncCharacter(*character); });

    // Finalise the update sequence for each character.
    for (auto&& character: players)
    {
        if (!character->active())
            continue;

        // Reset the update flags
        character->resetUpdateFlags();
        character->resetMovementState();

        // Clear the temporary attributes used in updating
        character->clearAttribute(Attribute::LastChatMessage);
    }

    // Finalise the update sequence for npcs and mobs
    for (auto&& mob: mobs)
        if (mob)
            mob->resetUpdateFlags();
    for (auto&& npc: npcs)
        if (npc)
            npc->resetUpdateFlags();
}

/**
 * Synchronizes a character.
 * @param player     The character to synchronize
 */
void ParallelClientSynchronizer::syncCharacter(Player& player)
{
    if (!player.active())
        return;

    // Prepare the synchronization tasks
    MapSynchronizationTask mapTask(player);
    CharacterSynchronizationTask charsTask(player);
    NpcSynchronizationTask npcTask(player);
    MobSynchronizationTask mobTask(player);

    // The vector of entities that are currently being observed
    auto& observed = player.observedEntities();

    // Loop over the currently observed entities
    auto itr = observed.begin();
    while (itr != observed.end())
    {
        auto entity = *itr;  // The current observed entity

        // If the entity is not active, or this character can't observe them, remove them.
        if (!entity->active() || !entity->observable(player))
        {
            if (entity->type() == EntityType::Player)
                charsTask.removeCharacter(dynamic_cast<Player&>(*entity));
            else if (entity->type() == EntityType::Item)
                mapTask.removeItem(dynamic_cast<GroundItem&>(*entity));
            else if (entity->type() == EntityType::Npc)
                npcTask.removeNpc(dynamic_cast<Npc&>(*entity));
            else if (entity->type() == EntityType::Mob)
                mobTask.removeMob(dynamic_cast<Mob&>(*entity));
            itr = observed.erase(itr);
            continue;
        }

        ++itr;
    }

    // Get the neighbouring cells of the character.
    auto& pos   = player.position();
    auto& world = player.world();
    auto map    = world.maps().forId(pos.map());
    auto cells  = map->getNeighbouringCells(pos);

    // Loop through all the nearby cells
    for (auto&& cell: cells)
    {
        // Loop through the entities in each cell.
        for (auto&& entity: cell->entities())
        {
            // If the entity is not yet active, do nothing
            if (!entity->active())
                continue;

            // Skip ourselves
            if (&player == entity.get())
                continue;

            // If we can't see the other entity, skip them.
            if (!entity->observable(player))
                continue;

            // If the entity is already being observed, skip them
            auto pred = [&](const std::shared_ptr<Entity>& other) { return other.get() == entity.get(); };
            if (std::find_if(observed.begin(), observed.end(), pred) != observed.end())
                continue;

            // Add the entity to the list of observed entities
            observed.push_back(entity);

            // Inform the relevant task
            if (entity->type() == EntityType::Player)
                charsTask.addCharacter(dynamic_cast<Player&>(*entity));
            else if (entity->type() == EntityType::Item)
                mapTask.addItem(dynamic_cast<GroundItem&>(*entity));
            else if (entity->type() == EntityType::Npc)
                npcTask.addNpc(dynamic_cast<Npc&>(*entity));
            else if (entity->type() == EntityType::Mob)
                mobTask.addMob(dynamic_cast<Mob&>(*entity));
        }
    }

    // Synchronise the actively observed entities
    mapTask.sync();
    charsTask.sync();
    npcTask.sync();
    mobTask.sync();
}
