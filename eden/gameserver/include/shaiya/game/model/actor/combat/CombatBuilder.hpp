#pragma once
#include <shaiya/game/Forward.hpp>

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
        bool attack(std::shared_ptr<Actor> victim);

        /**
         * Checks if our combatant can attack a victim.
         * @param victim    The victim actor.
         * @return          If the victim can be attacked.
         */
        bool canAttack(const std::shared_ptr<Actor>& victim);

        /**
         * Ticks the combat of an actor.
         */
        void tick();

        /**
         * Resets the state of combat
         */
        void reset();

        /**
         * Checks if our combatant is in combat.
         * @return  If the combatant is in combat.
         */
        [[nodiscard]] bool inCombat() const;

    private:
        /**
         * The actor that owns this combat builder.
         */
        Actor& combatant_;

        /**
         * The victim that the target is engaged in
         */
        std::shared_ptr<Actor> victim_;
    };
}