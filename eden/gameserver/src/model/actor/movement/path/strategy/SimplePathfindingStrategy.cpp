#include <shaiya/game/model/actor/movement/path/strategy/SimplePathfindingStrategy.hpp>

using namespace shaiya::game;

/**
 * Calculates a route for a given path request. Here we use a very simple raycasting algorithm to construct a line
 * between two points, and then split the line into a series of waypoints based on the movement distance of the actor.
 * @param request   The path request.
 * @return          The route of waypoints to traverse.
 */
Route SimplePathfindingStrategy::calculateRoute(const PathRequest& request)
{
    // Initialise the waypoints to use as a route
    std::deque<Position> waypoints;
    waypoints.push_back(request.start);

    // The start and destination
    auto& start = request.start;
    auto& dest  = request.destination;

    // The number of steps between each waypoint
    float steps = 3;

    // The distance between the two positions
    auto distance = start.getDistance(dest);
    if (distance < steps)
    {
        waypoints.push_back(dest);
        return { waypoints };
    }

    // The delta positions
    auto dx  = std::abs(start.x() - dest.x());
    auto dz  = std::abs(start.z() - dest.z());
    auto dx2 = 0.0f;
    auto dz2 = 0.0f;

    // The start and destination x and z positions
    auto x1 = start.x();
    auto z1 = start.z();
    auto x2 = dest.x();
    auto z2 = dest.z();

    // The current x and z
    auto x = start.x();
    auto z = start.z();

    // The amount to move each waypoint
    auto sx = (x1 < x2) ? steps : -steps;
    auto sz = (z1 < z2) ? steps : -steps;

    // The previous position
    while (dx2 < dx || dz2 < dz)
    {
        if (dx2 < dx)
        {
            auto d = std::min((dx - dx2), sx);
            dx2 += std::abs(d);
            x += d;
        }

        if (dz2 < dz)
        {
            auto d = std::min((dz - dz2), sz);
            dz2 += std::abs(d);
            z += d;
        }
        waypoints.emplace_back(start.map(), x, start.y(), z);
    }

    // Add the destination as the final waypoint
    waypoints.push_back(dest);
    return { waypoints };
}