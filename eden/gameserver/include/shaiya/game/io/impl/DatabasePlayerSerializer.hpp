#pragma once
#include <shaiya/game/Forward.hpp>
#include <shaiya/game/io/PlayerSerializer.hpp>

namespace shaiya::game
{
    /**
     * Saves and loads player characters against a database.
     */
    class DatabasePlayerSerializer: public PlayerSerializer
    {
    public:
        /**
         * Initialises this character serializer.
         * @param db            The database service.
         * @param itemDefs      The item definitions.
         * @param worldId       The id of this world server.
         */
        DatabasePlayerSerializer(shaiya::database::DatabaseService& db, shaiya::client::ItemSData& itemDefs,
                                    size_t worldId);

        /**
         * Loads a player character.
         * @param character The character to load.
         * @return          If the character was loaded successfully.
         */
        bool load(Player& player) override;

        /**
         * Saves a player character.
         * @param character The character to save.
         */
        void save(Player& player) override;

    private:
        /**
         * The database service.
         */
        shaiya::database::DatabaseService& db_;

        /**
         * The item definitions.
         */
        shaiya::client::ItemSData& itemDefs_;

        /**
         * The id of this world server.
         */
        size_t worldId_;

        /**
         * Loads the inventory of this character.
         * @param character The character.
         * @return          If the inventory was successfully loaded.
         */
        bool loadInventory(Player& player);

        /**
         * Loads the equipment of this character.
         * @param character The character.
         * @return          If the equipment was successfully loaded.
         */
        bool loadEquipment(Player& player);
    };
}