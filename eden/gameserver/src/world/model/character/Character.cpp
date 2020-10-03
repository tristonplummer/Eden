#include <shaiya/game/net/GameSession.hpp>

using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Creates a character instance from a connected session
 * @param session   The session instance.
 * @param id        The character id.
 */
Character::Character(shaiya::net::GameSession& session, size_t id): session_(session)
{
    // Set the character id and faction
    id_      = id;
    faction_ = session.faction();
}

/**
 * Sends the startup packets for this character.
 */
void Character::sendStartup()
{
    // Send the character details.
    CharacterDetails details;
    session_.write(details);
}