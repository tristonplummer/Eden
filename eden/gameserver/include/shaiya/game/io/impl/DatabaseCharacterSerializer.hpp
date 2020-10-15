#pragma once
#include <shaiya/game/Forward.hpp>
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
         * @param db            The database service.
         * @param itemService   The item definition service.
         * @param worldId       The id of this world server.
         */
        DatabaseCharacterSerializer(shaiya::database::DatabaseService& db, ItemDefinitionService& itemService,
                                    size_t worldId);

        /**
         * Loads a player character.
         * @param character The character to load.
         * @return          If the character was loaded successfully.
         */
        bool load(Character& character) override;

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

        /**
         * The item definition service.
         */
        ItemDefinitionService& itemService_;

        /**
         * The id of this world server.
         */
        size_t worldId_;

        /**
         * Loads the inventory of this character.
         * @param character The character.
         * @return          If the inventory was successfully loaded.
         */
        bool loadInventory(Character& character);

        /**
         * Loads the equipment of this character.
         * @param character The character.
         * @return          If the equipment was successfully loaded.
         */
        bool loadEquipment(Character& character);
    };
}