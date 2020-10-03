#include <shaiya/game/service/ServiceContext.hpp>

#include <cassert>
#include <thread>

using namespace shaiya::game;

/**
 * Initialises this context.
 * @param config    The configuration tree
 */
ServiceContext::ServiceContext(boost::property_tree::ptree& config)
{
    // The database credentials
    auto dbHost = config.get<std::string>("Database.Host");
    auto dbUser = config.get<std::string>("Database.User");
    auto dbPass = config.get<std::string>("Database.Pass");
    auto dbName = config.get<std::string>("Database.Database");

    // The world api port
    auto worldApiPort = config.get<uint16_t>("Network.WorldApiPort");

    // Initialise the database service
    dbService_  = new shaiya::database::DatabaseService(dbHost, dbName, dbUser, dbPass);
    apiService_ = new WorldApiService();
    charScreen  = new CharacterScreenService(*dbService_);

    // Run the api service on an external thread, as it blocks
    std::thread apiThread(&WorldApiService::start, apiService_, worldApiPort);
    apiThread.detach();
}

/**
 * Gets the character screen service.
 * @return  The character screen.
 */
CharacterScreenService& ServiceContext::getCharScreen()
{
    assert(charScreen_);
    return *charScreen_;
}

/**
 * Gets the api service.
 * @return  The api service.
 */
WorldApiService& ServiceContext::getApiService()
{
    assert(apiService_);
    return *apiService_;
}