#pragma once
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

    private:
        /**
         * The encryption service instance.
         */
        EncryptionService* encryptionService_;

        /**
         * The world service instance.
         */
        WorldService* worldService_;
    };
}