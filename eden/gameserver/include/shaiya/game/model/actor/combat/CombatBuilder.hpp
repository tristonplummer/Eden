#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/model/actor/combat/hit/Hit.hpp>
#include <shaiya/game/model/actor/combat/strategy/CombatStrategy.hpp>

#include <chrono>
#include <vector>

namespace shaiya::game
{
    /**
     * The combat builder is a class that facilitates the construction of combat related events for all actors, such as
     * engaging in combat, casting a spell, and so on.
     */
    class CombatBuilder
    {
    public:
        /**
         * Initialises this combat builder for an actor.
         * @param actor The actor.
         */
        explicit CombatBuilder(Actor& combatant);

        /**
         * Attempts to attack a victim.
         * @param victim    The victim actor.
         * @return          If an attack was attempted.
         */
        bool attack(const std::shared_ptr<Actor>& victim);

        /**
         * Checks if our combatant can attack a victim.
         * @param victim    The victim actor.
         * @return          If the victim can be attacked.
         */
        bool canAttack(const std::shared_ptr<Actor>& victim);

        /**
         * Targets an actor.
         * @param victim    The actor to target.
         */
        void target(const std::shared_ptr<Actor>& victim);

        /**
         * Ticks the combat of an actor.
         */
        void tick();

        /**
         * Resets the state of combat
         */
        void reset();

        /**
         * Get the hit queue of this combat builder.
         * @return  The hits.
         */
        [[nodiscard]] const std::vector<Hit>& hits() const
        {
            return hitQueue_;
        }

        /**
         * Checks if our combatant is in combat.
         * @return  If the combatant is in combat.
         */
        [[nodiscard]] bool inCombat() const;

        /**
         * Gets the victim of this combat builder.
         * @return  The victim.
         */
        [[nodiscard]] const std::shared_ptr<Actor>& victim() const
        {
            return victim_;
        }

    private:
        /**
         * The actor that owns this combat builder.
         */
        Actor& combatant_;

        /**
         * The victim that the target is engaged in.
         */
        std::shared_ptr<Actor> victim_;

        /**
         * The combat strategy used by this combat builder.
         */
        std::unique_ptr<CombatStrategy> strategy_;

        /**
         * The vector of hits that were performed
         */
        std::vector<Hit> hitQueue_;

        /**
         * The next time that an actor can perform an attack.
         */
        std::chrono::steady_clock::time_point nextAttackTime_{ std::chrono::steady_clock::now() };
    };
}