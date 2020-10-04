#include <shaiya/game/world/model/actor/StatSet.hpp>

#include <cmath>
#include <stdexcept>

using namespace shaiya::game;

// Okay, listen. I know what you're thinking. Cups, what the fuck is this monstrosity? I know, look, I'm sorry.
// It works well enough, it does what it's supposed to do, and it provides a not-completely-terrible API, so fuck off.
// Thank you.

/**
 * Sets the base value for a stat. This is the stat value that an has before
 * any modifications from equipment or buffs, such as that assigned from a mob definition, or
 * selected by a character in that stat allocation page.
 * @param stat  The stat.
 * @param value The base value of the stat.
 * @param sync  If we should synchronise the stats after.
 */
void StatSet::setBase(Stat stat, int32_t value, bool sync)
{
    if (value == getBase(stat))  // No-op if we're not actually changing any values.
        return;

    switch (stat)
    {
        case Stat::Strength: strength_.base = value; break;
        case Stat::Dexterity: dexterity_.base = value; break;
        case Stat::Reaction: reaction_.base = value; break;
        case Stat::Intelligence: intelligence_.base = value; break;
        case Stat::Wisdom: wisdom_.base = value; break;
        case Stat::Luck: luck_.base = value; break;
        case Stat::Defense: defense_.base = value; break;
        case Stat::Resistance: resistance_.base = value; break;
        case Stat::Absorption: absorption_.base = value; break;
        case Stat::MinPhysicalAttack: minPhysicalAttack_.base = value; break;
        case Stat::MaxPhysicalAttack: maxPhysicalAttack_.base = value; break;
        case Stat::MinRangedAttack: minRangedAttack_.base = value; break;
        case Stat::MaxRangedAttack: maxRangedAttack_.base = value; break;
        case Stat::MinMagicalAttack: minMagicalAttack_.base = value; break;
        case Stat::MaxMagicalAttack: maxMagicalAttack_.base = value; break;
        case Stat::MaxHealth: maxHitpoints_.base = value; break;
        case Stat::MaxMana: maxMana_.base = value; break;
        case Stat::MaxStamina: maxStamina_.base = value; break;
    }

    if (sync)
        this->sync();
}

/**
 * Sets an additional value for a stat. This is the stat value that is affected by buffs and equipment.
 * @param stat  The stat.
 * @param value The value of the stat.
 * @param sync  If we should synchronise the stats after.
 */
void StatSet::setAdditional(Stat stat, int32_t value, bool sync)
{
    if (value == getAdditional(stat))  // No-op if we're not actually changing any values.
        return;

    switch (stat)
    {
        case Stat::Strength: strength_.additional = value; break;
        case Stat::Dexterity: dexterity_.additional = value; break;
        case Stat::Reaction: reaction_.additional = value; break;
        case Stat::Intelligence: intelligence_.additional = value; break;
        case Stat::Wisdom: wisdom_.additional = value; break;
        case Stat::Luck: luck_.additional = value; break;
        case Stat::Defense: defense_.additional = value; break;
        case Stat::Resistance: resistance_.additional = value; break;
        case Stat::Absorption: absorption_.additional = value; break;
        case Stat::MinPhysicalAttack: minPhysicalAttack_.additional = value; break;
        case Stat::MaxPhysicalAttack: maxPhysicalAttack_.additional = value; break;
        case Stat::MinRangedAttack: minRangedAttack_.additional = value; break;
        case Stat::MaxRangedAttack: maxRangedAttack_.additional = value; break;
        case Stat::MinMagicalAttack: minMagicalAttack_.additional = value; break;
        case Stat::MaxMagicalAttack: maxMagicalAttack_.additional = value; break;
        case Stat::MaxHealth: maxHitpoints_.additional = value; break;
        case Stat::MaxMana: maxMana_.additional = value; break;
        case Stat::MaxStamina: maxStamina_.additional = value; break;
    }

    if (sync)
        this->sync();
}

/**
 * Sets the current hitpoints of the actor.
 * @param hitpoints The current hitpoints.
 */
