#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/common/net/packet/game/AccountFaction.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/CharacterScreenService.hpp>

using namespace shaiya;
using namespace shaiya::game;
using namespace shaiya::net;

/**
 * The number of character slots in the character list
 */
constexpr auto CHARACTER_LIST_SIZE = 5;

/**
 * The number of bytes in an empty character list entry (opcode = 2, slot = 1, id = 4)
 */
constexpr auto EMPTY_CHARCTER_LENGTH = 7;

/**
 * The name of the query for fetching the faction for an account
 */
constexpr auto FETCH_ACCOUNT_FACTION = "fetch_account_faction";

/**
 * The name of the query for updating the faction of an account.
 */
constexpr auto UPDATE_ACCOUNT_FACTION = "update_account_faction";

/**
 * The name of the query for fetching the character list
 */
constexpr auto FETCH_CHARACTERS = "fetch_characters";

/**
 * The name of the query for creating a character
 */
constexpr auto CREATE_CHARACTER = "create_character";

/**
 * Initialises the character screen service.
 * @param db        The database service to use.
 * @param worldId   The id of this world server.
 */
CharacterScreenService::CharacterScreenService(shaiya::database::DatabaseService& db, uint32_t worldId)
    : db_(db), worldId_(worldId)
{
    db.prepare(FETCH_ACCOUNT_FACTION, "SELECT faction FROM gamedata.factions WHERE userid = $1 and world = $2");
    db.prepare(UPDATE_ACCOUNT_FACTION, "SELECT gamedata.update_faction($1, $2, $3);");
    db.prepare(FETCH_CHARACTERS, "SELECT * FROM gamedata.get_characters($1, $2);");
    db.prepare(CREATE_CHARACTER,
               "SELECT status FROM gamedata.create_character($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);");
}

/**
 * Displays the character screen for a session.
 * @param session   The session.
 */
void CharacterScreenService::display(GameSession& session)
{
    // The player's faction
    auto faction = getFaction(session);

    // Send the player their faction
    AccountFactionNotify factionNotify;
    factionNotify.faction = faction;
    session.write(factionNotify);

    // If the faction is neither Light nor Fury, don't send the character list
    if (faction == ShaiyaFaction::Neither)
        return;

    // Set the faction for the user
    session.setFaction(faction);

    // Get the list of characters for this session
    auto characters = getCharacters(session);

    // Send the character list
    for (auto&& character: characters)
        session.write(character, character.id ? sizeof(character) : EMPTY_CHARCTER_LENGTH);
}

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
CharacterCreateResult CharacterScreenService::createCharacter(GameSession& session, int slot, int race, int mode, int hair,
                                                              int face, int height, int job, int gender, std::string name)
{
    // If the destination slot is greater than the maximum character slots
    if (slot >= CHARACTER_LIST_SIZE)
        return CharacterCreateResult::Error;

    try
    {
        // Create a new connection to the database
        auto connection = db_.connection();
        pqxx::work tx(*connection);

        // Attempt to create the character
        auto response = tx.exec_prepared(CREATE_CHARACTER, worldId_, session.userId(), slot, race, mode, hair, face, height,
                                         job, gender, name);

        // If nothing was returned, treat it as an error
        if (response.empty())
            return CharacterCreateResult::Error;

        // The table returned by the character creation
        auto row    = response.front();
        auto result = static_cast<CharacterCreateResult>(row.front().as<size_t>());
        if (result == CharacterCreateResult::Success)
            tx.commit();
        return result;
    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << "Exception occurred while creating character for user id " << session.userId() << " from ip address "
                   << session.remoteAddress() << ": " << e.what();
        return CharacterCreateResult::Error;
    }
}

/**
 * Gets the list of characters for a session.
 * @param session   The session instance.
 * @return          The session's characters.
 */
std::vector<CharacterListEntry> CharacterScreenService::getCharacters(GameSession& session)
{
    // Prepare the packets
    std::vector<CharacterListEntry> characters;
    characters.resize(CHARACTER_LIST_SIZE);
    for (auto i = 0; i < characters.size(); i++)
        characters.at(i).slot = i;

    // Attempt to get the character list from the database
    try
    {
        // Create a new connection to the database
        auto connection = db_.connection();
        pqxx::work tx(*connection);

        // Fetch the character rows
        auto rows = tx.exec_prepared(FETCH_CHARACTERS, worldId_, session.userId());

        // Loop through the rows
        for (auto&& row: rows)
        {
            auto slot = row["slot"].as<int>();
            assert(slot <= CHARACTER_LIST_SIZE);

            auto& character        = characters.at(slot);
            character.id           = row["charid"].as<int>();
            character.level        = row["level"].as<int>();
            character.race         = static_cast<ShaiyaRace>(row["race"].as<int>());
            character.mode         = static_cast<ShaiyaGameMode>(row["mode"].as<int>());
            character.hair         = row["hair"].as<int>();
            character.face         = row["face"].as<int>();
            character.height       = row["height"].as<int>();
            character.job          = static_cast<ShaiyaClass>(row["class"].as<int>());
            character.gender       = row["gender"].as<int>();
            character.map          = row["map"].as<int>();
            character.strength     = row["strength"].as<int>();
            character.dexterity    = row["dexterity"].as<int>();
            character.reaction     = row["reaction"].as<int>();
            character.intelligence = row["intelligence"].as<int>();
            character.wisdom       = row["wisdom"].as<int>();
            character.luck         = row["luck"].as<int>();
            character.name         = row["name"].as<std::string>();
        }
    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << "Exception occurred while fetching characters for user id " << session.userId() << " from ip address "
                   << session.remoteAddress() << ": " << e.what();
    }

    return characters;
}

/**
 * Gets the faction for a given session.
 * @param session   The session instance.
 * @return          The session's faction.
 */
ShaiyaFaction CharacterScreenService::getFaction(shaiya::net::GameSession& session)
{
    try
    {
        // Create a new connection to the database
        auto connection = db_.connection();
        pqxx::work tx(*connection);

        // Get the faction for the user on this server
        auto rows = tx.exec_prepared(FETCH_ACCOUNT_FACTION, session.userId(), worldId_);

        // If no faction was found, return a notification for the user to select their faction
        if (rows.empty())
            return ShaiyaFaction::Neither;

        // Get the first row
        auto row = rows.front();

        // Set the faction id
        auto faction = row["faction"].as<int>();
        return static_cast<ShaiyaFaction>(faction);
    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << "Exception occurred while fetching faction for user id " << session.userId() << " from ip address "
                   << session.remoteAddress() << ": " << e.what();
        return ShaiyaFaction::Neither;
    }
}

/**
 * Sets the faction for a session.
 * @param session   The session.
 * @param faction   The selected faction.
 */
bool CharacterScreenService::setFaction(shaiya::net::GameSession& session, ShaiyaFaction faction)
{
    // The faction may only be Light or Fury
    if (faction != ShaiyaFaction::Fury && faction != ShaiyaFaction::Light)
        return false;
    try
    {
        // Create a new connection to the database
        auto connection = db_.connection();
        pqxx::work tx(*connection);

        // Execute the update
        tx.exec_prepared(UPDATE_ACCOUNT_FACTION, worldId_, session.userId(), (int)faction);
        tx.commit();
        return true;
    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << "Exception occurred while setting faction to " << (int)faction << " for session with user id "
                   << session.userId() << " from ip address " << session.remoteAddress() << ": " << e.what();
        return false;
    }
}