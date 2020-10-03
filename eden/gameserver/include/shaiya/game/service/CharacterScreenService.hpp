#pragma once
#include <shaiya/common/db/DatabaseService.hpp>

// Forward declaration of the game session.
namespace shaiya::net
{
    class GameSession;
}

namespace shaiya::game
{
    /**
     * A service that is responsible for handling character screen operations, such as displaying the character screen,
     * selecting a new faction, and the creation, deletion and restoration of characters.
     */
    class CharacterScreenService
    {
    public:
        /**
         * Initialises the character screen service.
         * @param db    The database service to use.
         */
        explicit CharacterScreenService(shaiya::database::DatabaseService& db);

        /**
         * Displays the character screen for a session.
         * @param session   The session.
         */
        void display(shaiya::net::GameSession& session);

    private:
        /**
         * The database service.
         */
        shaiya::database::DatabaseService& db_;
    };
}