#include <shaiya/game/io/impl/DatabaseCharacterSerializer.hpp>

#include <glog/logging.h>

using namespace shaiya::game;

/**
 * The name of the query for loading a character's details.
 */
constexpr auto LOAD_CHARACTER_DETAILS = "load_character_details";

/**
 * The name of the query for loading a character's inventory.
 */
constexpr auto LOAD_CHARACTER_INVENTORY = "load_character_inventory";

/**
 * The name of the query for loading a character's equipment
 */
constexpr auto LOAD_CHARACTER_EQUIPMENT = "load_character_equipment";

/**
 * Initialises this character serializer.
 * @param db        The database service.
 * @param worldId   The id of this world server.
 */
DatabaseCharacterSerializer::DatabaseCharacterSerializer(shaiya::database::DatabaseService& db, size_t worldId)
    : db_(db), worldId_(worldId)
{
    db.prepare(LOAD_CHARACTER_DETAILS, "SELECT * FROM gamedata.characters WHERE world = $1 AND charid = $2;");
    db.prepare(LOAD_CHARACTER_INVENTORY, "SELECT * FROM gamedata.read_character_inventory($1, $2);");
    db.prepare(LOAD_CHARACTER_EQUIPMENT, "SELECT * FROM gamedata.read_character_equipment($1, $2);");
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

        if (!loadInventory(character))
            return false;
        if (!loadEquipment(character))
            return false;

        return true;
    }
    catch (const std::exception& e)
    {
        LOG(INFO) << "Exception occurred while loading character with id " << character.id() << ": " << e.what();
    }
    return false;
}

/**
 * Loads the inventory of this character.
 * @param character The character.
 * @return          If the inventory was successfully loaded.
 */
bool DatabaseCharacterSerializer::loadInventory(Character& character)
{
    try
    {
        // Create a new connection to the database
        auto connection = db_.connection();
        pqxx::work tx(*connection);

        // The character's inventory
        auto& inventory = character.inventory();

        // Fetch the character inventory
        auto rows = tx.exec_prepared(LOAD_CHARACTER_INVENTORY, worldId_, character.id());
        if (rows.empty())
            return true;

        // Loop through the inventory rows
        for (auto&& row: rows)
        {
            // Read the item details
            auto id    = row["itemid"].as<size_t>();
            auto slot  = row["slot"].as<size_t>();
            auto count = row["count"].as<size_t>();

            // The item instance
            auto item = std::make_shared<Item>(id);
            item->setCount(count);

            // Add the item
            inventory.add(std::move(item), slot);
        }
        return true;
    }
    catch (const std::exception& e)
    {
        LOG(INFO) << "Exception occurred while loading inventory for character  with id " << character.id() << ": "
                  << e.what();
    }
    return false;
}

/**
 * Loads the equipment of this character.
 * @param character The character.
 * @return          If the equipment was successfully loaded.
 */
bool DatabaseCharacterSerializer::loadEquipment(Character& character)
{
    try
    {
        // Create a new connection to the database
        auto connection = db_.connection();
        pqxx::work tx(*connection);

        // The character's equipment
        auto& equipment = character.equipment();

        // Fetch the character inventory
        auto rows = tx.exec_prepared(LOAD_CHARACTER_EQUIPMENT, worldId_, character.id());
        if (rows.empty())
            return true;

        // Loop through the equipment rows
        for (auto&& row: rows)
        {
            // Read the item details
            auto id   = row["itemid"].as<size_t>();
            auto slot = row["slot"].as<size_t>();

            // Add the item
            equipment.add(std::make_shared<Item>(id), static_cast<EquipmentSlot>(slot));
        }
        return true;
    }
    catch (const std::exception& e)
    {
        LOG(INFO) << "Exception occurred while loading equipment for character  with id " << character.id() << ": "
                  << e.what();
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