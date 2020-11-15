#include <shaiya/game/model/actor/Actor.hpp>
#include <shaiya/game/model/item/Item.hpp>
#include <shaiya/game/scheduling/impl/ActorDeathTask.hpp>
#include <shaiya/game/scheduling/impl/HealthNormalizationTask.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

using namespace shaiya::game;

/**
 * Initialises this actor.
 * @param world The world instance.
 */
Actor::Actor(GameWorldService& world): Entity(world), combat_(*this), movement_(*this)
{
    attackSpeed_ = AttackSpeed::Normal;
}

/**
 * Initialises this actor.
 */
void Actor::init()
{
    // Initialise the base entity
    Entity::init();

    // Synchronise our stats
    syncStats();

    // Add listeners
    stats().onSync([&](const StatSet& stats, StatUpdateType type) { onHitpointsStatSync(stats, type); });
}

/**
 * Activates this actor.
 */
void Actor::activate()
{
    Entity::activate();

    setDead(false);
    combat().reset();
    movement().reset();

    // Schedule a health normalization task.
    // What do we do in the event that an actor is deactivated and re-activated before this task executes? Perhaps we need
    // to separate the concept of "active" and "loaded into the map".
    world().schedule(std::make_shared<HealthNormalizationTask>(*this));
}

/**
 * Processes the tick for this entity.
 */
void Actor::tick()
{
    Entity::tick();

    if (!dead())
    {
        combat().tick();
        if (type() != EntityType::Player)
            movement().tick();
    }
}

/**
 * Synchronises this actor's stats, by calculating the bonuses from their
 * active buffs and worn equipment.
 */
void Actor::syncStats()
{
    // The additional stats
    auto strength     = 0;
    auto dexterity    = 0;
    auto reaction     = 0;
    auto intelligence = 0;
    auto wisdom       = 0;
    auto luck         = 0;
    auto hitpoints    = 0;
    auto stamina      = 0;
    auto mana         = 0;
    auto minAttack    = 0;
    auto maxAttack    = 0;

    // Iterate through the actor's equipment
    for (auto&& item: equipment_.items())
    {
        if (!item)
            continue;
        auto& def = item->definition();

        // Add the values for the item definition
        strength += def.strength;
        dexterity += def.dexterity;
        reaction += def.reaction;
        intelligence += def.intelligence;
        wisdom += def.wisdom;
        luck += def.luck;
        hitpoints += def.hitpoints;
        stamina += def.stamina;
        mana += def.mana;
        minAttack += def.minAttack;
        maxAttack += def.minAttack + def.attackBonus;
    }

    // Set the additional values
    stats_.setAdditional(Stat::Strength, strength);
    stats_.setAdditional(Stat::Dexterity, dexterity);
    stats_.setAdditional(Stat::Reaction, reaction);
    stats_.setAdditional(Stat::Intelligence, intelligence);
    stats_.setAdditional(Stat::Wisdom, wisdom);
    stats_.setAdditional(Stat::Luck, luck);
    stats_.setAdditional(Stat::MaxHealth, hitpoints);
    stats_.setAdditional(Stat::MaxStamina, stamina);
    stats_.setAdditional(Stat::MaxMana, mana);
    stats_.setAdditional(Stat::MinPhysicalAttack, minAttack);
    stats_.setAdditional(Stat::MaxPhysicalAttack, maxAttack);
    stats_.setAdditional(Stat::MinRangedAttack, minAttack);
    stats_.setAdditional(Stat::MaxRangedAttack, maxAttack);
    stats_.setAdditional(Stat::MinMagicalAttack, minAttack);
    stats_.setAdditional(Stat::MaxMagicalAttack, maxAttack);

    // Explicitly synchronise the stats
    stats_.sync();
}

/**
 * Gets executed when the stats for this character are synchronized.
 * @param stats     The stats for this character.
 */
void Actor::onHitpointsStatSync(const StatSet& stats, StatUpdateType type)
{
    if (stats.currentHitpoints() == 0)
    {
        setDead(true);
        world().schedule(std::make_shared<ActorDeathTask>(*this));
    }
}

/**
 * Sets the position of this entity.
 * @param position  The position.
 */
void Actor::setPosition(Position position)
{
    Entity::setPosition(position);
}

/**
 * Sets the name of this actor.
 * @param name  The name value.
 */
void Actor::setName(const std::string& name)
{
    if (name_ == name)
        return;
    name_ = name;
    flagUpdate(UpdateFlag::Appearance);
}

/**
 * Sets the class of the actor.
 * @param job   The class value.
 */
void Actor::setJob(shaiya::ShaiyaClass job)
{
    if (class_ == job)
        return;
    class_ = job;
    flagUpdate(UpdateFlag::Appearance);
}

/**
 * Sets the level of the actor.
 * @param level The level value.
 */
void Actor::setLevel(uint16_t level)
{
    level_ = level;
}

/**
 * Sets the death state of this actor.
 * @param dead  The death state.
 */
void Actor::setDead(bool dead)
{
    dead_ = dead;
}