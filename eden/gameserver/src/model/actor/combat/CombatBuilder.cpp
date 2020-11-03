#include <shaiya/game/model/actor/Actor.hpp>
#include <shaiya/game/model/actor/combat/CombatBuilder.hpp>

using namespace shaiya::game;

/**
 * Initialises this combat builder for an actor.
 * @param actor The actor.
 */
CombatBuilder::CombatBuilder(Actor& combatant): combatant_(combatant)
{
}

/**
 * Attempts to attack a victim.
 * @param victim    The victim actor.
 * @return          If an attack was attempted.
 */
bool CombatBuilder::attack(std::shared_ptr<Actor> victim)
{
    if (!canAttack(victim))  // Validate that we can attack the target
        return false;

    victim_ = std::move(victim);
    return true;
}

/**
 * Checks if our combatant can attack a victim.
 * @param victim    The victim actor.
 * @return          If the victim can be attacked.
 */
bool CombatBuilder::canAttack(const std::shared_ptr<Actor>& victim)
{
    return true;
}

/**
 * Ticks the combat of an actor.
 */
void CombatBuilder::tick()
{
    if (!inCombat())
        return;

    combatant_.setPosition(victim_->position());
}

/**
 * Checks if our combatant is in combat.
 * @return  If the combatant is in combat.
 */
bool CombatBuilder::inCombat() const
{
    return victim_ != nullptr;
}