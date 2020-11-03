#include <shaiya/game/model/actor/Actor.hpp>
#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/movement/MovementQueue.hpp>
#include <shaiya/game/model/actor/movement/path/strategy/SimplePathfindingStrategy.hpp>

using namespace shaiya::game;

/**
 * The distance from the spawn point that a mob should reset.
 */
constexpr auto MobResetDistance = 30.0f;

/**
 * The distance between each waypoint.
 */
constexpr auto WaypointDistance = 3.0f;

/**
 * The actor should run if the next waypoint is far enough away.
 */
constexpr auto RunningDistance = 5.0f;

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
    // If our actor is a mob, we should handle resetting if we move too far from the spawn point
    if (actor_.type() == EntityType::Mob)
    {
        auto& mob = dynamic_cast<Mob&>(actor_);
        if (mob.spawnArea().distanceTo(mob.position()) > MobResetDistance)
        {
            if (mob.combat().inCombat() || target_)
            {
                mob.combat().reset();
                moveTo(mob.spawnArea().randomPoint());
            }
        }
    }

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
    reset();
    createRoute(destination);
}

/**
 * Creates the route to a destination.
 * @param destination   The destination
 */
void MovementQueue::createRoute(const Position& destination)
{
    // The start position
    auto& start = actor_.position();

    // The path request
    PathRequest request;
    request.start       = start;
    request.destination = destination;

    // Calculate the route to take.
    auto strategy = std::make_unique<SimplePathfindingStrategy>();
    auto route    = strategy->calculateRoute(request);

    // Add the route waypoints
    for (auto&& waypoint: route.waypoints)
        addWaypoint(waypoint);
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
    target_ = std::move(target);

    createRoute(target_->position());
}

/**
 * Resets the movement queue.
 */
void MovementQueue::reset()
{
    target_    = nullptr;                 // Reset the follow target.
    waypoints_ = std::deque<Position>();  // Clear the queue of waypoints.
    running_   = false;
}