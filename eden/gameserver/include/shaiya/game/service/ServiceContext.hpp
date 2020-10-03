#pragma once
#include <shaiya/common/db/DatabaseService.hpp>
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

    private:
        /**
         * The database service instance.
         */
        shaiya::database::DatabaseService* dbService_;

        /**
         * The api service, which is used by the login server.
         */
        WorldApiService* apiService_;
    };
}