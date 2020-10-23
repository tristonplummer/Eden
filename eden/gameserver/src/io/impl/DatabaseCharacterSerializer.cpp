#include <shaiya/common/client/item/ItemSData.hpp>
#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/game/io/impl/DatabaseCharacterSerializer.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>
#include <shaiya/game/world/model/item/Item.hpp>

#include <glog/logging.h>

using namespace shaiya::database;
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
 * The name of the query for saving a character's details.
 */
constexpr auto SAVE_CHARACTER_DETAILS = "save_character_details";

/**
 * Initialises this character serializer.
 * @param db            The database service.
 * @param itemService   The item definition service.
 * @param worldId       The id of this world server.
 */
DatabaseCharacterSerializer::DatabaseCharacterSerializer(DatabaseService& db, shaiya::client::ItemSData& itemDefs,
                                                         size_t worldId)
    : db_(db), itemDefs_(itemDefs), worldId_(worldId)
{
    db.prepare(LOAD_CHARACTER_DETAILS, "SELECT * FROM gamedata.characters WHERE world = $1 AND charid = $2;");
    db.prepare(LOAD_CHARACTER_INVENTORY, "SELECT * FROM gamedata.read_character_inventory($1, $2);");
    db.prepare(LOAD_CHARACTER_EQUIPMENT, "SELECT * FROM gamedata.read_character_equipment($1, $2);");
    db.prepare(SAVE_CHARACTER_DETAILS,
               "UPDATE gamedata.characters SET map = $3, posX = $4, posY = $5, posZ = $6, statpoints = $7, strength = $8, "
               "dexterity = $9, reaction = $10, intelligence = $11, wisdom = $12, luck = $13, hitpoints = $14, mana = $15, "
               "stamina = $16 WHERE world = $1 "
               "AND charid = $2;");
}

/**
 * Loads a player character.
 * @param character The character to load.
 */
bool DatabaseCharacterSerializer::load(Player& character)
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
        character.setStatpoints(row["statpoints"].as<size_t>());

        // Set the character appearance
        auto& appearance = character.appearance();
        appearance.setFace(row["face"].as<size_t>());
        appearance.setHair(row["hair"].as<size_t>());
        appearance.setHeight(row["height"].as<size_t>());
        appearance.setGender(static_cast<ShaiyaGender>(row["gender"].as<size_t>()));

        // Build the position
        auto map  = row["map"].as<size_t>();
        auto posX = row["posx"].as<float>();
        auto posY = row["posy"].as<float>();
        auto posZ = row["posz"].as<float>();
        character.setPosition({ static_cast<uint16_t>(map), posX, posY, posZ });

        if (!loadInventory(character))
            return false;
        if (!loadEquipment(character))
            return false;

        // Set the base stats
        auto& stats = character.stats();
        stats.setBase(Stat::Strength, row["strength"].as<size_t>());
        stats.setBase(Stat::Dexterity, row["dexterity"].as<size_t>());
        stats.setBase(Stat::Reaction, row["reaction"].as<size_t>());
        stats.setBase(Stat::Intelligence, row["intelligence"].as<size_t>());
        stats.setBase(Stat::Wisdom, row["wisdom"].as<size_t>());
        stats.setBase(Stat::Luck, row["luck"].as<size_t>());
        stats.sync();
        stats.setHitpoints(row["hitpoints"].as<size_t>());
        stats.setMana(row["mana"].as<size_t>());
        stats.setStamina(row["stamina"].as<size_t>());

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
bool DatabaseCharacterSerializer::loadInventory(Player& character)
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

            // The item definition
            auto* def = itemDefs_.forId(id);
            if (!def)
                continue;

            // The item instance
            auto item = std::make_shared<Item>(*def);
            item->setQuantity(count);

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
bool DatabaseCharacterSerializer::loadEquipment(Player& character)
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

            // The item definition
            auto* def = itemDefs_.forId(id);
            if (!def)
                continue;

            // Add the item
            equipment.add(std::make_shared<Item>(*def), static_cast<EquipmentSlot>(slot));
        }
        return true;
    }
    catch (const std::exception& e)
    {
        LOG(INFO) << "Exception occurred while loading equipment for character with id " << character.id() << ": "
                  << e.what();
    }
    return false;
}

/**
 * Saves a player character.
 * @param character The character to save.
 */
void DatabaseCharacterSerializer::save(Player& character)
{
    try
    {
        // Create a new connection to the database
        auto connection = db_.connection();
        pqxx::work tx(*connection);

        // The character's position
        auto& pos = character.position();

        // The character's stats
        auto& stats = character.stats();

        // Save the details
        tx.exec_prepared(SAVE_CHARACTER_DETAILS, worldId_, character.id(), pos.map(), pos.x(), pos.y(), pos.z(),
                         character.statpoints(), stats.getBase(Stat::Strength), stats.getBase(Stat::Dexterity),
                         stats.getBase(Stat::Reaction), stats.getBase(Stat::Intelligence), stats.getBase(Stat::Wisdom),
                         stats.getBase(Stat::Luck), stats.currentHitpoints(), stats.currentMana(), stats.currentStamina());
        tx.commit();
    }
    catch (const std::exception& e)
    {
        LOG(INFO) << "Exception occured while saving character details for id " << character.id() << ": " << e.what();
    }
}