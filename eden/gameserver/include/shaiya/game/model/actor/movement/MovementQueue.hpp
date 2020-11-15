#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/model/actor/Actor.hpp>
#include <shaiya/game/model/actor/movement/MovementSpeed.hpp>

#include <chrono>
#include <queue>

namespace shaiya::game
{
    /**
     * A class used primarily by mobs and npcs, to simulate natural feeling movement in the game world.
     */
    class MovementQueue
    {
    public:
        /**
         * Initialises this movement queue.
         * @param actor The actor to handle.
         */
        explicit MovementQueue(Actor& actor);

        /**
         * Ticks this movement queue.
         */
        void tick();

        /**
         * Add a destination to move to.
         * @param destination   The destination.
         */
        void moveTo(const Position& destination);

        /**
         * Add a destination to move to.
         * @param destination   The destination.
         * @param radius        The radius to stop before the destination
         */
        void runTo(const Position& destination, size_t radius = 1);

        /**
         * Add a destination to move to.
         * @param destination   The destination.
         * @param radius        The radius to stop before the destination
         */
        void walkTo(const Position& destination, size_t radius = 1);

        /**
         * Adds a waypoint.
         * @param waypoint  The waypoint.
         */
        void addWaypoint(const Position& waypoint);

        /**
         * Follows a target.
         * @param target    The target.
         */
        void follow(std::shared_ptr<Actor> target);

        /**
         * Sets the movement speed of this movement queue.
         * @param speed The new speed.
         */
        void setMovementSpeed(MovementSpeed speed);

        /**
         * Resets the movement queue.
         */
        void reset();

        /**
         * Gets the movement speed of this movement queue.
         * @return  The movement speed.
         */
        [[nodiscard]] const MovementSpeed& speed()
        {
            return speed_;
        }

        /**
         * If this movement queue is processing waypoints.
         * @return  If the waypoints queue is populated.
         */
        [[nodiscard]] bool hasWaypoints() const
        {
            return waypoints_.empty();
        }

        /**
         * If the actor is currently running.
         * @return  If the actor is running
         */
        [[nodiscard]] bool running() const
        {
            return running_;
        }

    private:
        /**
         * Creates the route to a destination.
         * @param destination   The destination
         * @param radius        The radius to stop before the destination
         */
        void createRoute(const Position& destination, size_t radius = 1);

        /**
         * Gets the movement delay for moving to a specific waypoint.
         * @param waypoint  The waypoint.
         * @return          The delay, in milliseconds
         */
        std::chrono::milliseconds movementDelay(const Position& waypoint);

        /**
         * The actor to handle movement for.
         */
        Actor& actor_;

        /**
         * A target to follow.
         */
        std::shared_ptr<Actor> target_;

        /**
         * The queue of waypoints.
         */
        std::deque<Position> waypoints_;

        /**
         * The movement speed
         */
        MovementSpeed speed_{};

        /**
         * The next time a movement can be made.
         */
        std::chrono::steady_clock::time_point nextMovementTime{ std::chrono::steady_clock::now() };

        /**
         * If the actor is running to the waypoint
         */
        bool running_{ false };
    };
}