#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/item/container/event/EquipmentEventListener.hpp>
#include <shaiya/game/world/model/item/container/event/InventoryEventListener.hpp>

using namespace shaiya;
using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Creates a character instance from a connected session
 * @param session   The session instance.
 * @param id        The character id.
 */
Character::Character(GameSession& session, size_t id)
    : session_(session), actionBar_(*this), appearance_(*this), Actor(session.context().getGameWorld())
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

    // Add event listeners
    inventory_.addListener(std::make_shared<InventoryEventListener>(*this));
    equipment_.addListener(std::make_shared<EquipmentEventListener>(*this));
    stats().onSync([&](const StatSet& stats, StatUpdateType type) { onStatSync(stats, type); });

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

    // Cheat way to send our current health after loading the character
    onStatSync(stats_, StatUpdateType::Status);
}

/**
 * Gets executed when the stats for this character are synchronized.
 * @param stats     The stats for this character.
 * @param type      The update type.
 */
void Character::onStatSync(const StatSet& stats, StatUpdateType type)
{
    // A status update is just an update about our current health
    if (type == StatUpdateType::Status)
    {
        CharacterCurrentHitpoints update;
        update.hitpoints = stats.currentHitpoints();
        update.mana      = stats.currentMana();
        update.stamina   = stats.currentStamina();
        session_.write(update);
        return;
    }

    // Update the max health, mana and stamina
    auto updateMaxHealth = [&](MaxHitpointType type, uint32_t value) {
        CharacterMaxHitpoints update;
        update.id    = id();
        update.type  = type;
        update.value = value;
        session_.write(update);
    };
    updateMaxHealth(MaxHitpointType::Hitpoints, stats.maxHitpoints());
    updateMaxHealth(MaxHitpointType::Mana, stats.maxMana());
    updateMaxHealth(MaxHitpointType::Stamina, stats.maxStamina());
}

/**
 * Sets the movement state of a character.
 * @param movementState The new movement state.
 */
void Character::setMovementState(MovementState movementState)
{
    // If the previous movement state is not jumping or performing a backflip, flag an update.
    if (movementState_ != MovementState::Jumping && movementState_ != MovementState::Backflip)
        flagUpdate(UpdateFlag::MovementState);
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
    flagUpdate(UpdateFlag::Appearance);
}