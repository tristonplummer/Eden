#include <shaiya/common/client/item/ItemSData.hpp>
#include <shaiya/common/util/Async.hpp>
#include <shaiya/game/io/impl/DatabasePlayerSerializer.hpp>
#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/npc/Npc.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/item/GroundItem.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/scheduling/impl/NpcMovementTask.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/sync/ParallelClientSynchronizer.hpp>

#include <chrono>
#include <iostream>
#include <yaml-cpp/yaml.h>

using namespace shaiya::game;

/**
 * Initialise this game world service.
 * @param db            The database service.
 * @param worldId       The id of this world service.
 */
GameWorldService::GameWorldService(shaiya::database::DatabaseService& db, size_t worldId): db_(db)
{
    itemDefs_         = shaiya::client::ItemSData("./data/game/Item.SData");
    mobDefs_          = std::make_unique<MobDefinitionService>(db_);
    synchronizer_     = std::make_unique<ParallelClientSynchronizer>();
    playerSerializer_ = std::make_unique<DatabasePlayerSerializer>(db_, itemDefs_, worldId);

    // Initialise the mobs container
    mobs_ = EntityContainer<Mob>(16384);
}

/**
 * Loads the game world service.
 * @param config    The configuration instance.
 */
void GameWorldService::load(boost::property_tree::ptree& config)
{
    mapRepository_.load(config.get<std::string>("World.MapFilePath"), *this);  // Load the game's maps.
    loadBasePlayerDefinitions(config.get<std::string>("World.BasePlayerDefsPath"));
    loadPlayerLevelDefinitions(config.get<std::string>("World.LevelDefsPath"));
    loadPlayerExperienceDefinitions(config.get<std::string>("World.ExpDefsPath"));

    // Global tasks
    schedule(std::make_shared<NpcMovementTask>());
}

/**
 * Loads the base player definitions.
 * @param path  The path to the config file.
 */
void GameWorldService::loadBasePlayerDefinitions(const std::string& path)
{
    auto yaml = YAML::LoadFile(path);
    for (const auto& node: yaml["definitions"])
    {
        auto def = node["definition"];
        auto job = def["class"].as<std::string>();

        auto hitpoints      = def["hitpoints"];
        auto baseHitpoints  = hitpoints["base"].as<int>();
        auto deltaHitpoints = hitpoints["delta"].as<int>();

        auto mana      = def["mana"];
        auto baseMana  = mana["base"].as<int>();
        auto deltaMana = mana["delta"].as<int>();

        auto stamina      = def["stamina"];
        auto baseStamina  = stamina["base"].as<int>();
        auto deltaStamina = stamina["delta"].as<int>();

        ShaiyaClass jobValue = ShaiyaClass::Fighter;
        if (job == "Fighter")
            jobValue = ShaiyaClass::Fighter;
        else if (job == "Defender")
            jobValue = ShaiyaClass::Defender;
        else if (job == "Ranger")
            jobValue = ShaiyaClass::Ranger;
        else if (job == "Archer")
            jobValue = ShaiyaClass::Archer;
        else if (job == "Mage")
            jobValue = ShaiyaClass::Mage;
        else if (job == "Priest")
            jobValue = ShaiyaClass::Priest;
        basePlayerDefs_[jobValue] = BasePlayerDefinition{ .hitpoints      = baseHitpoints,
                                                          .hitpointsDelta = deltaHitpoints,
                                                          .mana           = baseMana,
                                                          .manaDelta      = deltaMana,
                                                          .stamina        = baseStamina,
                                                          .staminaDelta   = deltaStamina };
    }
}

/**
 * Gets the base player definition.
 * @param job   The class.
 * @param level The level.
 * @return      The base definition.
 */
std::tuple<int, int, int> GameWorldService::getBasePlayerDefinition(shaiya::ShaiyaClass job, int16_t level) const
{
    auto base = basePlayerDefs_.at(job);
    return { (base.hitpoints + (base.hitpointsDelta * level)), (base.mana + (base.manaDelta * level)),
             (base.stamina + (base.staminaDelta * level)) };
}

/**
 * Loads the level definitions for players.
 * @param path  The path to the config file.
 */
