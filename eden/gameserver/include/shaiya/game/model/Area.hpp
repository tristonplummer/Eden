#pragma once
#include <shaiya/game/model/Position.hpp>

#include <random>

namespace shaiya::game
{
    /**
     * Represents an area within a game.
     */
    class Area
    {
    public:
        /**
         * Constructs the area.
         * @param bottomLeft    The bottom left position.
         * @param topRight      The top right position.
         */
        Area(Position bottomLeft, Position topRight);

        /**
         * Gets a random position in this area.
         * @return  The random point.
         */
        [[nodiscard]] Position randomPoint() const;

        /**
         * Gets a random point with this area as the center.
         * @param range The range around the area.
         * @return      The random point.
         */
        [[nodiscard]] Position randomPoint(float range) const;

        /**
         * Gets the distance to a position
         * @param other The position
         * @return      The distance from this area to the position
         */
        [[nodiscard]] float distanceTo(const Position& other) const;

        /**
         * Check if this area contains a position.
         * @param other The position
         * @return      If the position is inside the area
         */
        [[nodiscard]] bool contains(const Position& other) const;

    private:
        /**
         * The bottom left position.
         */
        Position bottomLeft_;

        /**
         * The top right position.
         */
        Position topRight_;
    };
}