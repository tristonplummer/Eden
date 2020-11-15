#include <shaiya/common/util/Prng.hpp>
#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/scheduling/impl/NpcMovementTask.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

using namespace shaiya::game;

/**
 * The delay between executions, in game ticks.
 * 200 = 10s
 */
constexpr auto Delay = 200;

/**
 * The chance for a mob to move.
 */
constexpr auto MovementChance = 25;

/**
 * The range that a mob may wander from it's spawn point.
 */
constexpr auto MovementRange = 3.5f;

/**
 * Initialise this task.
 */
NpcMovementTask::NpcMovementTask(): ScheduledTask(Delay)
{
}

/**
 * Handle the execution of this task.
 */
void NpcMovementTask::execute(GameWorldService& world)
{
    auto& prng = shaiya::Prng::the();
    auto maps  = world.maps().maps();

    // Loop over the maps
    for (auto& map: maps)
    {
        for (auto&& entity: map->getLiveEntities())
        {
            if (!entity || entity->type() != EntityType::Mob)
                continue;

            auto mob = std::dynamic_pointer_cast<Mob>(entity);
            if (mob->combat().inCombat() || mob->dead())
                continue;

            auto move = prng.percentage(MovementChance);
            if (move)
                mob->setPosition(mob->spawnArea().randomPoint(MovementRange));
        }
    }
}