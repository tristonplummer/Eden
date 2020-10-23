#include <shaiya/common/net/packet/Packet.hpp>
#include <shaiya/common/net/packet/game/CharacterAppearance.hpp>
#include <shaiya/common/net/packet/game/CharacterChatMessage.hpp>
#include <shaiya/common/net/packet/game/CharacterEnteredViewport.hpp>
#include <shaiya/common/net/packet/game/CharacterLeftViewport.hpp>
#include <shaiya/common/net/packet/game/CharacterMovement.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/model/EntityType.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/item/Item.hpp>
#include <shaiya/game/sync/task/CharacterSynchronizationTask.hpp>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialise the synchronization task.
 * @param character The character we're currently synchronizing.
 */
CharacterSynchronizationTask::CharacterSynchronizationTask(Player& character): character_(character)
{
}

/**
 * Synchronizes the character.
 */
void CharacterSynchronizationTask::sync()
{
    // The list of characters that were observed by our character.
    auto& observedCharacters = character_.observedEntities();

    // Process our own update flags
    processUpdateFlags(character_);

    // Loop over the observed characters and process their flagged updates.
    for (auto&& observed: observedCharacters)
    {
        if (observed->type() == EntityType::Player)
            processUpdateFlags(dynamic_cast<Player&>(*observed));
    }
}

/**
 * Adds a character to the current character's viewport.
 * @param other The character to add.
 */
void CharacterSynchronizationTask::addCharacter(const Player& other)
{
    // The character's session
    auto& session = character_.session();

    // The other character's position
    auto& pos = other.position();

    // Inform that character that the other character has entered our viewport.
    CharacterEnteredViewport notification;
    notification.id = other.id();
    notification.x  = pos.x();
    notification.y  = pos.y();
    notification.z  = pos.z();
    session.write(notification);

    // Initialise the other character's appearance
    updateAppearance(other);
}

/**
 * Removes a character from the current character's viewport.
 * @param other The character to remove.
 */
void CharacterSynchronizationTask::removeCharacter(const Player& other)
{
    // Inform the character that the other character has left our viewport.
    CharacterLeftViewport notification;
    notification.id = other.id();
    character_.session().write(notification);
}

/**
 * Process the update flags for a character.
 * @param other The character to update for this character.
 */
void CharacterSynchronizationTask::processUpdateFlags(const Player& other)
{
    // Update appearance
    if (other.hasUpdateFlag(UpdateFlag::Appearance))
        updateAppearance(other);

    // Update the movement state (standing, sitting, jumping...)
    if (other.hasUpdateFlag(UpdateFlag::MovementState))
        updateMovementState(other);

    // Update normal chat
    if (other.hasUpdateFlag(UpdateFlag::Chat))
        updateChat(other);

    // Update movement for other characters (no reason to update for the current character).
    if (other.hasUpdateFlag(UpdateFlag::Movement) && other.id() != character_.id())
        updateMovement(other);
}

/**
 * Update the appearance of a character, for the current character.
 * @param other The character to update.
 */
void CharacterSynchronizationTask::updateAppearance(const Player& other)
{
    // The appearance of the character
    auto& app = other.appearance();

    // Construct the character appearance packet
    CharacterAppearance appearance;
    appearance.id      = other.id();
    appearance.faction = other.faction();
    appearance.state   = other.movementState();
    appearance.job     = other.job();
    appearance.name    = other.name();
    appearance.race    = other.race();
    appearance.face    = app.face();
    appearance.hair    = app.hair();
    appearance.height  = app.height();
    appearance.gender  = app.gender();

    // The equipment of the character
    auto equipment = other.equipment().visibleItems();
    for (auto i = 0; i < appearance.equipment.size(); i++)
    {
        auto& item = equipment.at(i);
        auto& slot = appearance.equipment.at(i);
        if (!item)  // If no item exists at this slot, skip it.
            continue;
        slot.type   = item->type();
        slot.typeId = item->typeId();
    }
    character_.session().write(appearance);
}

/**
 * Updates the movement of a character, for the current character.
 * @param other The character to update.
 */
void CharacterSynchronizationTask::updateMovement(const Player& other)
{
    // The character's position
    auto& pos = other.position();

    // Construct the movement packet
    CharacterMovementUpdate update;
    update.id        = other.id();
    update.motion    = other.motion();
    update.direction = other.direction();
    update.x         = pos.x();
    update.y         = pos.y();
    update.z         = pos.z();
    character_.session().write(update);
}

/**
 * Updates the movement state of a character, for the current character.
 * @param other The character to update.
 */
void CharacterSynchronizationTask::updateMovementState(const Player& other)
{
    // Construct the movement state notification
    MovementStateNotification update;
    update.id    = other.id();
    update.state = other.movementState();
    character_.session().write(update);
}

/**
 * Updates the chat of a character, for the other character.
 * @param other The character to update.
 */
void CharacterSynchronizationTask::updateChat(const Player& other)
{
    auto chatMessage = other.getAttribute<std::string>(Attribute::LastChatMessage);

    // Send the update
    CharacterChatMessageUpdate update;
    update.sender  = other.id();
    update.length  = chatMessage.length();
    update.message = chatMessage;
    character_.session().write(update);
}