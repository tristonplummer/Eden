#include <shaiya/login/service/ServiceContext.hpp>

#include <cassert>

using namespace shaiya::login;

/**
 * Initialises this context.
 * @param config    The configuration tree
 */
ServiceContext::ServiceContext(boost::property_tree::ptree& config)
{
    // The database credentials
    auto dbHost = config.get<std::string>("Database.Host");
    auto dbUser = config.get<std::string>("Database.Host");
    auto dbPass = config.get<std::string>("Database.Host");
    auto dbName = config.get<std::string>("Database.Host");

    // The world api port
    auto worldApiPort = config.get<uint16_t>("Network.WorldApiPort");

    // Initialise the database service
    dbService_         = new shaiya::database::DatabaseService(dbHost, dbName, dbUser, dbPass);
    encryptionService_ = new EncryptionService();
    authService_       = new AuthenticationService(*dbService_);
    worldService_      = new WorldService(*dbService_, worldApiPort);
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
 * Gets the authentication service.
 * @return  The authentication service.
 */
AuthenticationService& ServiceContext::getAuthService()
{
    assert(authService_);
    return *authService_;
}