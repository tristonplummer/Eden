#include <shaiya/login/service/ServiceContext.hpp>

#include <cassert>

using namespace shaiya::login;

/**
 * Initialises this context.
 */
ServiceContext::ServiceContext()
{
    encryptionService_ = new EncryptionService();
    worldService_      = new WorldService();
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