#pragma once
#include <shaiya/common/db/DatabaseService.hpp>

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
         */
        ServiceContext();

    private:

        /**
         * The database service instance.
         */
        shaiya::database::DatabaseService* dbService_;
    };
}