#pragma once
#include <shaiya/game/model/actor/movement/path/PathRequest.hpp>
#include <shaiya/game/model/actor/movement/path/Route.hpp>

namespace shaiya::game
{
    /**
     * Represents a strategy that can be used to find a path to a target.
     */
    class PathfindingStrategy
    {
    public:
        /**
         * Calculates a route for a given path request.
         * @param request   The path request.
         * @return          The route of waypoints to traverse.
         */
        virtual Route calculateRoute(const PathRequest& request) = 0;
    };
}