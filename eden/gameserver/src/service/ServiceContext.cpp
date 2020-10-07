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

    // The world id
    auto worldId = config.get<uint32_t>("World.Id");

    // Initialise the database service
    dbService_   = new shaiya::database::DatabaseService(dbHost, dbName, dbUser, dbPass);
    apiService_  = new WorldApiService();
    itemService_ = new ItemDefinitionService(*dbService_);
    charScreen_  = new CharacterScreenService(*dbService_, worldId);
    gameService_ = new GameWorldService(*dbService_, *itemService_, worldId);

    // Load the game world
    gameService_->load(config);

    // Run the api service on an external thread, as it blocks
    std::thread apiThread(&WorldApiService::start, apiService_, worldApiPort);
    apiThread.detach();

    // The game tick should run on an external thread
    auto tickRate = config.get<size_t>("World.TickRate");
    std::thread worldThread(&GameWorldService::tick, gameService_, tickRate);
    worldThread.detach();
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

/**
 * Gets the game world.
 * @return  The game world.
 */
GameWorldService& ServiceContext::getGameWorld()
{
    assert(gameService_);
    return *gameService_;
}