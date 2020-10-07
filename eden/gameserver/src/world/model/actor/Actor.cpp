#include <shaiya/game/world/model/actor/Actor.hpp>

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