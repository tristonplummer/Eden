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
 * Initialises this character.
 */
void Character::init()
{
    // Initialise the base actor
    Actor::init();

    // Prepare the character details
    CharacterDetails details;
    details.statpoints = statpoints_;
    details.skillpoints = skillpoints_;

    // Write the character's position
    details.x = position_.x();
    details.y = position_.y();
    details.z = position_.z();

    // Write the maximum health, mana and stamina
    details.maxHitpoints = stats_.maxHitpoints();
    details.maxMana      = stats_.maxMana();
    details.maxStamina   = stats_.maxStamina();

    // Write the character's kills
    details.kills     = kills_;
    details.deaths    = deaths_;
    details.victories = victories_;
    details.defeats   = defeats_;
    session_.write(details);  // Send the character details.
}