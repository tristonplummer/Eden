#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/world/model/actor/Actor.hpp>
#include <shaiya/game/world/scheduling/impl/HealthNormalizationTask.hpp>

using namespace shaiya::game;

/**
 * Initialises this actor.
 * @param world The world instance.
 */
Actor::Actor(GameWorldService& world): Entity(world)
{
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
}

/**
 * Activates this actor.
 */
void Actor::activate()
{
    Entity::activate();

    // Schedule a health normalization task.
    // What do we do in the event that an actor is deactivated and re-activated before this task executes? Perhaps we need
    // to separate the concept of "active" and "loaded into the map".
    world().schedule(std::make_shared<HealthNormalizationTask>(*this));
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
    }

    // Set the additional values
    stats_.setAdditional(Stat::Strength, strength);
    stats_.setAdditional(Stat::Dexterity, dexterity);
    stats_.setAdditional(Stat::Reaction, reaction);
    stats_.setAdditional(Stat::Intelligence, intelligence);
    stats_.setAdditional(Stat::Wisdom, wisdom);
    stats_.setAdditional(Stat::Luck, luck);

    // Explicitly synchronise the stats
    stats_.sync();
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