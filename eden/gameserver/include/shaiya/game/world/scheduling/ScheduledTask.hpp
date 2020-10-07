#pragma once
#include <cstddef>
#include <cstdint>

namespace shaiya::game
{
    /**
     * A game-related task that is scheduled to run in the future.
     */
    class ScheduledTask
    {
    public:
        /**
         * Initialises this task.
         * @param delay The task delay before executing.
         */
        explicit ScheduledTask(size_t delay);

        /**
         * Executes this task.
         */
        virtual void execute() = 0;

        /**
         * Pulses this task, updating the delay and calling ScheduledTask#execute if necessary.
         */
        void pulse();

        /**
         * Stops this task.
         */
        void stop();

        /**
         * Checks if this task is running.
         * @return  If the task is running.
         */
        [[nodiscard]] bool running() const
        {
            return running_;
        }

    private:
        /**
         * If this task is running.
         */
        bool running_{ true };

        /**
         * The delay between executions.
         */
        size_t delay_{ 0 };

        /**
         * The current pulse count.
         */
        int32_t pulses_{ 0 };
    };
}