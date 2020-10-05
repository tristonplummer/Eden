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
         * Default initialise everything to 0.
         */
        Position() = default;

        /**
         * Creates a position from a set of coordinates.
         * @param map   The map id.
         * @param x     The x coordinate.
         * @param y     The y coordinate.
         * @param z     The z coordinate.
         */
        Position(uint16_t map, float x, float y, float z);

        /**
         * Checks if a position is within distance of another.
         * @param other     The other position.
         * @param distance  The distance.
         * @return          If the other position is in distance of this position.
         */
        [[nodiscard]] bool isWithinDistance(const Position& other, float distance) const;

        /**
         * Checks if a position is within viewport distance of another.
         * @param other     The other position.
         * @return          If the other position is in viewport distance of this position.
         */
        [[nodiscard]] bool isWithinViewportDistance(const Position& other) const;

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

        /**
         * Checks if a position is equal to this one.
         * @param other The other position.
         * @return      If the two positions are equal.
         */
        bool operator==(const Position& other) const;

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