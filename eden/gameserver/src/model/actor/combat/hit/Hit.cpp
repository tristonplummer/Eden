#include <shaiya/game/model/actor/combat/hit/Hit.hpp>

using namespace shaiya::game;

/**
 * The maximum damage that can be dealt in a single hit.
 */
constexpr auto MaxDamage = UINT16_MAX;

/**
 * Initialises the hit.
 * @param victim    The victim of the hit.
 * @param connected If the hit connected with the target.
 */
Hit::Hit(Actor& victim, bool connected): victim_(victim), connected_(connected)
{
}

/**
 * Sets the damage of this hit.
 * @param hitpoints The damage to the victim's hitpoints.
 */
void Hit::setDamage(int32_t hitpoints)
{
    if (hitpoints > MaxDamage)
        hitpoints = MaxDamage;
    hitpointDamage_ = hitpoints;
}

/**
 * Marks this hit as a critical strike.
 * @param critical  If this hit is a critical strike
 */
void Hit::setCritical(bool critical)
{
    critical_ = critical;
}