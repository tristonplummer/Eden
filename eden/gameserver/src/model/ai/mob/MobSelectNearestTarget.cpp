#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/ai/mob/MobSelectNearestTarget.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/model/map/MapCell.hpp>

using namespace shaiya::game;
using namespace shaiya::game::ai;

/**
 * Initialises the AI.
 * @param mob   The mob to operate on.
 */
MobSelectNearestTarget::MobSelectNearestTarget(Mob& mob): mob_(mob)
{
}

/**
 * Selects the closest target.
 * @return  The target.
 */
std::shared_ptr<Actor> MobSelectNearestTarget::select()
{
    // If the mob is in an evasion state, don't even bother
    if (mob_.hasAttribute(Attribute::Evading))
        return nullptr;

    // The vector of valid targets
    std::vector<std::shared_ptr<Player>> targets;

    // The position and map of the mob
    auto& pos = mob_.position();
    auto map  = mob_.map();

    auto& cells = map->getNeighbouringCells(pos);  // The cells to search
    for (auto&& cell: cells)
    {
        for (auto&& player: cell->players())
        {
            if (!mob_.combat().canAttack(player))  // If we can't attack the player, skip them entirely
                continue;

            auto aggroRange = player->size() + mob_.size() + mob_.definition().aggressionRadius;
            if (player->position().isWithinDistance(pos, aggroRange))
                targets.push_back(player);
        }
    }

    // Sort the vector by level first (low levels should be attacked first), and then by distance
    std::sort(targets.begin(), targets.end(),
              [&](const std::shared_ptr<Player>& first, const std::shared_ptr<Player>& second) {
                  if (first->level() != second->level())
                      return first->level() < second->level();
                  return first->position().getDistance(pos) < second->position().getDistance(pos);
              });

    // Return the targetable player
    return targets.empty() ? nullptr : targets.front();
}