#pragma once
#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/game/service/CharacterScreenService.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/service/WorldApiService.hpp>

#include <boost/property_tree/ptree.hpp>

namespace shaiya::game
{
    /**
     * Handles the initialisation and querying of the various services used
     * throughout the game server.
     */
    class ServiceContext
    {
    public:
        /**
         * Initialises this context.
         * @param config    The configuration tree
         */
        explicit ServiceContext(boost::property_tree::ptree& config);

        /**
         * Gets the character screen service.
         * @return  The character screen.
         */
        CharacterScreenService& getCharScreen();

        /**
         * Gets the api service.
         * @return  The api service.
         */
        WorldApiService& getApiService();

        /**
         * Gets the game world.
         * @return  The game world.
         */
        GameWorldService& getGameWorld();

    private:
        /**
         * The database service instance.
         */
        shaiya::database::DatabaseService* dbService_;

        /**
         * The character screen service.
         */
        CharacterScreenService* charScreen_;

        /**
         * The api service, which is used by the login server.
         */
        WorldApiService* apiService_;

        /**
         * The game world service.
         */
        GameWorldService* gameService_;
    };
}