#pragma once

namespace shaiya::game
{
    /**
     * Represents the attack speed of an actor, with the value being the minimum time in between
     * attack cycles.
     */
    enum class AttackSpeed
    {
        Disabled      = 0,
        ExtremelySlow = 4000,
        VerySlow      = 3750,
        Slow          = 3500,
        ABitSlow      = 3250,
        Normal        = 3000,
        ABitFast      = 2750,
        Fast          = 2500,
        VeryFast      = 2250,
        ExtremelyFast = 2000
    };
}