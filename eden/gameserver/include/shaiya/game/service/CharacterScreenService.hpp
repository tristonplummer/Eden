#pragma once
#include <shaiya/common/DataTypes.hpp>
#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/common/net/packet/game/CharacterCreation.hpp>
#include <shaiya/common/net/packet/game/CharacterList.hpp>
#include <shaiya/game/Forward.hpp>

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

        /**
         * Attempts to create a new character for the session.
         * @param session   The session instance
         * @param slot      The slot to place the character in
         * @param race      The race of the character
         * @param mode      The game mode of the character
         * @param hair      The hair of the character
         * @param face      The face of the character
         * @param height    The height of the character
         * @param job       The class of the character
         * @param gender    The gender of the character
         * @param name      The name of the character
         * @return          The result of the character creation
         */
        shaiya::net::CharacterCreateResult createCharacter(shaiya::net::GameSession& session, int slot, int race, int mode,
                                                           int hair, int face, int height, int job, int gender,
                                                           std::string name);

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