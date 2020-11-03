#pragma once
#include <shaiya/game/model/Position.hpp>

#include <deque>

namespace shaiya::game
{
    /**
     * Represents the route of waypoints to take.
     */
    struct Route
    {
        /**
         * The waypoints in the route.
         */
        std::deque<Position> waypoints;
    };
}