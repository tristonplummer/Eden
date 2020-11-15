#include <shaiya/game/scheduling/ScheduledTask.hpp>

using namespace shaiya::game;

/**
 * Initialises this task.
 * @param delay The task delay before executing.
 */
ScheduledTask::ScheduledTask(size_t delay)
{
    delay_  = delay;
    pulses_ = delay;
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
void ScheduledTask::pulse(GameWorldService& world)
{
    if (running_ && --pulses_ <= 0)
    {
        execute(world);
        pulses_ = delay_;
    }
}