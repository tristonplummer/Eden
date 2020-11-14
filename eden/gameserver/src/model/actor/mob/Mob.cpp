#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/ai/mob/MobResetEvade.hpp>
#include <shaiya/game/model/ai/mob/MobSelectNearestTarget.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

using namespace shaiya::game;

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

    // Initialise the movement speed
    MovementSpeed speed{};
    speed.walkingInterval = def.walkingInterval;
    speed.walkingSteps    = def.walkingStep;
    speed.runningInterval = def.runningInterval;
    speed.runningSteps    = def.runningStep;
    movement().setMovementSpeed(speed);

    // Initialise the size
    size_ = def.size;

    // Initialise the level
    level_ = def.level;

    // Set the current and max health of the mob
    stats().setBase(Stat::MaxHealth, def.hitpoints);
    stats().sync();
    stats().setHitpoints(def.hitpoints);
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

    // Perform any evading and resetting mob to spawn point if the mob gets too far
    ai::MobResetEvade evade(*this);
    evade.execute();

    // If the mob is not in combat, search for a target
    if (!combat().inCombat() && !hasAttribute(Attribute::Evading))
    {
        // Select the closest player target
        ai::MobSelectNearestTarget targeting(*this);
        auto target = targeting.select();

        // Engage the target in combat
        if (target)
        {
            combat().attack(target);
            movement().follow(target);
        }
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