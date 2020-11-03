#pragma once
#include <shaiya/game/Forward.hpp>

namespace shaiya::game::ai
{
    /**
     * Selects the closest player target for a mob.
     */
    class MobResetEvade
    {
    public:
        /**
         * Initialises the AI.
         * @param mob   The mob to operate on.
         */
        explicit MobResetEvade(Mob& mob);

        /**
         * Executes the resetting & evading script.
         */
        void execute();

    private:
        /**
         * The mob instance.
         */
        Mob& mob_;
    };
}