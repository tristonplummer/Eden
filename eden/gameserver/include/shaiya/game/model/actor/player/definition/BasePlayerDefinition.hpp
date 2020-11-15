#pragma once
#include <cstdint>

namespace shaiya::game
{
    /**
     * Represents the base stat values of a player at a given level and class.
     */
    struct BasePlayerDefinition
    {
        /**
         * The base hitpoints value.
         */
        int32_t hitpoints{};

        /**
         * The hitpoints delta value.
         */
        int32_t hitpointsDelta{};

        /**
         * The base mana value.
         */
        int32_t mana{};

        /**
         * The mana delta value
         */
        int32_t manaDelta{};

        /**
         * The base stamina value.
         */
        int32_t stamina{};

        /**
         * The stamina delta value
         */
        int32_t staminaDelta{};
    };
}