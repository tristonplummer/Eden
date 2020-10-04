#pragma once
#include <shaiya/game/world/model/actor/Stat.hpp>

#include <cstdint>

namespace shaiya::game
{
    /**
     * "Statistics" (stats) may be used by an actor to determine
     * combat values.
     */
    class StatSet
    {
    public:
        /**
         * Default initialises all values to 0.
         */
        StatSet() = default;

        /**
         * Sets the base value for a stat. This is the stat value that an has before
         * any modifications from equipment or buffs, such as that assigned from a mob definition, or
         * selected by a character in that stat allocation page.
         * @param stat  The stat.
         * @param value The base value of the stat.
         * @param sync  If we should synchronise the stats after.
         */
        void setBase(Stat stat, int32_t value, bool sync = true);

        /**
         * Sets an additional value for a stat. This is the stat value that is affected by buffs and equipment.
         * @param stat  The stat.
         * @param value The value of the stat.
         * @param sync  If we should synchronise the stats after.
         */
        void setAdditional(Stat stat, int32_t value, bool sync = true);

        /**
         * Sets the current hitpoints of the actor.
         * @param hitpoints The current hitpoints.
         */
        void setHitpoints(int32_t hitpoints);

        /**
         * Sets the current mana of the actor.
         * @param mana  The current mana.
         */
        void setMana(int32_t mana);

        /**
         * Sets the current stamina of the actor.
         * @param stamina   The current stamina.
         */
        void setStamina(int32_t stamina);

        /**
         * Synchronises the stats for computing their total values, and modifying the total
         * values accordingly. This is usually called when a stat value is modified.
         */
        void sync();

        /**
         * Gets the base value of a stat.
         * @param stat  The stat.
         * @return      The base value.
         */
        [[nodiscard]] int32_t getBase(Stat stat) const;

        /**
         * Gets the additional value of a stat.
         * @param stat  The stat.
         * @return      The additional value.
         */
        [[nodiscard]] int32_t getAdditional(Stat stat) const;

        /**
         * Gets the total value of a stat.
         * @param stat  The stat.
         * @return      The total value.
         */
        [[nodiscard]] int32_t getTotal(Stat stat) const;

        /**
         * Gets the current hitpoints of the actor.
         * @return  The current hitpoints.
         */
        [[nodiscard]] uint32_t currentHitpoints() const
        {
            return currentHitpoints_;
        }

        /**
         * Gets the current mana of the actor.
         * @return  The current mana.
         */
        [[nodiscard]] uint32_t currentMana() const
        {
            return currentMana_;
        }

        /**
         * Gets the current stamina of the actor.
         * @return  The current stamina.
         */
        [[nodiscard]] uint32_t currentStamina() const
        {
            return currentStamina_;
        }

        /**
         * Gets the maximum hitpoints of the actor.
         * @return  The maximum hitpoints.
         */
        [[nodiscard]] uint32_t maxHitpoints() const
        {
            return maxHitpoints_.total;
        }

        /**
         * Gets the maximum mana of the actor.
         * @return  The maximum mana.
         */
        [[nodiscard]] uint32_t maxMana() const
        {
            return maxMana_.total;
        }

        /**
         * Gets the maximum stamina of the actor.
         * @return  The maximum stamina.
         */
        [[nodiscard]] uint32_t maxStamina() const
        {
            return maxStamina_.total;
        }

    private:
        /**
         * Represents the values of a stat. This is represented as a triple value, as
         * the "total" isn't simply just a calculation of base and additional - it can be modified by the
         * StatSet#sync method in some cases such as with resistance, defense, and attack power values.
         */
        struct StatTriple
        {
            int32_t base{ 0 };
            int32_t additional{ 0 };
            int32_t total{ 0 };
        };

        /**
         * The strength of an actor. Strength increases physical and ranged attack power.
         */
        StatTriple strength_;

        /**
         * The dexterity of an actor. Dexterity increases physical and ranged attack power, and is
         * also used in determining if a physical attack hits or misses.
         */
        StatTriple dexterity_;

        /**
         * The reaction of an actor. Reaction increases physical and ranged defense, and increases the maximum
         * hitpoints of the actor.
         */
        StatTriple reaction_;

        /**
         * The intelligence of an actor. Intelligence increases magical attack power.
         */
        StatTriple intelligence_;

        /**
         * The wisdom of an actor. Wisdom increases the actor's total mana and magical resistance, and is also used
         * in determining if a magical attack hits or misses.
         */
        StatTriple wisdom_;

        /**
         * The luck of an actor. Luck increases ranged attack power, and is used to determine the critical success chance
         * when attacking another actor.
         */
        StatTriple luck_;

        /**
         * The physical defense of an actor.
         */
        StatTriple defense_;

        /**
         * The magical resistance of an actor.
         */
        StatTriple resistance_;

        /**
         * The absorption of an actor. The difference between defense and absorption, is that absorption
         * is removed from the incoming damage before the critical attack bonus is applied, and applies to all sources of
         * damage including damage-over-time effects.
         */
        StatTriple absorption_;

        /**
         * The minimum physical attack of an actor.
         */
        StatTriple minPhysicalAttack_;

        /**
         * The maximum physical attack of an actor.
         */
        StatTriple maxPhysicalAttack_;

        /**
         * The minimum ranged attack of an actor. This is used over physical attack when the actor
         * is an archer type class.
         */
        StatTriple minRangedAttack_;

        /**
         * The maximum ranged attack of an actor. This is used over physical attack when the actor
         * is an archer type class.
         */
        StatTriple maxRangedAttack_;

        /**
         * The minimum magical attack of an actor.
         */
        StatTriple minMagicalAttack_;

        /**
         * The maximum magical attack of an actor.
         */
        StatTriple maxMagicalAttack_;

        /**
         * The current hitpoints of an actor.
         */
        int32_t currentHitpoints_{ 0 };

        /**
         * The current mana of an actor.
         */
        int32_t currentMana_{ 0 };

        /**
         * The current stamina of an actor.
         */
        int32_t currentStamina_{ 0 };

        /**
         * The maximum health value of an actor.
         */
        StatTriple maxHitpoints_;

        /**
         * The maximum mana value of an actor.
         */
        StatTriple maxMana_;

        /**
         * The maximum stamina value of an actor.
         */
        StatTriple maxStamina_;
    };
}