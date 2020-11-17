#pragma once
#include <cstdint>
#include <string>

namespace shaiya::game
{
    /**
     * Represents the definition of a mob.
     */
    struct MobDefinition
    {
        /**
         * The id of the mob.
         */
        uint32_t id{};

        /**
         * The name of the mob.
         */
        std::string name{};

        /**
         * The level of the mob.
         */
        uint16_t level{};

        /**
         * The amount of experience the mob gives upon death.
         */
        uint32_t experience{};

        /**
         * The hitpoints of the mob.
         */
        uint32_t hitpoints{};

        /**
         * The stamina of the mob.
         */
        uint32_t stamina{};

        /**
         * The mana of the mob.
         */
        uint32_t mana{};

        /**
         * The dexterity of the mob.
         */
        uint32_t dexterity{};

        /**
         * The wisdom of the mob.
         */
        uint32_t wisdom{};

        /**
         * The luck of the mob.
         */
        uint32_t luck{};

        /**
         * The physical defense of the mob.
         */
        uint32_t defense{};

        /**
         * The magic resistance of the mob.
         */
        uint32_t resistance{};

        /**
         * The element of the mob.
         */
        uint8_t element{};

        /**
         * The size of the mob.
         */
        uint32_t size{};

        /**
         * The aggression radius of the mob.
         */
        uint32_t aggressionRadius{};
    };
}