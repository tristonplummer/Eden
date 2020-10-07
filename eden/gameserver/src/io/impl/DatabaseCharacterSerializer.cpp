#include <shaiya/game/io/impl/DatabaseCharacterSerializer.hpp>
#include <glog/logging.h>

using namespace shaiya::game;

/**
 * Initialises this character serializer.
 * @param db    The database service.
 */
DatabaseCharacterSerializer::DatabaseCharacterSerializer(shaiya::database::DatabaseService& db): db_(db)
{
}

/**
 * Loads a player character.
 * @param character The character to load.
 */
void DatabaseCharacterSerializer::load(Character& character)
{
    LOG(INFO) << "Load " << character.id();
}

/**
 * Saves a player character.
 * @param character The character to save.
 */
void DatabaseCharacterSerializer::save(Character& character)
{
}