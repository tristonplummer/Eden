#include <shaiya/common/net/packet/game/PlayerAutoAttack.hpp>
#include <shaiya/game/model/actor/Actor.hpp>
#include <shaiya/game/model/actor/combat/CombatBuilder.hpp>
#include <shaiya/game/model/actor/combat/strategy/DefaultCombatStrategy.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/net/GameSession.hpp>

using namespace shaiya::game;
using namespace std::chrono;
using namespace std::chrono_literals;

/**
 * Initialises this combat builder for an actor.
 * @param actor The actor.
 */
CombatBuilder::CombatBuilder(Actor& combatant): combatant_(combatant)
{
    strategy_ = std::make_unique<DefaultCombatStrategy>();
}

/**
 * Attempts to attack a victim.
 * @param victim    The victim actor.
 * @return          If an attack was attempted.
 */
bool CombatBuilder::attack(const std::shared_ptr<Actor>& victim)
{
    if (!canAttack(victim))  // Validate that we can attack the target
        return false;

    victim_ = victim;
    return true;
}

/**
 * Checks if our combatant can attack a victim.
 * @param victim    The victim actor.
 * @return          If the victim can be attacked.
 */
bool CombatBuilder::canAttack(const std::shared_ptr<Actor>& victim)
{
    if (combatant_.type() == EntityType::Player)
    {
        auto wep = combatant_.equipment().at(EquipmentSlot::Weapon);
        return wep != nullptr;
    }

    return !victim->hasAttribute(Attribute::Evading);
}

/**
 * Targets an actor.
 * @param victim    The actor to target.
 */
void CombatBuilder::target(const std::shared_ptr<Actor>& victim)
{
    victim_ = victim;
}

/**
 * Ticks the combat of an actor.
 */
void CombatBuilder::tick()
{
    hitQueue_.clear();

    if (!inCombat())
        return;

    auto now = steady_clock::now();
    if (now < nextAttackTime_)
        return;

    if (strategy_)
    {
        if (canAttack(victim_))
        {
            if (strategy_->isWithinRange(combatant_, *victim_))
            {
                auto hits = strategy_->attack(combatant_, *victim_);
                if (!hits.empty())
                {
                    for (auto&& hit: hits)
                    {
                        auto& stats = hit.victim().stats();
                        if (!hit.missed())
                        {
                            auto [hitpoints, mana, stamina] = hit.damage();
                            stats.setHitpoints(stats.currentHitpoints() - hitpoints);
                        }

                        hitQueue_.push_back(hit);
                    }

                    combatant_.flagUpdate(UpdateFlag::Combat);
                }
            }
            else
            {
                reset();
            }
        }
        else
        {
            if (combatant_.type() == EntityType::Player)
            {
                shaiya::net::PlayerAutoAttack attack;
                attack.status = shaiya::net::AttackStatus::CannotAttack;
                attack.id     = combatant_.id();
                attack.target = victim_ == nullptr ? 0 : victim_->id();
                dynamic_cast<Player&>(combatant_).session().write(attack);
            }

            reset();
        }
    }

    nextAttackTime_ = now + milliseconds(static_cast<int>(combatant_.attackSpeed()));
}

/**
 * Resets the state of combat
 */
void CombatBuilder::reset()
{
    victim_ = nullptr;
}

/**
 * Checks if our combatant is in combat.
 * @return  If the combatant is in combat.
 */
bool CombatBuilder::inCombat() const
{
    return victim_ != nullptr;
}