#include <shaiya/game/io/impl/DatabaseCharacterSerializer.hpp>
#include <shaiya/game/net/GameSession.hpp>

#include <chrono>

using namespace shaiya::game;

/**
 * Initialise this game world service.
 * @param db            The database service.
 * @param itemService   The item definition service.
 * @param worldId       The id of this world service.
 */
GameWorldService::GameWorldService(shaiya::database::DatabaseService& db, ItemDefinitionService& itemService, size_t worldId)
    : db_(db)
{
    synchronizer_        = std::make_unique<ParallelClientSynchronizer>();
    characterSerializer_ = std::make_unique<DatabaseCharacterSerializer>(db, itemService, worldId);
}

/**
 * Loads the game world service.
 * @param config    The configuration instance.
 */
void GameWorldService::load(boost::property_tree::ptree& config)
{
    mapRepository_.load(config.get<std::string>("World.MapFilePath"));  // Load the game's maps.
}

/**
 * Handles the main tick of the world.
 * @param tickRate  The tick frequency
 */
void GameWorldService::tick(size_t tickRate)
{
    using namespace std::chrono;

    // While this service is running
    while (running_)
    {
        // The time we should sleep until, for the next tick
        auto nextTick = steady_clock::now() + milliseconds(tickRate);

        // Process all the queued incoming packets
        for (auto&& character: characters_)
            character->session().processQueue();

        // Pulse the game world
        scheduler_.pulse();

        // Synchronize the characters with the world state
        synchronizer_->synchronize(characters_);

        // Sleep until the next tick
        std::this_thread::sleep_until(nextTick);
    }
}

/**
 * Handles the registration of a character to this game world.
 * @param character The character to register.
 */
void GameWorldService::registerCharacter(std::shared_ptr<Character> character)
{
    // Load the character
    if (!characterSerializer_->load(*character))
        return character->session().close();

    // Lock the mutex and add the character to the vector
    std::lock_guard lock{ mutex_ };
    characters_.push_back(character);

    // Initialise the character
    character->init();
}

/**
 * Removes a character from this game world.
 * @param character The character to remove.
 */
void GameWorldService::unregisterCharacter(std::shared_ptr<Character>& character)
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Find the character
    auto predicate = [&](auto& element) { return element.get() == character.get(); };
    auto pos       = std::find_if(characters_.begin(), characters_.end(), predicate);
    if (pos != characters_.end())
    {
        // The character instance
        auto deregisteredCharacter = *pos;

        // Deactivate the character
        deregisteredCharacter->deactivate();

        // Save the character
        characterSerializer_->save(*deregisteredCharacter);

        // Remove the character from their map
        auto map = mapRepository_.forId(deregisteredCharacter->position().map());
        map->remove(deregisteredCharacter);

        // Remove the character
        characters_.erase(pos);
    }
}

/**
 * Schedules a task to be executed in the future.
 * @param task  The task.
 */
void GameWorldService::schedule(std::shared_ptr<ScheduledTask> task)
{
    scheduler_.schedule(std::move(task));
}