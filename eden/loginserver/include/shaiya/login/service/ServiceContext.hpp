#pragma once
#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/login/service/AuthenticationService.hpp>
#include <shaiya/login/service/EncryptionService.hpp>
#include <shaiya/login/service/WorldService.hpp>

namespace shaiya::login
{
    /**
     * Handles the initialisation and querying of the various services used
     * throughout the login server.
     */
    class ServiceContext
    {
    public:
        /**
         * Initialises this context.
         */
        ServiceContext();

        /**
         * Gets the encryption service.
         * @return  The encryption service.
         */
        EncryptionService& getEncryptionService();

        /**
         * Gets the world service.
         * @return  The world service.
         */
        WorldService& getWorldService();

        /**
         * Gets the authentication service.
         * @return  The authentication service.
         */
        AuthenticationService& getAuthService();

    private:
        /**
         * The encryption service instance.
         */
        EncryptionService* encryptionService_;

        /**
         * The world service instance.
         */
        WorldService* worldService_;

        /**
         * The authentication service instance
         */
        AuthenticationService* authService_;

        /**
         * The database service instance.
         */
        shaiya::database::DatabaseService* dbService_;
    };
}