#include <shaiya/common/client/item/ItemSData.hpp>
#include <shaiya/common/util/Async.hpp>
#include <shaiya/game/io/impl/DatabasePlayerSerializer.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/world/model/actor/npc/Npc.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>
#include <shaiya/game/world/model/item/GroundItem.hpp>
#include <shaiya/game/world/model/map/Map.hpp>
#include <shaiya/game/world/sync/ParallelClientSynchronizer.hpp>

#include <chrono>

using namespace shaiya::game;

/**
 * Initialise this game world service.
 * @param db            The database service.
 * @param worldId       The id of this world service.
 */
GameWorldService::GameWorldService(shaiya::database::DatabaseService& db, size_t worldId): db_(db)
{
    itemDefs_            = shaiya::client::ItemSData("./data/game/Item.SData");
    synchronizer_        = std::make_unique<ParallelClientSynchronizer>();
    playerSerializer_    = std::make_unique<DatabasePlayerSerializer>(db, itemDefs_, worldId);
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

        // Finalise the registration and unregistrations for characters
        finaliseRegistrations();
        finaliseUnregistrations();

        // Process all the queued incoming packets
        for (auto&& player: players_)
            player->session().processQueue();

        // Pulse the game world
        scheduler_.pulse();

        // Synchronize the characters with the world state
        synchronizer_->synchronize(players_);

        // The current time
        auto now = steady_clock::now();
        if (now >= nextTick)
        {
            auto difference = duration_cast<milliseconds>(now - nextTick);
            LOG(INFO) << "Game tick took too long - went over " << tickRate << "ms tick rate by " << difference.count()
                      << "ms.";
        }

        // Sleep until the next tick
        std::this_thread::sleep_until(nextTick);
    }
}

/**
 * Handles the registration of a character to this game world.
 * @param character The character to register.
 */
void GameWorldService::registerPlayer(std::shared_ptr<Player> character)
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Add the character to the queue of characters that need to be registered
    newPlayers_.push(std::move(character));
}

/**
 * Removes a character from this game world.
 * @param character The character to remove.
 */
void GameWorldService::unregisterPlayer(std::shared_ptr<Player> character)
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Adds the character to the queue of characters that need to be unregistered
    oldPlayers_.push(std::move(character));
}

/**
 * Registers a ground item to this world.
 * @param item  The ground item instance.
 */
void GameWorldService::registerItem(std::shared_ptr<GroundItem> item)
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Add the item to the ground items container
    if (groundItems_.add(item))
    {
        item->activate();
    }
}

/**
 * Removes a ground item from this world.
 * @param item  The ground item instance.
 */
void GameWorldService::unregisterItem(std::shared_ptr<GroundItem> item)
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Remove the item
    groundItems_.remove(item);

    // Deactivate the item
    item->deactivate();

    // Remove the entity from their map
    auto map = mapRepository_.forId(item->position().map());
    map->remove(item);
}

/**
 * Registers an npc to this world.
 * @param npc  The npc instance.
 */
void GameWorldService::registerNpc(std::shared_ptr<Npc> npc)
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Add the item to the ground items container
    if (npcs_.add(npc))
    {
        npc->activate();
    }
}

/**
 * Removes an npc from this world.
 * @param item  The npc instance.
 */
void GameWorldService::unregisterNpc(std::shared_ptr<Npc> npc)
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Remove the npc
    npcs_.remove(npc);

    // Deactivate the npc
    npc->deactivate();

    // Remove the entity from their map
    auto map = mapRepository_.forId(npc->position().map());
    map->remove(npc);
}

/**
 * Finalises the registration of characters that are queued to be registered.
 */
void GameWorldService::finaliseRegistrations()
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Process the registrations
    while (!newPlayers_.empty())
    {
        auto character = newPlayers_.front();
        newPlayers_.pop();
        players_.push_back(character);

        auto load = [&, character]() {
            playerSerializer_->load(*character);
            character->init();
        };
        ASYNC(load)
    }
}

/**
 * Finalises the unregistration of characters that are queued to be unregistered.
 */
void GameWorldService::finaliseUnregistrations()
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Process the unregistrations
    while (!oldPlayers_.empty())
    {
        // Deactivate the character
        auto character = oldPlayers_.front();
        oldPlayers_.pop();
        character->deactivate();

        // Remove the character from their map
        auto map = mapRepository_.forId(character->position().map());
        map->remove(character);

        auto finalise = [&, character = std::move(character)]() {
            playerSerializer_->save(*character);

            // Find the character
            auto predicate = [&](auto& element) { return element.get() == character.get(); };
            auto pos       = std::find_if(players_.begin(), players_.end(), predicate);

            // Remove the character
            if (pos != players_.end())
                players_.erase(pos);
        };
        ASYNC(finalise)
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