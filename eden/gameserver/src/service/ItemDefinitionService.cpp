#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/game/service/ItemDefinitionService.hpp>

using namespace shaiya::game;

/**
 * Initialise this item definition service.
 * @param db    The database instance.
 */
ItemDefinitionService::ItemDefinitionService(shaiya::database::DatabaseService& db)
{
    // Get the item definitions from the database
    auto connection = db.connection();
    pqxx::work tx(*connection);
    auto rows = tx.exec("SELECT * FROM gamedefs.items");

    // Loop through the rows
    for (auto&& row: rows)
    {
        // Create the item definition
        auto def = new ItemDefinition();

        // Set the definition values
        def->id           = row["itemid"].as<size_t>();
        def->strength     = row["strength"].as<size_t>();
        def->dexterity    = row["dexterity"].as<size_t>();
        def->reaction     = row["reaction"].as<size_t>();
        def->intelligence = row["intelligence"].as<size_t>();
        def->wisdom       = row["wisdom"].as<size_t>();
        def->luck         = row["luck"].as<size_t>();
        def->name         = row["name"].as<std::string>();

        // Store the item definition
        definitions_[def->id] = def;
    }
}

/**
 * Gets the item definition for a given id.
 * @param id    The item id.
 * @return      The item definition.
 */
const ItemDefinition* ItemDefinitionService::forId(uint32_t id) const
{
    if (!definitions_.contains(id))
        return nullptr;
    return definitions_.at(id);
}

/**
 * Gets the item definition for a given id.
 * @param type      The item type.
 * @param typeId    The item type id.
 * @return          The item definition.
 */
const ItemDefinition* ItemDefinitionService::forId(uint8_t type, uint8_t typeId) const
{
    return forId((type * 1000) + typeId);
}