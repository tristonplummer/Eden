#pragma once
#include <shaiya/login/service/EncryptionService.hpp>

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

    private:
        /**
         * The encryption service instance.
         */
        EncryptionService* encryptionService_;
    };
}