#pragma once
#include <shaiya/game/world/scheduling/ScheduledTask.hpp>

#include <memory>
#include <queue>
#include <vector>

namespace shaiya::game
{
    /**
     * A class which manages scheduled tasks
     */
    class Scheduler
    {
    public:
        /**
         * Pulses the active tasks, and removes those that are no longer running.
         */
        void pulse();

        /**
         * Schedules a task to be executed in the future.
         * @param task  The task.
         */
        void schedule(std::shared_ptr<ScheduledTask> task);

    private:
        /**
         * The pending tasks.
         */
        std::queue<std::shared_ptr<ScheduledTask>> pendingTasks_;

        /**
         * The active task.
         */
        std::vector<std::shared_ptr<ScheduledTask>> activeTasks_;
    };
}