#include <shaiya/common/net/packet/Packet.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/sync/task/NpcSynchronizationTask.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialise the synchronization task.
 * @param character The character we're currently synchronizing.
 */
NpcSynchronizationTask::NpcSynchronizationTask(Character& character): character_(character)
{
}

/**
 * Synchronizes the character.
 */
void NpcSynchronizationTask::sync()
{
}

/**
 * Adds an NPC to the current character's viewport.
 * @param other The npc to add.
 */
void NpcSynchronizationTask::addNpc(const Npc& other)
{
    auto& def = other.definition();
    auto& pos = other.position();

    NpcEnteredViewport update;
    update.id     = other.id();
    update.type   = def.type;
    update.typeId = def.typeId;
    update.x      = pos.x();
    update.y      = pos.y();
    update.z      = pos.z();
    character_.session().write(update);
}

/**
 * Removes an NPC from the current character's viewport.
 * @param other The npc to remove.
 */
void NpcSynchronizationTask::removeNpc(const Npc& other)
{
    NpcLeftViewport update;
    update.id = other.id();
    character_.session().write(update);
}