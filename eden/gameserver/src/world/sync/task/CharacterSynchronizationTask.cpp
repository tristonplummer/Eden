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

                // If we can't see the other character, skip them.
                if (!observable(*other))
                    continue;

                // Add the character
                addCharacter(*other);
                observedCharacters.push_back(other);
            }
        }
    }
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
 * Update the appearance of a character, for the current character.
 * @param other The character to update.
 */
void CharacterSynchronizationTask::updateAppearance(const Character& other)
{
    // Construct the character appearance packet
    CharacterAppearance appearance;
    appearance.id      = other.id();
    appearance.faction = other.faction();
    appearance.name    = "Cups";
    character_.session().write(appearance);
}

/**
 * Checks if a character is observable to the current character.
 * @param other The other character.
 * @return      If the other character can be observed.
 */
bool CharacterSynchronizationTask::observable(const Character& other)
{
    auto& charPos  = character_.position();
    auto& otherPos = other.position();
    return charPos.isWithinViewportDistance(otherPos);
}