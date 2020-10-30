#include <shaiya/game/model/actor/mob/Mob.hpp>

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
}