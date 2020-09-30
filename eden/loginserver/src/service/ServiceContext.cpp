#include <shaiya/login/service/ServiceContext.hpp>

#include <cassert>

using namespace shaiya::login;

/**
 * Initialises this context.
 */
ServiceContext::ServiceContext()
{
    encryptionService_ = new EncryptionService();
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