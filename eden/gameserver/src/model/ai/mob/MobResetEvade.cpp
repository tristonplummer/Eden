#include <shaiya/game/model/Attribute.hpp>
#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/ai/mob/MobResetEvade.hpp>

using namespace shaiya::game;
using namespace shaiya::game::ai;

/**
 * The distance from the spawn point that a mob should reset.
 */
constexpr auto MobResetDistance = 30.0f;

/**
 * Initialises the AI.
 * @param mob   The mob to operate on.
 */
MobResetEvade::MobResetEvade(Mob& mob): mob_(mob)
{
}

/**
 * Executes the resetting & evading script.
 */
void MobResetEvade::execute()
{
    auto& spawn = mob_.spawnArea();  // The spawn area of the mob
    auto& pos   = mob_.position();   // The position of the mob

    // Clear evasion flag
    if (spawn.contains(mob_.position()))
    {
        mob_.clearAttribute(Attribute::Evading);
        return;
    }

    // If the mob is too far from it's spawn point, force it to reset back
    if (spawn.distanceTo(pos) > MobResetDistance && !mob_.hasAttribute(Attribute::Evading))
    {
        mob_.setAttribute(Attribute::Evading);
        mob_.combat().reset();
        mob_.movement().moveTo(spawn.randomPoint());
    }
}