#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/scheduling/ScheduledTask.hpp>

namespace shaiya::game
{
    /**
     * A task that handles the respawning of a mob.
     */
    class MobRespawnTask: public ScheduledTask
    {
    public:
        /**
         * Initialise this task.
         * @param mob   The mob to operate on.
         * @param delay The respawn delay.
         */
        explicit MobRespawnTask(Mob& mob, size_t delay);

        /**
         * Handle the execution of this task.
         */
        void execute(GameWorldService& world) override;

    private:
        /**
         * The mob who this task is to operate on.
         */
        Mob& mob_;

        /**
         * The number of times this task has been executed.
         */
        size_t cycles_{ 0 };
    };
}