void GameWorldService::loadPlayerLevelDefinitions(const std::string& path)
{
    auto yaml = YAML::LoadFile(path);
    for (const auto& node: yaml["definitions"])
    {
        auto def         = node["definition"];
        auto gameMode    = def["mode"].as<std::string>();
        auto maxLevel    = def["maxlevel"].as<int>();
        auto statpoints  = def["statpoints"].as<int>();
        auto skillpoints = def["skillpoints"].as<int>();

        auto mode = ShaiyaGameMode::Easy;
        if (gameMode == "Normal")
            mode = ShaiyaGameMode::Normal;
        else if (gameMode == "Hard")
            mode = ShaiyaGameMode::Hard;
        else if (gameMode == "Ultimate")
            mode = ShaiyaGameMode::Ultimate;

        playerLevelDefs_[mode] = { maxLevel, statpoints, skillpoints };
    }
}

/**
 * Gets the level definition for a player of a given mode.
 * @param mode  The game mode.
 * @return      The max level, the statpoints to add per level, and the skillpoints to add per level.
 */
std::tuple<int, int, int> GameWorldService::getPlayerLevelDefinition(shaiya::ShaiyaGameMode mode) const
{
    return playerLevelDefs_.at(mode);
}

/**
 * Loads the experience definitions for players.
 * @param path  The path to the config file.
 */
void GameWorldService::loadPlayerExperienceDefinitions(const std::string& path)
{
    auto yaml = YAML::LoadFile(path);
    for (const auto& node: yaml["definitions"])
    {
        auto def   = node["definition"];
        auto level = def["level"].as<int>();

        for (const auto& levelDef: def["values"])
        {
            auto gameMode   = levelDef["mode"].as<std::string>();
            auto experience = levelDef["experience"].as<int>();

            auto mode = ShaiyaGameMode::Easy;
            if (gameMode == "Normal")
                mode = ShaiyaGameMode::Normal;
            else if (gameMode == "Hard")
                mode = ShaiyaGameMode::Hard;
            else if (gameMode == "Ultimate")
                mode = ShaiyaGameMode::Ultimate;

            auto idx                   = (level * 1000) + static_cast<int>(mode);
            playerExperienceDefs_[idx] = experience;
        }
    }
}

/**
 * Gets the required experience for a specific game mode and level
 * @param mode      The game mode.
 * @param level     The level.
 * @return          The required experience.
 */
int GameWorldService::getRequiredExperience(shaiya::ShaiyaGameMode mode, uint16_t level) const
{
    auto idx = (level * 1000) + static_cast<int>(mode);
    if (playerExperienceDefs_.contains(idx))
        return playerExperienceDefs_.at(idx);
    return 0;
}

/**
 * Gets the total experience required for a specific game mode's level.
 * @param level     The level.
 * @return          The sum of all experience values.
 */
int GameWorldService::getTotalExperience(shaiya::ShaiyaGameMode mode, uint16_t level) const
{
    int sum = 0;
    if (level <= 0)
        return sum;

    for (auto lvl = 1; lvl <= level; lvl++)
    {
        sum += getRequiredExperience(mode, lvl);
    }
    return sum;
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
        auto start    = steady_clock::now();

        // Finalise the registration and unregistrations for characters
        finaliseRegistrations();
        finaliseUnregistrations();

        // Process all the queued incoming packets
        for (auto&& player: players_)
            player->session().processQueue();

        // Process the map tick
        mapRepository_.tick();

        // Pulse the game world
        scheduler_.pulse(*this);

        // Synchronize the characters with the world state
        synchronizer_->synchronize(players_, npcs_, mobs_);

        // The current time
        auto now  = steady_clock::now();
        auto diff = (now - start);
        // LOG(INFO) << "Tick took " << duration_cast<milliseconds>(diff).count();

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
void GameWorldService::unregisterItem(const std::shared_ptr<GroundItem>& item)
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
void GameWorldService::registerNpc(const std::shared_ptr<Npc>& npc)
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Add the npc to the npc container
    if (npcs_.add(npc))
    {
        npc->init();
        npc->activate();
    }
}

/**
 * Removes an npc from this world.
 * @param item  The npc instance.
 */
void GameWorldService::unregisterNpc(const std::shared_ptr<Npc>& npc)
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
 * Registers a mob to this world.
 * @param mob   The mob instance.
 */
void GameWorldService::registerMob(const std::shared_ptr<Mob>& mob)
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Add the mob to the mobs container
    if (mobs_.add(mob))
    {
        mob->init();
        mob->activate();
    }
}

/**
 * Removes a mob from this world.
 * @param mob   The mob instance.
 */
void GameWorldService::unregisterMob(const std::shared_ptr<Mob>& mob)
{
    // Lock the mutex
    std::lock_guard lock{ mutex_ };

    // Remove the mob
    mobs_.remove(mob);

    // Deactivate the mob
    mob->deactivate();

    // Remove the entity from their map
    auto map = mapRepository_.forId(mob->position().map());
    map->remove(mob);
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