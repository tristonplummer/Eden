#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/scheduling/ScheduledTask.hpp>

namespace shaiya::game
{
    /**
     * A task that handles the death of an actor.
     */
    class ActorDeathTask: public ScheduledTask
    {
    public:
        /**
         * Initialise this task.
         * @param actor The actor to operate on.
         */
        explicit ActorDeathTask(Actor& actor);

        /**
         * Handle the execution of this task.
         */
        void execute(GameWorldService& world) override;

    private:
        /**
         * The actor who this task is to operate on.
         */
        Actor& actor_;
    };
}