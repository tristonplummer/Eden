#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/ai/mob/MobSelectNearestTarget.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/model/map/MapCell.hpp>

using namespace shaiya::game;
using namespace shaiya::game::ai;

/**
 * The aggression distance.
 */
constexpr auto AggroDistance = 10;

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
            if (player->position().isWithinDistance(pos, AggroDistance))
                targets.push_back(player);
        }
    }

    return targets.empty() ? nullptr : targets.front();
}