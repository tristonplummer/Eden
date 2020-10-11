#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/world/sync/task/MapSynchronizationTask.hpp>

using namespace shaiya::game;

/**
 * Initialise the synchronization task.
 * @param character The character we're currently synchronizing.
 */
MapSynchronizationTask::MapSynchronizationTask(Character& character): character_(character)
{
}

/**
 * Synchronizes the character.
 */
void MapSynchronizationTask::sync()
{
    auto& world = character_.world();
    auto& pos   = character_.position();

    auto map      = world.maps().forId(pos.map());   // The map that the player is currently on.
    auto viewport = map->getNeighbouringCells(pos);  // The cells in the player's current viewport.
}