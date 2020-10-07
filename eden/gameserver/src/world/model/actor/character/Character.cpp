#include <shaiya/game/net/GameSession.hpp>

using namespace shaiya;
using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Creates a character instance from a connected session
 * @param session   The session instance.
 * @param id        The character id.
 */
Character::Character(GameSession& session, size_t id)
    : session_(session), equipment_(*this), inventory_(*this), actionBar_(*this), Actor(session.context().getGameWorld())
{
    // Set this entity type
    type_ = EntityType::Character;

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

    inventory().add(std::make_shared<Item>(40, 255));
    equipment().add(std::make_shared<Item>(6, 203), EquipmentSlot::Weapon);

    setPosition(Position(0, 1200, 78, 1200));

    // Prepare the character details
    CharacterDetails details;
    details.statpoints  = statpoints_;
    details.skillpoints = skillpoints_;

    // Write the character's position
    details.x = position().x();
    details.y = position().y();
    details.z = position().z();

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

    // Synchronise the item containers
    equipment().sync();
    inventory().sync();
}

/**
 * Sets the movement state of a character.
 * @param movementState The new movement state.
 */
void Character::setMovementState(MovementState movementState)
{
    // If the previous movement state is not jumping or performing a backflip, flag an update.
    if (movementState_ != MovementState::Jumping && movementState_ != MovementState::Backflip)
        flagUpdate(UpdateMask::MovementState);
    movementState_ = movementState;  // Update the movement state
}

/**
 * Resets the movement state of a character.
 */
void Character::resetMovementState()
{
    // If the current movement state is standing or sitting, do nothing
    if (movementState_ == MovementState::Standing || movementState_ == MovementState::Sitting)
        return;
    movementState_ = MovementState::Standing;
}

/**
 * Sets the race for this character.
 * @param race  The race for this character.
 */
void Character::setRace(ShaiyaRace race)
{
    if (race_ == race)
        return;
    race_ = race;
    flagUpdate(UpdateMask::Appearance);
}