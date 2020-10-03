#pragma once
#include <cstdint>

namespace shaiya::game
{
    /**
     * Represents a position in 3D space.
     */
    class Position
    {
    public:
    private:
        /**
         * The map id of this position.
         */
        uint16_t map_{0};

        /**
         * Represents the position along the x-axis
         */
        float x_{ 0 };

        /**
         * Represents the position along the vertical axis.
         */
        float y_{ 0 };

        /**
         * Represents the position along the y-axis
         */
        float z_{ 0 };
    };
}