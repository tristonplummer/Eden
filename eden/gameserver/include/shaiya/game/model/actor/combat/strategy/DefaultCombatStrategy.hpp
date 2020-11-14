#pragma once
#include <shaiya/game/model/actor/combat/strategy/CombatStrategy.hpp>

namespace shaiya::game
{
    /**
     * The default combat strategy for executing combat cycles.
     */
    class DefaultCombatStrategy: public CombatStrategy
    {
    public:
        /**
         * Checks if the combatant is within range of the victim.
         * @param combatant The attacker.
         * @param victim    The victim.
         * @return          If the combatant is within range.
         */
        bool isWithinRange(Actor& combatant, Actor& victim) override;

    private:
        /**
         * Calculates the damage done to the victim.
         * @param combatant The attacker.
         * @param victim    The victim.
         * @return          The base damage to deal.
         */
        double calculateDamage(Actor& combatant, Actor& victim) override;

        /**
         * Calculates the accuracy chance against the victim.
         * @param combatant The attacker.
         * @param victim    The victim.
         * @return          The accuracy chance.
         */
        int calculateAccuracy(Actor& combatant, Actor& victim) override;

        /**
         * Calculates the critical chance against the victim.
         * @param combatant The attacker.
         * @param victim    The victim.
         * @return          The critical chance.
         */
        int calculateCriticalChance(Actor& combatant, Actor& victim) override;
    };
}