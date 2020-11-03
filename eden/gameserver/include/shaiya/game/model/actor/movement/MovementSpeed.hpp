#pragma once
#include <shaiya/common/DataTypes.hpp>

namespace shaiya::game
{
    /**
     * Represents the movement speed of an actor.
     */
    struct MovementSpeed
    {
        /**
         * The interval between walking waypoints.
         */
        uint32_t walkingInterval{};

        /**
         * The number of steps that may be taken each walk cycle.
         */
        uint8_t walkingSteps{};

        /**
         * The interval between running waypoints.
         */
        uint32_t runningInterval{};

        /**
         * The number of steps that may be taken each run cycle.
         */
        uint8_t runningSteps{};
    };
}