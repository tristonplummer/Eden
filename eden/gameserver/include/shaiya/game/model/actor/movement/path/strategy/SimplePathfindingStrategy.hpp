#include <shaiya/game/model/actor/movement/path/PathfindingStrategy.hpp>

namespace shaiya::game
{
    /**
     * Represents a simple pathfinding strategy that does no collision detection, to find a direct route between two points.
     */
    class SimplePathfindingStrategy: public PathfindingStrategy
    {
    public:
        /**
         * Calculates a route for a given path request.
         * @param request   The path request.
         * @return          The route of waypoints to traverse.
         */
        Route calculateRoute(const PathRequest& request) override;
    };
}