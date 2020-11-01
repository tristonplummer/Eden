#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/scheduling/ScheduledTask.hpp>

namespace shaiya::game
{
    /**
     * A task that periodically checks for players that a mob can start chasing and attack.
     */
    class MobAggressionTask: public ScheduledTask
    {
    public:
        /**
         * Initialise this task.
         */
        explicit MobAggressionTask(Mob& mob);

        /**
         * Handle the execution of this task.
         */
        void execute(GameWorldService& world) override;

    private:
        /**
         * The mob to handle.
         */
        Mob& mob_;
    };
}