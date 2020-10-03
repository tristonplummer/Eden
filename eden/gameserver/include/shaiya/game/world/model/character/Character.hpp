#pragma once
#include <shaiya/game/world/model/Actor.hpp>

namespace shaiya::net
{
    class GameSession;  // Forward declaration of the session.
}

namespace shaiya::game
{
    /**
     * Represents a player character.
     */
    class Character: public Actor
    {
    public:
        /**
         * Creates a character instance from a connected session
         * @param session   The session instance.
         * @param id        The character id.
         */
        Character(shaiya::net::GameSession& session, size_t id);

        /**
         * Sends the startup packets for this character.
         */
        void sendStartup();

        /**
         * Gets the session for this character.
         * @return  The session.
         */
        shaiya::net::GameSession& session() const
        {
            return session_;
        }

    private:
        /**
         * The game session instance.
         */
        shaiya::net::GameSession& session_;
    };
}