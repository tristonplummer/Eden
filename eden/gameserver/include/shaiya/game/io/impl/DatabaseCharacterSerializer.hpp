#pragma once
#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/game/io/CharacterSerializer.hpp>

namespace shaiya::game
{
    /**
     * Saves and loads player characters against a database.
     */
    class DatabaseCharacterSerializer: public CharacterSerializer
    {
    public:
        /**
         * Initialises this character serializer.
         * @param db    The database service.
         */
        explicit DatabaseCharacterSerializer(shaiya::database::DatabaseService& db);

        /**
         * Loads a player character.
         * @param character The character to load.
         */
        void load(Character& character) override;

        /**
         * Saves a player character.
         * @param character The character to save.
         */
        void save(Character& character) override;

    private:
        /**
         * The database service.
         */
        shaiya::database::DatabaseService& db_;
    };
}