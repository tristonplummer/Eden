#pragma once
#include <shaiya/game/model/Position.hpp>

#include <cstddef>

namespace shaiya::game
{
    /**
     * Represent a request to find a path between two points.
     */
    struct PathRequest
    {
        /**
         * The start position.
         */
        Position start;

        /**
         * The destination
         */
        Position destination;

        /**
         * The radius of the source actor
         */
        size_t sourceRadius{ 1 };

        /**
         * The radius of the destination actor
         */
        size_t destinationRadius{ 1 };

        /**
         * If we should run to the destination
         */
        bool running{ false };
    };
}