#include <shaiya/game/service/ServiceContext.hpp>

#include <cassert>

using namespace shaiya::game;

/**
 * Initialises this context.
 */
ServiceContext::ServiceContext()
{
    dbService_ = new shaiya::database::DatabaseService("localhost", "shaiya", "cups", "password123");
}