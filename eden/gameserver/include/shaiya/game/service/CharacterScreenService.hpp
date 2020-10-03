#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/common/net/packet/game/CharacterList.hpp>

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
         * @param db        The database service to use.
         * @param worldId   The id of this world server.
         */
        explicit CharacterScreenService(shaiya::database::DatabaseService& db, uint32_t worldId);

        /**
         * Displays the character screen for a session.
         * @param session   The session.
         */
        void display(shaiya::net::GameSession& session);

        /**
         * Sets the faction for a session.
         * @param session   The session.
         * @param faction   The selected faction.
         * @return          If the faction select failed.
         */
        bool setFaction(shaiya::net::GameSession& session, ShaiyaFaction faction);

    private:
        /**
         * Gets the faction for a given session.
         * @param session   The session instance.
         * @return          The session's faction.
         */
        ShaiyaFaction getFaction(shaiya::net::GameSession& session);

        /**
         * Gets the list of characters for a session.
         * @param session   The session instance.
         * @return          The session's characters.
         */
        std::vector<shaiya::net::CharacterListEntry> getCharacters(shaiya::net::GameSession& session);

        /**
         * The database service.
         */
        shaiya::database::DatabaseService& db_;

        /**
         * The id of this world server.
         */
        uint32_t worldId_{ 0 };
    };
}