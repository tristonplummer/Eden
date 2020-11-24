#include <shaiya/game/model/actor/Actor.hpp>
#include <shaiya/game/model/actor/movement/path/strategy/SimplePathfindingStrategy.hpp>

#include <cmath>

using namespace shaiya::game;

/**
 * Initialises this movement queue.
 * @param actor The actor to handle.
 */
MovementQueue::MovementQueue(Actor& actor): actor_(actor)
{
}

/**
 * Ticks this movement queue.
 */
void MovementQueue::tick()
{
    // Handle following
    if (target_)
    {
        if (!waypoints_.empty())
        {
            auto& dest = waypoints_.back();
            auto& pos  = actor_.position();
            if (dest.x() == pos.x() || dest.z() == pos.z())
                follow(target_);
        }
        else
        {
            follow(target_);
        }
    }

    // Handle waypoints
    if (waypoints_.empty())
        return;
    auto& waypoint = waypoints_.front();

    // If the actor is at the waypoint, remove it, else move to the waypoint
    if (actor_.position().x() == waypoint.x() && actor_.position().z() == waypoint.z())
    {
        waypoints_.pop_front();
    }

    // Move to the waypoint
    actor_.setPosition(waypoint);
}

/**
 * Add a destination to move to.
 * @param destination   The destination.
 */
void MovementQueue::moveTo(const Position& destination)
{
    auto isRunning = running();
    reset();

    running_ = isRunning;
    createRoute(destination);
}

/**
 * Add a destination to move to.
 * @param destination   The destination.
 * @param radius        The radius to stop before the destination
 */
void MovementQueue::runTo(const Position& destination, size_t radius)
{
    reset();
    running_ = true;

    createRoute(destination, radius);
}

/**
 * Add a destination to move to.
 * @param destination   The destination.
 * @param radius        The radius to stop before the destination
 */
void MovementQueue::walkTo(const Position& destination, size_t radius)
{
    reset();
    running_ = false;

    createRoute(destination, radius);
}

/**
 * Creates the route to a destination.
 * @param destination   The destination
 * @param radius        The radius to stop before the destination
 */
void MovementQueue::createRoute(const Position& destination, size_t radius)
{
    // The start position
    auto& start = actor_.position();

    // The path request
    PathRequest request;
    request.start             = start;
    request.destination       = destination;
    request.running           = running_;
    request.sourceRadius      = actor_.size();
    request.destinationRadius = radius;

    // Calculate the route to take.
    auto strategy = std::make_unique<SimplePathfindingStrategy>();
    auto route    = strategy->calculateRoute(request);

    // Add the route waypoints
    for (auto&& waypoint: route.waypoints)
        addWaypoint(waypoint);
}

/**
 * Gets the movement delay for moving to a specific waypoint.
 * @param waypoint  The waypoint.
 * @return          The delay, in milliseconds
 */
std::chrono::milliseconds MovementQueue::movementDelay(const Position& waypoint)
{
    return std::chrono::milliseconds(1000);
}

/**
 * Adds a waypoint.
 * @param waypoint  The waypoint.
 */
void MovementQueue::addWaypoint(const Position& waypoint)
{
    waypoints_.push_back(waypoint);
}

/**
 * Follows a target.
 * @param target    The target.
 */
void MovementQueue::follow(std::shared_ptr<Actor> target)
{
    reset();
    auto& dest = target->position();
    runTo(dest, target->size());

    target_ = std::move(target);
}

/**
 * Resets the movement queue.
 */
void MovementQueue::reset()
{
    target_          = nullptr;                           // Reset the follow target.
    waypoints_       = std::deque<Position>();            // Clear the queue of waypoints.
    nextMovementTime = std::chrono::steady_clock::now();  // Allow the next movement to process straight away
    running_         = true;
}