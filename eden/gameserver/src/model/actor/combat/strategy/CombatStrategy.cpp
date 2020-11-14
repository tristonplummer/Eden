#include <shaiya/common/util/Prng.hpp>
#include <shaiya/game/model/actor/combat/strategy/CombatStrategy.hpp>

using namespace shaiya::game;

/**
 * Initiates the attack.
 * @param combatant The attacker.
 * @param victim    The victim.
 */
std::vector<Hit> CombatStrategy::attack(Actor& combatant, Actor& victim)
{
    auto& prng = shaiya::Prng::the();
    std::vector<Hit> hits;

    auto accuracy = calculateAccuracy(combatant, victim);
    if (!prng.percentage(accuracy))
    {
        hits.emplace_back(victim, false);
        return hits;
    }

    Hit hit(victim);

    auto damage   = calculateDamage(combatant, victim);
    auto critical = calculateCriticalChance(combatant, victim);
    if (prng.percentage(critical))
    {
        damage *= criticalMultiplier(combatant, victim);
        hit.setCritical(true);
    }

    hit.setDamage(std::floor(damage));
    hits.push_back(hit);
    return hits;
}

/**
 * Calculates the critical damage multiplier to use against the victim.
 * @param combatant The attacker.
 * @param victim    The victim.
 * @return          The critical damage multiplier.
 */
double CombatStrategy::criticalMultiplier(Actor& combatant, Actor& victim)
{
    return 1.5;
}