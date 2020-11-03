#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::client
{
    /**
     * Represents the definition of a mob.
     */
    struct MobDefinition
    {
        /**
         * The id of the mob.
         */
        uint16_t id{};

        /**
         * The name of the mob.
         */
        VariableString<64> name{};

        /**
         * The model id of the mob.
         */
        uint16_t model{};

        /**
         * The level of the mob.
         */
        uint16_t level{};

        /**
         * The AI script number of the mob.
         */
        uint8_t ai{};

        /**
         * The amount of hitpoints the mob has.
         */
        uint32_t hitpoints{};

        /**
         * The height of the mob.
         */
        uint8_t height{};

        /**
         * The size (radius) of the mob.
         */
        uint8_t size{};

        /**
         * The element value of the mob.
         */
        uint8_t element{};

        /**
         * The interval between random walking.
         */
        uint32_t walkingInterval{};

        /**
         * The maximum number of steps this mob may take while walking.
         */
        uint8_t walkingStep{};

        /**
         * The interval at which to process running.
         */
        uint32_t runningInterval{};

        /**
         * The maximum number of steps this mob may take while running.
         */
        uint8_t runningStep{};
    } PACKED;
}