void StatSet::setHitpoints(int32_t hitpoints)
{
    if (hitpoints < 0)
        hitpoints = 0;
    currentHitpoints_ = hitpoints;
}

/**
 * Sets the current mana of the actor.
 * @param mana  The current mana.
 */
void StatSet::setMana(int32_t mana)
{
    if (mana < 0)
        mana = 0;
    currentMana_ = mana;
}

/**
 * Sets the current stamina of the actor.
 * @param stamina   The current stamina.
 */
void StatSet::setStamina(int32_t stamina)
{
    if (stamina < 0)
        stamina = 0;
    currentStamina_ = stamina;
}

/**
 * Synchronises the stats by performing the calculations for attack power, defense, resistance, and other
 * secondary stats.
 */
void StatSet::sync()
{
    // Calculate the primary stat totals
    strength_.total     = strength_.base + strength_.additional;
    dexterity_.total    = dexterity_.base + dexterity_.additional;
    reaction_.total     = reaction_.base + reaction_.additional;
    intelligence_.total = intelligence_.base + intelligence_.additional;
    wisdom_.total       = wisdom_.base + wisdom_.additional;
    luck_.total         = luck_.base + luck_.additional;

    // The actor's physical defense is equal to their base defense, plus the defense they gain
    // from buffs and equipment, plus their total reaction stat.
    defense_.total = defense_.base + defense_.additional + reaction_.total;

    // The actor's magical resistance is equal to their base resistance, plus the resistance they gain
    // from buffs and equipment, plus double their total wisdom stat.
    resistance_.total = resistance_.base + resistance_.additional + (wisdom_.total * 2);

    // Absorption is not modified by any other stats.
    absorption_.total = absorption_.base + absorption_.additional;

    // Calculate the actor's maximum health, mana, and stamina
    maxHitpoints_.total = maxHitpoints_.base + maxHitpoints_.additional + (reaction_.total * 5);
    maxMana_.total      = maxMana_.base + maxMana_.additional + (wisdom_.total * 5);
    maxStamina_.total   = maxStamina_.base + maxStamina_.additional + (dexterity_.total * 5);

    // Calculate the physical attack bonus
    auto physicalAttackBonus = 0;
    physicalAttackBonus += std::floor(strength_.total * 1.3);    // Strength adds 1.3 Physical attack power per point
    physicalAttackBonus += std::floor(dexterity_.total * 0.25);  // Dexterity adds 0.25 Physical attack power per point.

    // Calculate the ranged attack bonus
    auto rangedAttackBonus = 0;
    rangedAttackBonus += strength_.total;                     // Strength adds 1 Ranged attack power per point.
    rangedAttackBonus += std::floor(dexterity_.total * 0.2);  // Dexterity adds 0.2 Ranged attack power per point.
    rangedAttackBonus += std::floor(luck_.total * 0.3);       // Luck adds 0.3 Ranged attack power per point.

    // Calculate the magical attack bonus
    auto magicalAttackBonus = 0;
    magicalAttackBonus += std::floor(intelligence_.total * 1.3);  // Intelligence adds 1.3 Magical attack power per point.
    magicalAttackBonus += std::floor(wisdom_.total * 0.2);        // Wisdom adds 0.2 Magical attack power per point.

    // Calculate the total minimum and maximum attack powers
    minPhysicalAttack_.total = minPhysicalAttack_.base + minPhysicalAttack_.additional + physicalAttackBonus;
    minRangedAttack_.total   = minRangedAttack_.base + minRangedAttack_.additional + rangedAttackBonus;
    minMagicalAttack_.total  = minMagicalAttack_.base + minMagicalAttack_.additional + magicalAttackBonus;

    maxPhysicalAttack_.total = maxPhysicalAttack_.base + maxPhysicalAttack_.additional + physicalAttackBonus;
    maxRangedAttack_.total   = maxRangedAttack_.base + maxRangedAttack_.additional + rangedAttackBonus;
    maxMagicalAttack_.total  = maxMagicalAttack_.base + maxMagicalAttack_.additional + magicalAttackBonus;
}

/**
 * Gets the base value of a stat.
 * @param stat  The stat.
 * @return      The base value.
 */
