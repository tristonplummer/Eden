#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/scheduling/impl/MobAggressionTask.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

using namespace shaiya::game;

/**
 * The delay between executions, in game ticks.
 */
constexpr auto Delay = 2;

/**
 * Initialise this task.
 * @param mob   The mob to handle the aggression for.
 */
MobAggressionTask::MobAggressionTask(Mob& mob): ScheduledTask(Delay), mob_(mob)
{
}

/**
 * Handle the execution of this task.
 */
void MobAggressionTask::execute(GameWorldService& world)
{
    if (!mob_.active())
        return;

    auto& pos = mob_.position();
    auto map  = mob_.map();
    if (map == nullptr)
        return;

    auto entities = map->getNeighbouringEntities(pos, EntityType::Player);
    if (entities.empty())
        return;

    // Sort the entities in order of distance from the mob
    auto pred = [&](const std::shared_ptr<Entity>& first, const std::shared_ptr<Entity>& second) {
        return first->position().getDistance(pos) < second->position().getDistance(pos);
    };
    std::sort(entities.begin(), entities.end(), pred);

    // Attack the closest actor
    auto closest = std::dynamic_pointer_cast<Actor>(entities.front());
    mob_.combat().attack(closest);
}