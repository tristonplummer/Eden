#pragma once

#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/login/service/world/WorldServer.hpp>

#include <thread>
#include <vector>

// Forward declaration of the login session
namespace shaiya::net
{
    class LoginSession;
}

namespace shaiya::login
{
    /**
     * A service that handles the query of world server statuses.
     */
    class WorldService
    {
    public:
        /**
         * Initialises this world service.
         * @param db            The database service.
         * @param worldApiPort  The port that the world api services are listening on.
         */
        WorldService(shaiya::database::DatabaseService& db, uint16_t worldApiPort);

        /**
         * Gets a world with a specified id. Returns a null pointer in the event that no world
         * with the specified id exists.
         * @param id    The world id.
         * @return      The world instance.
         */
        WorldServer* getWorld(uint8_t id);

        /**
         * Sends the world list to a session.
         * @param session   The session instance.
         */
        void sendWorldList(shaiya::net::LoginSession& session);

    private:
        /**
         * The vector of world servers to operate on.
         */
        std::vector<WorldServer> worlds_;

        /**
         * If this service is operating.
         */
        bool running_{ true };

        /**
         * The worker thread for running ping requests.
         */
        std::thread thread_;
    };
}