#include <shaiya/login/service/ServiceContext.hpp>

#include <cassert>

using namespace shaiya::login;

/**
 * Initialises this context.
 */
ServiceContext::ServiceContext()
{
    encryptionService_ = new EncryptionService();
    dbService_         = new DatabaseService("localhost", "shaiya", "cups", "password123");
    worldService_      = new WorldService(*dbService_);
}

/**
 * Gets the encryption service.
 * @return  The encryption service.
 */
EncryptionService& ServiceContext::getEncryptionService()
{
    assert(encryptionService_);
    return *encryptionService_;
}

/**
 * Gets the world service.
 * @return  The world service.
 */
WorldService& ServiceContext::getWorldService()
{
    assert(worldService_);
    return *worldService_;
}

/**
 * Gets the database service.
 * @return  The database service.
 */
DatabaseService& ServiceContext::getDatabaseService()
{
    assert(dbService_);
    return *dbService_;
}