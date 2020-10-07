#include <shaiya/game/world/scheduling/ScheduledTask.hpp>

using namespace shaiya::game;

/**
 * Initialises this task.
 * @param delay The task delay before executing.
 */
ScheduledTask::ScheduledTask(size_t delay)
{
    delay_ = delay;
}

/**
 * Stops this task.
 */
void ScheduledTask::stop()
{
    running_ = false;
}

/**
 * Pulses this task, updating the delay and calling ScheduledTask#execute if necessary.
 */
void ScheduledTask::pulse()
{
    if (running_ && --pulses_ <= 0)
    {
        execute();
        pulses_ = delay_;
    }
}