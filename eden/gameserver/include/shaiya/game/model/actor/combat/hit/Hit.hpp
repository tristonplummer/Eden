#pragma once
#include <shaiya/game/Forward.hpp>

#include <tuple>

namespace shaiya::game
{
    /**
     * Represents a hit against an actor.
     */
    class Hit
    {
    public:
        /**
         * Initialises the hit.
         * @param victim    The victim of the hit.
         * @param connected If the hit connected with the target.
         */
        explicit Hit(Actor& victim, bool connected = true);

        /**
         * Sets the damage of this hit.
         * @param hitpoints The damage to the victim's hitpoints.
         */
        void setDamage(int32_t hitpoints);

        /**
         * Marks this hit as a critical strike.
         * @param critical  If this hit is a critical strike
         */
        void setCritical(bool critical);

        /**
         * Gets the damage values.
         * @return  The damage done to the victim's hitpoints, mana, and stamina.
         */
        [[nodiscard]] std::tuple<int32_t, int32_t, int32_t> damage() const
        {
            return { hitpointDamage_, manaDamage_, staminaDamage_ };
        }

        /**
         * Gets the victim of the attack.
         * @return  The victim.
         */
        [[nodiscard]] Actor& victim()
        {
            return victim_;
        }

        /**
         * Gets the victim of the attack.
         * @return  The victim.
         */
        [[nodiscard]] const Actor& victim() const
        {
            return victim_;
        }

        /**
         * If this hit missed the victim.
         * @return  If the hit missed.
         */
        [[nodiscard]] bool missed() const
        {
            return !connected_;
        }

        /**
         * If this hit missed the victim.
         * @return  If the hit missed.
         */
        [[nodiscard]] bool critical() const
        {
            return critical_;
        }

    private:
        /**
         * The victim of the hit.
         */
        Actor& victim_;

        /**
         * If the hit connected with the victim.
         */
        bool connected_{ false };

        /**
         * If the hit is a critical strike.
         */
        bool critical_{ false };

        /**
         * The damage to the victim's hitpoints.
         */
        int32_t hitpointDamage_{ 0 };

        /**
         * The damage to the victim's mana.
         */
        int32_t manaDamage_{ 0 };

        /**
         * The damage to the victim's stamina.
         */
        int32_t staminaDamage_{ 0 };
    };
}