int32_t StatSet::getBase(Stat stat) const
{
    switch (stat)
    {
        case Stat::Strength: return strength_.base;
        case Stat::Dexterity: return dexterity_.base;
        case Stat::Reaction: return reaction_.base;
        case Stat::Intelligence: return intelligence_.base;
        case Stat::Wisdom: return wisdom_.base;
        case Stat::Luck: return luck_.base;
        case Stat::Defense: return defense_.base;
        case Stat::Resistance: return resistance_.base;
        case Stat::Absorption: return absorption_.base;
        case Stat::MinPhysicalAttack: return minPhysicalAttack_.base;
        case Stat::MaxPhysicalAttack: return maxPhysicalAttack_.base;
        case Stat::MinRangedAttack: return minRangedAttack_.base;
        case Stat::MaxRangedAttack: return maxRangedAttack_.base;
        case Stat::MinMagicalAttack: return minMagicalAttack_.base;
        case Stat::MaxMagicalAttack: return maxMagicalAttack_.base;
        case Stat::MaxHealth: return maxHitpoints_.base;
        case Stat::MaxMana: return maxMana_.base;
        case Stat::MaxStamina: return maxStamina_.base;
    }

    throw std::invalid_argument("Tried to get the base value of an unknown stat.");
}

/**
 * Gets the additional value of a stat.
 * @param stat  The stat.
 * @return      The additional value.
 */
int32_t StatSet::getAdditional(Stat stat) const
{
    switch (stat)
    {
        case Stat::Strength: return strength_.additional;
        case Stat::Dexterity: return dexterity_.additional;
        case Stat::Reaction: return reaction_.additional;
        case Stat::Intelligence: return intelligence_.additional;
        case Stat::Wisdom: return wisdom_.additional;
        case Stat::Luck: return luck_.additional;
        case Stat::Defense: return defense_.additional;
        case Stat::Resistance: return resistance_.additional;
        case Stat::Absorption: return absorption_.additional;
        case Stat::MinPhysicalAttack: return minPhysicalAttack_.additional;
        case Stat::MaxPhysicalAttack: return maxPhysicalAttack_.additional;
        case Stat::MinRangedAttack: return minRangedAttack_.additional;
        case Stat::MaxRangedAttack: return maxRangedAttack_.additional;
        case Stat::MinMagicalAttack: return minMagicalAttack_.additional;
        case Stat::MaxMagicalAttack: return maxMagicalAttack_.additional;
        case Stat::MaxHealth: return maxHitpoints_.additional;
        case Stat::MaxMana: return maxMana_.additional;
        case Stat::MaxStamina: return maxStamina_.additional;
    }

    throw std::invalid_argument("Tried to get the additional value of an unknown stat.");
}

/**
 * Gets the total value of a stat.
 * @param stat  The stat.
 * @return      The total value.
 */
int32_t StatSet::getTotal(Stat stat) const
{
    switch (stat)
    {
        case Stat::Strength: return strength_.total;
        case Stat::Dexterity: return dexterity_.total;
        case Stat::Reaction: return reaction_.total;
        case Stat::Intelligence: return intelligence_.total;
        case Stat::Wisdom: return wisdom_.total;
        case Stat::Luck: return luck_.total;
        case Stat::Defense: return defense_.total;
        case Stat::Resistance: return resistance_.total;
        case Stat::Absorption: return absorption_.total;
        case Stat::MinPhysicalAttack: return minPhysicalAttack_.total;
        case Stat::MaxPhysicalAttack: return maxPhysicalAttack_.total;
        case Stat::MinRangedAttack: return minRangedAttack_.total;
        case Stat::MaxRangedAttack: return maxRangedAttack_.total;
        case Stat::MinMagicalAttack: return minMagicalAttack_.total;
        case Stat::MaxMagicalAttack: return maxMagicalAttack_.total;
        case Stat::MaxHealth: return maxHitpoints_.total;
        case Stat::MaxMana: return maxMana_.total;
        case Stat::MaxStamina: return maxStamina_.total;
    }

    throw std::invalid_argument("Tried to get the total value of an unknown stat.");
}