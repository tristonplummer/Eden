#include <shaiya/common/util/Prng.hpp>
#include <shaiya/game/model/actor/Actor.hpp>
#include <shaiya/game/model/actor/combat/strategy/DefaultCombatStrategy.hpp>
#include <shaiya/game/model/item/Item.hpp>

using namespace shaiya::game;

/**
 * The minimum accuracy for physical attacks.
 */
constexpr auto MinimumPhysicalAccuracy = 20;

/**
 * The maximum accuracy for physical attacks.
 */
constexpr auto MaximumPhysicalAccuracy = 100;

/**
 * The number of dexterity points difference per evasion percent.
 */
constexpr auto DexterityChance = 15.0;

/**
 * Checks if the combatant is within range of the victim.
 * @param combatant The attacker.
 * @param victim    The victim.
 * @return          If the combatant is within range.
 */
bool DefaultCombatStrategy::isWithinRange(Actor& combatant, Actor& victim)
{
    auto weapon = combatant.equipment().at(EquipmentSlot::Weapon);
    auto range  = combatant.size();

    if (weapon)
    {
        auto& def = weapon->definition();
        range += def.attackRange;
    }

    return combatant.position().isWithinDistance(victim.position(), range);
}

/**
 * Calculates the damage done to the victim.
 * @param combatant The attacker.
 * @param victim    The victim.
 * @return          The base damage to deal.
 */
double DefaultCombatStrategy::calculateDamage(Actor& combatant, Actor& victim)
{
    auto& prng           = shaiya::Prng::the();
    auto& combatantStats = combatant.stats();
    auto& victimStats    = victim.stats();

    // The minimum and maximum physical attack values
    auto minPhysical = combatantStats.getTotal(Stat::MinPhysicalAttack);
    auto maxPhysical = combatantStats.getTotal(Stat::MaxPhysicalAttack);

    return prng.random(minPhysical, maxPhysical);
}

/**
 * Calculates the accuracy chance against the victim.
 * @param combatant The attacker.
 * @param victim    The victim.
 * @return          The accuracy chance.
 */
int DefaultCombatStrategy::calculateAccuracy(Actor& combatant, Actor& victim)
{
    auto combatantDexterity = combatant.stats().getTotal(Stat::Dexterity);
    auto victimDexterity    = victim.stats().getTotal(Stat::Dexterity);

    if (combatantDexterity >= victimDexterity)  // If the attacker has more dex, they will always land a hit
        return MaximumPhysicalAccuracy;

    auto delta  = victimDexterity - combatantDexterity;
    auto chance = delta / DexterityChance;
    return std::max(MinimumPhysicalAccuracy, MaximumPhysicalAccuracy - (int)std::floor(chance));
}

/**
 * Calculates the critical chance against the victim.
 * @param combatant The attacker.
 * @param victim    The victim.
 * @return          The critical chance.
 */
int DefaultCombatStrategy::calculateCriticalChance(Actor& combatant, Actor& victim)
{
    return 50;
}
