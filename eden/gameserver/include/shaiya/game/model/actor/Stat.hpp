#pragma once

namespace shaiya::game
{
    /**
     * Represents a statistic held by an actor.
     */
    enum class Stat
    {
        /**
         * Increases physical and ranged attack power.
         */
        Strength,

        /**
         * Increases physical and ranged attack power, and is also used in determining if a physical attack hits or misses.
         */
        Dexterity,

        /**
         * Increases physical and ranged defense, and increases the maximum hitpoints of the actor.
         */
        Reaction,

        /**
         * Increases magical attack power.
         */
        Intelligence,

        /**
         * Increases the actor's total mana and magical resistance, and is also used in determining if a magical attack hits
         * or misses.
         */
        Wisdom,

        /**
         * Increases ranged attack power, and is used to determine the critical success chance when attacking another actor.
         */
        Luck,

        /**
         * The physical defense of an actor.
         */
        Defense,

        /**
         * The magical resistance of an actor.
         */
        Resistance,

        /**
         * The absorption of an actor. The difference between defense and absorption, is that absorption
         * is removed from the incoming damage before the critical attack bonus is applied, and applies to all sources of
         * damage including damage-over-time effects.
         */
        Absorption,

        /**
         * The minimum physical attack of an actor.
         */
        MinPhysicalAttack,

        /**
         * The maximum physical attack of an actor.
         */
        MaxPhysicalAttack,

        /**
         * The minimum ranged attack of an actor. This is used over physical attack when the actor
         * is an archer type class.
         */
        MinRangedAttack,

        /**
         * The maximum ranged attack of an actor. This is used over physical attack when the actor
         * is an archer type class.
         */
        MaxRangedAttack,

        /**
         * The minimum magical attack of an actor.
         */
        MinMagicalAttack,

        /**
         * The maximum magical attack of an actor.
         */
        MaxMagicalAttack,

        /**
         * The maximum health value of an actor. This is modified by reaction.
         */
        MaxHealth,

        /**
         * The maximum mana value of an actor. This is modified by wisdom.
         */
        MaxMana,

        /**
         * The maximum stamina value of an actor. This is modified by dexterity.
         */
        MaxStamina
    };
}