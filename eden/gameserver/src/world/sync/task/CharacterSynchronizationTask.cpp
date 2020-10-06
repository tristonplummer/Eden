#include <shaiya/common/net/packet/Packet.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/sync/task/CharacterSynchronizationTask.hpp>

#include <thread>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Initialise the synchronization task.
 * @param character The character we're currently synchronizing.
 */
CharacterSynchronizationTask::CharacterSynchronizationTask(Character& character): character_(character)
{
}

/**
 * Synchronizes the character.
 */
void CharacterSynchronizationTask::sync()
{
    // The list of characters that were observed by our character.
    auto& observedCharacters = character_.observedCharacters();

    // Iterate over the observed characters
    auto charIter = observedCharacters.begin();
    while (charIter != observedCharacters.end())
    {
        // The other character
        auto& other = *(*charIter);

        // Remove the observed character if we can't see them
        if (!observable(other))
        {
            // Remove the other character from our viewport
            removeCharacter(other);

            // Remove the character from the vector of observable character
            charIter = observedCharacters.erase(charIter);
            continue;
        }
        // Advance the iterator
        ++charIter;
    }

    // Get the neighbouring cells of our character.
    auto& pos   = character_.position();
    auto& world = character_.world();
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

            // If we can't see the other entity, skip them.
            if (!observable(*entity))
                continue;

            // If the entity is another character, and they are observable, add them
            if (entity->type() == EntityType::Character)
            {
                // The other character
                auto* other = dynamic_cast<Character*>(entity.get());

                // If the other character is us, skip them
                if (character_.id() == other->id())
                    continue;

                // If the other character is already in our vector of observed characters
                if (std::find(observedCharacters.begin(), observedCharacters.end(), other) != observedCharacters.end())
                    continue;

                // Add the character
                addCharacter(*other);
                observedCharacters.push_back(other);
            }
        }
    }

    // Process our own update flags
    processUpdateFlags(character_);

    // Loop over the observed characters and process their flagged updates.
    for (auto&& observed: observedCharacters)
        processUpdateFlags(*observed);
}

/**
 * Adds a character to the current character's viewport.
 * @param other The character to add.
 */
void CharacterSynchronizationTask::addCharacter(const Character& other)
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
void CharacterSynchronizationTask::removeCharacter(const Character& other)
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
void CharacterSynchronizationTask::processUpdateFlags(const Character& other)
{
    // Update appearance
    if (other.hasUpdateFlag(UpdateMask::Appearance))
        updateAppearance(other);

    // Update the movement state (standing, sitting, jumping...)
    if (other.hasUpdateFlag(UpdateMask::MovementState))
        updateMovementState(other);

    // Update normal chat
    if (other.hasUpdateFlag(UpdateMask::Chat))
        updateChat(other);

    // Update movement for other characters (no reason to update for the current character).
    if (other.hasUpdateFlag(UpdateMask::Movement) && other.id() != character_.id())
        updateMovement(other);
}

/**
 * Update the appearance of a character, for the current character.
 * @param other The character to update.
 */
void CharacterSynchronizationTask::updateAppearance(const Character& other)
{
    // Construct the character appearance packet
    CharacterAppearance appearance;
    appearance.id      = other.id();
    appearance.faction = other.faction();
    appearance.state   = other.movementState();
    appearance.name    = "Cups";

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
void CharacterSynchronizationTask::updateMovement(const Character& other)
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
void CharacterSynchronizationTask::updateMovementState(const Character& other)
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
void CharacterSynchronizationTask::updateChat(const Character& other)
{
    auto chatMessage = other.getAttribute<std::string>(Attribute::LastChatMessage);

    // Send the update
    CharacterChatMessageUpdate update;
    update.sender  = other.id();
    update.length  = chatMessage.length();
    update.message = chatMessage;
    character_.session().write(update);
}

/**
 * Checks if an entity can be observed by the current character.
 * @param other The other character.
 * @return      If the other entity can be observed.
 */
bool CharacterSynchronizationTask::observable(const Entity& other)
{
    auto& charPos  = character_.position();
    auto& otherPos = other.position();
    return charPos.isWithinViewportDistance(otherPos);
}