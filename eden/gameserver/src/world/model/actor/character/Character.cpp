#include <shaiya/common/net/packet/game/CharacterAdditionalStats.hpp>
#include <shaiya/common/net/packet/game/CharacterCurrentHitpoints.hpp>
#include <shaiya/common/net/packet/game/CharacterDetails.hpp>
#include <shaiya/common/net/packet/game/CharacterMaxHitpoints.hpp>
#include <shaiya/common/net/packet/game/WorldTime.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/ServiceContext.hpp>
#include <shaiya/game/world/model/actor/character/Character.hpp>
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

    // Write the current time
    WorldTime wTime{};
    auto time     = wTime.time.decode();
    auto currTime = std::mktime(&time);
    LOG(INFO) << std::ctime(&currTime);
    session_.write(wTime);
    inventory_.setGold(250000000);

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

    // Write some miscellaneous data about the character.
    details.gold = inventory_.gold();
    session_.write(details);  // Send the character details.

    // Synchronise the item containers
    equipment().sync();
    inventory().sync();

    // Cheat way to send our current health and additional stats after loading the character
    onStatSync(stats_, StatUpdateType::Full);
}

/**
 * Marks this character as active.
 */
void Character::activate()
{
    Actor::activate();
}

/**
 * Gets executed when the stats for this character are synchronized.
 * @param stats     The stats for this character.
 * @param type      The update type.
 */
void Character::onStatSync(const StatSet& stats, StatUpdateType type)
{
    // A status update is just an update about our current health
    CharacterCurrentHitpoints status;
    status.hitpoints = stats.currentHitpoints();
    status.mana      = stats.currentMana();
    status.stamina   = stats.currentStamina();
    session_.write(status);

    // If it's just a status update, we can stop here
    if (type == StatUpdateType::Status)
        return;

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

    // If this character is an Archer
    auto isArcher = job() == ShaiyaClass::Archer;

    // Update the additional stats
    CharacterAdditionalStats update;
    update.strength       = stats.getAdditional(Stat::Strength);
    update.dexterity      = stats.getAdditional(Stat::Dexterity);
    update.reaction       = stats.getAdditional(Stat::Reaction);
    update.intelligence   = stats.getAdditional(Stat::Intelligence);
    update.wisdom         = stats.getAdditional(Stat::Wisdom);
    update.luck           = stats.getAdditional(Stat::Luck);
    update.minAttack      = isArcher ? stats.getTotal(Stat::MinRangedAttack) : stats.getTotal(Stat::MinPhysicalAttack);
    update.maxAttack      = isArcher ? stats.getTotal(Stat::MaxRangedAttack) : stats.getTotal(Stat::MaxPhysicalAttack);
    update.minMagicAttack = stats.getTotal(Stat::MinMagicalAttack);
    update.maxMagicAttack = stats.getTotal(Stat::MaxMagicalAttack);
    update.defense        = stats.getTotal(Stat::Defense);
    update.resistance     = stats.getTotal(Stat::Resistance);
    session_.write(update);
}

/**
 * Sets the amount of statpoints for this character.
 * @param statpoints    The character's statpoints.
 */
void Character::setStatpoints(size_t statpoints)
{
    statpoints_ = statpoints;
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