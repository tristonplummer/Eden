#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/scheduling/ScheduledTask.hpp>

namespace shaiya::game
{
    /**
     * A task that periodically moves an NPC or Mob.
     */
    class NpcMovementTask: public ScheduledTask
    {
    public:
        /**
         * Initialise this task.
         */
        explicit NpcMovementTask();

        /**
         * Handle the execution of this task.
         */
        void execute(GameWorldService& world) override;
    };
}