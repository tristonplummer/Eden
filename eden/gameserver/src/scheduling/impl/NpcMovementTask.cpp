#include <shaiya/common/util/Prng.hpp>
#include <shaiya/game/model/actor/mob/Mob.hpp>
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
    auto& mobs = world.mobs();

    for (auto&& mob: mobs)
    {
        if (!mob)
            continue;

        auto move = prng.percentage(MovementChance);  //(rand() % 100) < MovementChance;
        if (move)
            mob->setPosition(mob->spawnArea().randomPoint(MovementRange));
    }
}