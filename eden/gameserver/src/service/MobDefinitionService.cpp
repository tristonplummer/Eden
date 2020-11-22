#include <shaiya/game/service/MobDefinitionService.hpp>

using namespace shaiya::game;
using namespace shaiya::database;

/**
 * Initialises the mob definition service.
 * @param db    The database service.
 */
MobDefinitionService::MobDefinitionService(DatabaseService& db)
{
    // Get the mob definitions from the database
    auto connection = db.connection();
    pqxx::work tx(*connection);
    auto rows = tx.exec("SELECT * FROM gamedefs.mobs");

    // Loop over the rows that were returned from the database
    for (auto&& row: rows)
    {
        auto* def             = new MobDefinition{};
        def->id               = row["id"].as<size_t>();
        def->name             = row["name"].as<std::string>();
        def->level            = row["level"].as<size_t>();
        def->experience       = row["experience"].as<size_t>();
        def->hitpoints        = row["hitpoints"].as<size_t>();
        def->stamina          = row["stamina"].as<size_t>();
        def->mana             = row["mana"].as<size_t>();
        def->dexterity        = row["dexterity"].as<size_t>();
        def->wisdom           = row["wisdom"].as<size_t>();
        def->luck             = row["luck"].as<size_t>();
        def->defense          = row["defense"].as<size_t>();
        def->resistance       = row["resistance"].as<size_t>();
        def->element          = row["element"].as<size_t>();
        def->size             = row["size"].as<size_t>();
        def->aggressionRadius = row["aggroradius"].as<size_t>();
        
        definitions_[def->id] = def;
    }
}

/**
 * Gets the mob definition for a given id.
 * @param id    The mob id.
 * @return      The mob definition.
 */
const MobDefinition* MobDefinitionService::forId(uint32_t id) const
{
    if (!definitions_.contains(id))
        return nullptr;
    return definitions_.at(id);
}