#include <shaiya/common/net/packet/game/MapGroundItem.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/sync/task/MapSynchronizationTask.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialise the synchronization task.
 * @param character The character we're currently synchronizing.
 */
MapSynchronizationTask::MapSynchronizationTask(Character& character): character_(character)
{
}

/**
 * Synchronizes the map.
 */
void MapSynchronizationTask::sync()
{
}

/**
 * Adds an item to the current character's viewport.
 * @param groundItem The item to add.
 */
void MapSynchronizationTask::addItem(const GroundItem& groundItem)
{
    auto item = groundItem.item();
    auto& pos = groundItem.position();

    // Add the ground item to the player's viewport
    GroundItemAdded update;
    update.id     = groundItem.id();
    update.x      = pos.x();
    update.y      = pos.y();
    update.z      = pos.z();
    update.type   = item->type();
    update.typeId = item->typeId();
    character_.session().write(update);
}

/**
 * Removes an item from the current character's viewport.
 * @param item The item to remove.
 */
void MapSynchronizationTask::removeItem(const GroundItem& item)
{
    // Remove the item from the player's viewport
    GroundItemRemoved update;
    update.id = item.id();
    character_.session().write(update);
}