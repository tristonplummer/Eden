#include <shaiya/common/net/packet/Packet.hpp>
#include <shaiya/common/net/packet/game/MobEnteredViewport.hpp>
#include <shaiya/common/net/packet/game/MobLeftViewport.hpp>
#include <shaiya/common/net/packet/game/MobMovement.hpp>
#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/sync/task/MobSynchronizationTask.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialise the synchronization task.
 * @param character The character we're currently synchronizing.
 */
MobSynchronizationTask::MobSynchronizationTask(Player& character): character_(character)
{
}

/**
 * Synchronizes the character.
 */
void MobSynchronizationTask::sync()
{
    // The list of entities that were observed by our character.
    auto& observedEntities = character_.observedEntities();

    // Loop over the observed entities and process their flagged updates.
    for (auto&& observed: observedEntities)
    {
        if (observed->type() == EntityType::Mob)
            processUpdateFlags(dynamic_cast<Mob&>(*observed));
    }
}

/**
 * Adds a mob to the current character's viewport.
 * @param other The mob to add.
 */
void MobSynchronizationTask::addMob(const Mob& other)
{
    auto& pos = other.position();

    MobEnteredViewport viewport;
    viewport.id    = other.id();
    viewport.mobId = other.definition().id;
    viewport.x     = pos.x();
    viewport.z     = pos.z();
    character_.session().write(viewport);
}

/**
 * Removes a mob from the current character's viewport.
 * @param other The mob to remove.
 */
void MobSynchronizationTask::removeMob(const Mob& other)
{
    character_.session().write(MobLeftViewport{ .id = static_cast<uint32_t>(other.id()) });
}

/**
 * Process the update flags for a mob.
 * @param other The mob to update for this character.
 */
void MobSynchronizationTask::processUpdateFlags(const Mob& other)
{
    // Update movement
    if (other.hasUpdateFlag(UpdateFlag::Movement))
        updateMovement(other);
}

/**
 * Updates the movement of a mob, for the current character.
 * @param other The mob to update.
 */
void MobSynchronizationTask::updateMovement(const Mob& other)
{
    auto& pos = other.position();

    MobMovement movement;
    movement.id      = other.id();
    movement.running = other.movement().running();
    movement.x       = pos.x();
    movement.z       = pos.z();
    character_.session().write(movement);
}