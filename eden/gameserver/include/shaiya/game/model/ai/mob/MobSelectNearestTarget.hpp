#pragma once
#include <shaiya/game/Forward.hpp>

namespace shaiya::game::ai
{
    /**
     * Selects the closest player target for a mob.
     */
    class MobSelectNearestTarget
    {
    public:
        /**
         * Initialises the AI.
         * @param mob   The mob to operate on.
         */
        explicit MobSelectNearestTarget(Mob& mob);

        /**
         * Selects the closest target.
         * @return  The target.
         */
        std::shared_ptr<Actor> select();

    private:
        /**
         * The mob instance.
         */
        Mob& mob_;
    };
}