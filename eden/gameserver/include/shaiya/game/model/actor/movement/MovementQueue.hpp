#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/model/actor/Actor.hpp>

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
         * If the actor is running to the waypoint
         */
        bool running_{ false };
    };
}