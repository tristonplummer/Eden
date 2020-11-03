#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/ai/mob/MobSelectNearestTarget.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

using namespace shaiya::game;

/**
 * The distance from the spawn point that a mob should reset.
 */
constexpr auto MobResetDistance = 30.0f;

/**
 * Initialises a mob with a specified definition.
 * @param def       The mob definition.
 * @param spawnArea The area that this mob can spawn in.
 * @param world     The game world instance.
 */
Mob::Mob(const client::MobDefinition& def, Area spawnArea, GameWorldService& world)
    : Actor(world), def_(def), spawnArea_(spawnArea)
{
    type_ = EntityType::Mob;
}

/**
 * Activates this actor.
 */
void Mob::activate()
{
    Actor::activate();
}

/**
 * Processes the tick for this entity.
 */
void Mob::tick()
{
    Actor::tick();

    // Clear evasion flag
    if (spawnArea().contains(position()))
    {
        clearAttribute(Attribute::Evading);
    }

    // If the mob is too far from it's spawn point, force it to reset back
    if (spawnArea().distanceTo(position()) > MobResetDistance && !hasAttribute(Attribute::Evading))
    {
        setAttribute(Attribute::Evading);
        combat().reset();
        movement().moveTo(spawnArea().randomPoint());
    }

    // If the mob is not in combat, search for a target
    if (!combat().inCombat() && !hasAttribute(Attribute::Evading))
    {
        // Select the closest player target
        ai::MobSelectNearestTarget targeting(*this);
        auto target = targeting.select();

        // Engage the target in combat
        if (target)
            combat().attack(target);
    }
}

/**
 * Sets the position of this mob.
 * @param position  The new position.
 */
void Mob::setPosition(Position position)
{
    auto map = world().maps().forId(position.map());

    if (map)
    {
        float y = map->heightmap().y(position.x(), position.z());
        if (y > position.y())
            position = Position(position.map(), position.x(), y, position.z());
    }

    Actor::setPosition(position);
}