#pragma once
#include <shaiya/common/db/DatabaseService.hpp>

// Forward declaration of the login session
namespace shaiya::net
{
    class LoginSession;
}

namespace shaiya::login
{
    /**
     * Handles the authentication of login requests.
     */
    class AuthenticationService
    {
    public:
        /**
         * Initialises this service, and prepares the queries to use in the database.
         * @param db    The database service.
         */
        explicit AuthenticationService(shaiya::database::DatabaseService& db);

        /**
         * Processes a login request.
         * @param session   The session that is sending the login request.
         * @param username  The provided username.
         * @param password  The provided password.
         */
        void login(shaiya::net::LoginSession& session, const std::string& username, const std::string& password);

    private:
        /**
         * The database service instance.
         */
        shaiya::database::DatabaseService& db_;
    };
}