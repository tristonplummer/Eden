#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/model/actor/combat/hit/Hit.hpp>

#include <vector>

namespace shaiya::game
{
    /**
     * Represents a strategy for executing a combat cycle between actors.
     */
    class CombatStrategy
    {
    public:
        /**
         * Initiates the attack.
         * @param combatant The attacker.
         * @param victim    The victim.
         */
        std::vector<Hit> attack(Actor& combatant, Actor& victim);

        /**
         * Checks if the combatant is within range of the victim.
         * @param combatant The attacker.
         * @param victim    The victim.
         * @return          If the combatant is within range.
         */
        virtual bool isWithinRange(Actor& combatant, Actor& victim) = 0;

    private:
        /**
         * Calculates the damage done to the victim.
         * @param combatant The attacker.
         * @param victim    The victim.
         * @return          The base damage to deal.
         */
        virtual double calculateDamage(Actor& combatant, Actor& victim) = 0;

        /**
         * Calculates the accuracy chance against the victim.
         * @param combatant The attacker.
         * @param victim    The victim.
         * @return          The accuracy chance.
         */
        virtual int calculateAccuracy(Actor& combatant, Actor& victim) = 0;

        /**
         * Calculates the critical chance against the victim.
         * @param combatant The attacker.
         * @param victim    The victim.
         * @return          The critical chance.
         */
        virtual int calculateCriticalChance(Actor& combatant, Actor& victim) = 0;

        /**
         * Calculates the critical damage multiplier to use against the victim.
         * @param combatant The attacker.
         * @param victim    The victim.
         * @return          The critical damage multiplier.
         */
        virtual double criticalMultiplier(Actor& combatant, Actor& victim);
    };
}