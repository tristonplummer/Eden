#include <shaiya/game/io/impl/DatabaseCharacterSerializer.hpp>

#include <glog/logging.h>

using namespace shaiya::game;

/**
 * The name of the query for loading a character's details.
 */
constexpr auto LOAD_CHARACTER_DETAILS = "load_character_details";

/**
 * Initialises this character serializer.
 * @param db        The database service.
 * @param worldId   The id of this world server.
 */
DatabaseCharacterSerializer::DatabaseCharacterSerializer(shaiya::database::DatabaseService& db, size_t worldId)
    : db_(db), worldId_(worldId)
{
    db.prepare(LOAD_CHARACTER_DETAILS, "SELECT * FROM gamedata.characters WHERE world = $1 AND charid = $2;");
}

/**
 * Loads a player character.
 * @param character The character to load.
 */
bool DatabaseCharacterSerializer::load(Character& character)
{
    try
    {
        // Create a new connection to the database
        auto connection = db_.connection();
        pqxx::work tx(*connection);

        // Fetch the character details
        auto rows = tx.exec_prepared(LOAD_CHARACTER_DETAILS, worldId_, character.id());
        if (rows.empty())
            return false;

        // The details row
        auto row = rows.front();

        // Set the character name, race and class
        character.setName(row["name"].as<std::string>());
        character.setRace(static_cast<ShaiyaRace>(row["race"].as<size_t>()));
        character.setJob(static_cast<ShaiyaClass>(row["class"].as<size_t>()));

        // Set the character appearance
        auto& appearance = character.appearance();
        appearance.setFace(row["face"].as<size_t>());
        appearance.setHair(row["hair"].as<size_t>());
        appearance.setHeight(row["height"].as<size_t>());
        appearance.setGender(static_cast<ShaiyaGender>(row["gender"].as<size_t>()));
        return true;
    }
    catch (const std::exception& e)
    {
        LOG(INFO) << "Exception occurred while loading character with id " << character.id() << ": " << e.what();
    }
    return false;
}

/**
 * Saves a player character.
 * @param character The character to save.
 */
void DatabaseCharacterSerializer::save(Character& character)
{
}