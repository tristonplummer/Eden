#pragma once
#include <shaiya/game/world/scheduling/ScheduledTask.hpp>

namespace shaiya::game
{
    class Actor;

    /**
     * A task that periodically normalizes the health of an actor.
     */
    class HealthNormalizationTask: public ScheduledTask
    {
    public:
        /**
         * Initialise this task.
         * @param actor The actor to operate on.
         */
        explicit HealthNormalizationTask(Actor& actor);

        /**
         * Handle the execution of this task.
         */
        void execute() override;

    private:
        /**
         * The actor who this task is to operate on.
         */
        Actor& actor_;
    };
}