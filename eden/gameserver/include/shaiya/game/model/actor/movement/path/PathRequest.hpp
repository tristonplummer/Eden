#pragma once
#include <shaiya/game/model/Position.hpp>
#include <shaiya/game/model/actor/movement/MovementSpeed.hpp>

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
         * The movement speed
         */
        MovementSpeed speed;

        /**
         * If we should run to the destination
         */
        bool running{ false };
    };
}