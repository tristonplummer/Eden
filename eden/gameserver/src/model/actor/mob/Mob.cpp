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
 * @param respawns  If this mob should respawn on death
 */
Mob::Mob(const MobDefinition& def, Area spawnArea, GameWorldService& world, bool respawns)
    : Actor(world), def_(def), spawnArea_(spawnArea), respawns_(respawns)
{
    type_ = EntityType::Mob;

    // Initialise the size
    size_ = def.size;

    // Initialise the level
    level_ = def.level;
    name_  = def.name;

    // Set the current and max health of the mob
    stats().setBase(Stat::MaxHealth, def_.hitpoints);
    stats().setHitpoints(def_.hitpoints);
    stats().sync();
}

/**
 * Activates this actor.
 */
void Mob::activate()
{
    Actor::activate();

    // Set the current and max health of the mob
    stats().setBase(Stat::MaxHealth, def_.hitpoints);
    stats().setHitpoints(def_.hitpoints);
    stats().sync();
}

/**
 * Processes the tick for this entity.
 */
void Mob::tick()
{
    Actor::tick();

    // If the mob is dead, don't continue
    if (dead())
        return;

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