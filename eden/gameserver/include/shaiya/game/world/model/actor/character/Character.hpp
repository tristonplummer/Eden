#pragma once
#include <shaiya/game/world/model/actor/Actor.hpp>

#include <vector>

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
         * Gets the vector of characters that are in this character's viewport.
         * @return  The observed characters.
         */
        [[nodiscard]] std::vector<Character*>& observedCharacters()
        {
            return observedCharacters_;
        }

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

        /**
         * The vector of characters that are in this character's viewport.
         */
        std::vector<Character*> observedCharacters_;
    };
}