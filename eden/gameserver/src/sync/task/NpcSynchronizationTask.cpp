#include <shaiya/common/net/packet/Packet.hpp>
#include <shaiya/common/net/packet/game/NpcEnteredViewport.hpp>
#include <shaiya/common/net/packet/game/NpcLeftViewport.hpp>
#include <shaiya/common/net/packet/game/NpcMovement.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/model/actor/npc/Npc.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/sync/task/NpcSynchronizationTask.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialise the synchronization task.
 * @param character The character we're currently synchronizing.
 */
NpcSynchronizationTask::NpcSynchronizationTask(Player& character): character_(character)
{
}

/**
 * Synchronizes the character.
 */
void NpcSynchronizationTask::sync()
{
    // The list of entities that were observed by our character.
    auto& observedEntities = character_.observedEntities();

    // Loop over the observed entities and process their flagged updates.
    for (auto&& observed: observedEntities)
    {
        if (observed->type() == EntityType::Npc)
            processUpdateFlags(dynamic_cast<Npc&>(*observed));
    }
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
    update.id        = other.id();
    update.type      = def.type;
    update.typeId    = def.typeId;
    update.direction = other.direction();
    update.x         = pos.x();
    update.y         = pos.y();
    update.z         = pos.z();
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

/**
 * Process the update flags for an NPC.
 * @param other The NPC to update for this character.
 */
void NpcSynchronizationTask::processUpdateFlags(const Npc& other)
{
    // Update movement
    if (other.hasUpdateFlag(UpdateFlag::Movement))
        updateMovement(other);
}

/**
 * Updates the movement of an NPC, for the current character.
 * @param other The NPC to update.
 */
void NpcSynchronizationTask::updateMovement(const Npc& other)
{
    auto& pos = other.position();

    NpcMovement movement;
    movement.id = other.id();
    movement.x  = pos.x();
    movement.y  = pos.y();
    movement.z  = pos.z();
    character_.session().write(movement);
}