#pragma once
#include <shaiya/game/world/model/actor/Actor.hpp>

namespace shaiya::net
{
    class GameSession;  // Forward declaration of the session.
}

namespace shaiya::game
{
    class GameWorldService;

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
         * Initialises this character.
         */
        void init() override;

        /**
         * Gets the game world service.
         * @return  The game world.
         */
        [[nodiscard]] GameWorldService& world() const;

        /**
         * Gets the session for this character.
         * @return  The session.
         */
        [[nodiscard]] shaiya::net::GameSession& session() const
        {
            return session_;
        }

    private:
        /**
         * The game session instance.
         */
        shaiya::net::GameSession& session_;

        /**
         * The number of unused stat points the character has available to them.
         */
        uint16_t statpoints_{ 0 };

        /**
         * The number of unused skill points the character has available to them.
         */
        uint16_t skillpoints_{ 0 };

        /**
         * The number of kills the character has.
         */
        uint32_t kills_{ 0 };

        /**
         * The number of deaths the character has.
         */
        uint32_t deaths_{ 0 };

        /**
         * The number of victories the character has.
         */
        uint32_t victories_{ 0 };

        /**
         * The number of defeats the character has.
         */
        uint32_t defeats_{ 0 };
    };
}