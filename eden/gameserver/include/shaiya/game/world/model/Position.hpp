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
        /**
         * Gets the map id.
         * @return  The map id.
         */
        [[nodiscard]] uint16_t map() const
        {
            return map_;
        }

        /**
         * Get the position along the x-axis.
         * @return  The x-coordinate
         */
        [[nodiscard]] float x() const
        {
            return x_;
        }

        /**
         * Get the position along the vertical axis.
         * @return  The y-coordinate
         */
        [[nodiscard]] float y() const
        {
            return y_;
        }

        /**
         * Get the position along the z-axis.
         * @return  The z-coordinate
         */
        [[nodiscard]] float z() const
        {
            return z_;
        }

    private:
        /**
         * The map id of this position.
         */
        uint16_t map_{ 0 };

        /**
         * Represents the position along the x-axis
         */
        float x_{ 0 };

        /**
         * Represents the position along the vertical axis.
         */
        float y_{ 0 };

        /**
         * Represents the position along the z-axis
         */
        float z_{ 0 };
    };
}