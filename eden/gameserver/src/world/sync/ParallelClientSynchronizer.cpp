#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>
#include <shaiya/game/world/model/actor/npc/Npc.hpp>
#include <shaiya/game/world/model/item/GroundItem.hpp>
#include <shaiya/game/world/model/map/Map.hpp>
#include <shaiya/game/world/model/map/MapCell.hpp>
#include <shaiya/game/world/sync/ParallelClientSynchronizer.hpp>
#include <shaiya/game/world/sync/task/CharacterSynchronizationTask.hpp>
#include <shaiya/game/world/sync/task/MapSynchronizationTask.hpp>
#include <shaiya/game/world/sync/task/NpcSynchronizationTask.hpp>

#include <execution>

using namespace shaiya::game;

/**
 * Synchronizes the state of the clients with the stat of the server.
 * @param characters    The vector containing the player characters.
 */
void ParallelClientSynchronizer::synchronize(std::vector<std::shared_ptr<Player>> characters)
{
    // Run the synchroniser for each character, in parallel.
    std::for_each(std::execution::par_unseq, characters.begin(), characters.end(),
                  [&](std::shared_ptr<Player>& character) { syncCharacter(*character); });

    // Finalise the update sequence for each character.
    for (auto&& character: characters)
    {
        if (!character->active())
            continue;

        // Reset the update flags
        character->resetUpdateFlags();
        character->resetMovementState();

        // Clear the temporary attributes used in updating
        character->clearAttribute(Attribute::LastChatMessage);
    }
}

/**
 * Synchronizes a character.
 * @param character     The character to synchronize
 */
void ParallelClientSynchronizer::syncCharacter(Player& character)
{
    if (!character.active())
        return;

    // Prepare the synchronization tasks
    MapSynchronizationTask mapTask(character);
    CharacterSynchronizationTask charsTask(character);
    NpcSynchronizationTask npcTask(character);

    // The vector of entities that are currently being observed
    auto& observed = character.observedEntities();

    // Loop over the currently observed entities
    auto itr = observed.begin();
    while (itr != observed.end())
    {
        auto entity = *itr;  // The current observed entity

        // If the entity is not active, or this character can't observe them, remove them.
        if (!entity->active() || !entity->observable(character))
        {
            if (entity->type() == EntityType::Character)
                charsTask.removeCharacter(dynamic_cast<Player&>(*entity));
            else if (entity->type() == EntityType::Item)
                mapTask.removeItem(dynamic_cast<GroundItem&>(*entity));
            else if (entity->type() == EntityType::Npc)
                npcTask.removeNpc(dynamic_cast<Npc&>(*entity));
            itr = observed.erase(itr);
            continue;
        }

        ++itr;
    }

    // Get the neighbouring cells of the character.
    auto& pos   = character.position();
    auto& world = character.world();
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
            if (&character == entity.get())
                continue;

            // If we can't see the other entity, skip them.
            if (!entity->observable(character))
                continue;

            // If the entity is already being observed, skip them
            auto pred = [&](const std::shared_ptr<Entity>& other) { return other.get() == entity.get(); };
            if (std::find_if(observed.begin(), observed.end(), pred) != observed.end())
                continue;

            // Add the entity to the list of observed entities
            observed.push_back(entity);

            // Inform the relevant task
            if (entity->type() == EntityType::Character)
                charsTask.addCharacter(dynamic_cast<Player&>(*entity));
            else if (entity->type() == EntityType::Item)
                mapTask.addItem(dynamic_cast<GroundItem&>(*entity));
            else if (entity->type() == EntityType::Npc)
                npcTask.addNpc(dynamic_cast<Npc&>(*entity));
        }
    }

    // Synchronise the actively observed entities
    mapTask.sync();
    charsTask.sync();
    npcTask.sync();
}
