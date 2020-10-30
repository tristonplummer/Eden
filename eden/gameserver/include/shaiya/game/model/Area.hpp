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