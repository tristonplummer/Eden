#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/scheduling/impl/MobRespawnTask.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

#include <glog/logging.h>

using namespace shaiya::game;

/**
 * Initialise this task.
 * @param mob   The mob to operate on.
 * @param delay The respawn delay.
 */
MobRespawnTask::MobRespawnTask(Mob& mob, size_t delay): mob_(mob), ScheduledTask(delay)
{
}

/**
 * Handle the execution of this task.
 */
void MobRespawnTask::execute(GameWorldService& world)
{
    if (cycles_ == 0)
        mob_.deactivate();

    if (cycles_ >= 4)
    {
        mob_.activate();
        mob_.setDead(false);
        mob_.flagUpdate(UpdateFlag::Respawn);
        mob_.setPosition(mob_.spawnArea().randomPoint());

        stop();
    }

    cycles_++;
}