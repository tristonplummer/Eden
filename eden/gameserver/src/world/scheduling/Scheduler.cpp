#include <shaiya/game/world/scheduling/Scheduler.hpp>

using namespace shaiya::game;

/**
 * Pulses the active tasks, and removes those that are no longer running.
 */
void Scheduler::pulse()
{
    // Add the pending tasks
    while (!pendingTasks_.empty())
    {
        activeTasks_.push_back(std::move(pendingTasks_.front()));
        pendingTasks_.pop();
    }

    // Iterate over the tasks
    auto itr = activeTasks_.begin();
    while (itr != activeTasks_.end())
    {
        auto task = *itr;
        task->pulse();

        if (!task->running())
        {
            itr = activeTasks_.erase(itr);
            continue;
        }
        itr++;
    }
}

/**
 * Schedules a task to be executed in the future.
 * @param task  The task.
 */
void Scheduler::schedule(std::shared_ptr<ScheduledTask> task)
{
    pendingTasks_.push(std::move(task));
}