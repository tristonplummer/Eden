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
         * If the actor is currently running.
         * @return  If the actor is running
         */
        [[nodiscard]] bool running() const
        {
            return running_;
        }

    private:
        /**
         * Resets the movement queue.
         */
        void reset();

        /**
         * Creates the route to a destination.
         * @param destination   The destination
         */
        void createRoute(const Position& destination);

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