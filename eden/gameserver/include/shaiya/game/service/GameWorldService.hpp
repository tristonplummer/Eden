#pragma once
#include <shaiya/common/client/item/ItemSData.hpp>
#include <shaiya/common/client/mob/MobSData.hpp>
#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/game/io/PlayerSerializer.hpp>
#include <shaiya/game/model/actor/player/definition/BasePlayerDefinition.hpp>
#include <shaiya/game/model/commands/CommandManager.hpp>
#include <shaiya/game/model/map/MapRepository.hpp>
#include <shaiya/game/scheduling/Scheduler.hpp>
#include <shaiya/game/service/MobDefinitionService.hpp>
#include <shaiya/game/sync/ClientSynchronizer.hpp>
#include <shaiya/game/util/EntityContainer.hpp>

#include <boost/property_tree/ptree.hpp>

#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

namespace shaiya::game
{
    class GameWorldService
    {
    public:
        /**
         * Initialise this game world service.
         * @param db            The database service.
         * @param worldId       The id of this world service.
         */
        explicit GameWorldService(shaiya::database::DatabaseService& db, size_t worldId);

        /**
         * Loads the game world service.
         * @param config    The configuration instance.
         */
        void load(boost::property_tree::ptree& config);

        /**
         * Handles the main tick of the world.
         * @param tickRate  The tick frequency
         */
        void tick(size_t tickRate);

        /**
         * Handles the registration of a player to this game world.
         * @param player    The player to register.
         */
        void registerPlayer(std::shared_ptr<Player> player);

        /**
         * Removes a player from this game world.
         * @param player    The player to remove.
         */
        void unregisterPlayer(std::shared_ptr<Player> player);

        /**
         * Registers a ground item to this world.
         * @param item  The ground item instance.
         */
        void registerItem(std::shared_ptr<GroundItem> item);

        /**
         * Removes a ground item from this world.
         * @param item  The ground item instance.
         */
        void unregisterItem(const std::shared_ptr<GroundItem>& item);

        /**
         * Registers an npc to this world.
         * @param npc  The npc instance.
         */
        void registerNpc(const std::shared_ptr<Npc>& npc);

        /**
         * Removes an npc from this world.
         * @param npc  The npc instance.
         */
        void unregisterNpc(const std::shared_ptr<Npc>& npc);

        /**
         * Registers a mob to this world.
         * @param mob   The mob instance.
         */
        void registerMob(const std::shared_ptr<Mob>& mob);

        /**
         * Removes a mob from this world.
         * @param mob   The mob instance.
         */
        void unregisterMob(const std::shared_ptr<Mob>& mob);

        /**
         * Finalises the registration of players that are queued to be registered.
         */
        void finaliseRegistrations();

        /**
         * Finalises the unregistration of players that are queued to be unregistered.
         */
        void finaliseUnregistrations();

        /**
         * Schedules a task to be executed in the future.
         * @param task  The task.
         */
        void schedule(std::shared_ptr<ScheduledTask> task);

        /**
         * Gets the base player definition.
         * @param job   The class.
         * @param level The level.
         * @return      The base definition values.
         */
        [[nodiscard]] std::tuple<int, int, int> getBasePlayerDefinition(shaiya::ShaiyaClass job, int16_t level) const;

        /**
         * Gets the level definition for a player of a given mode.
         * @param mode  The game mode.
         * @return      The max level, the statpoints to add per level, and the skillpoints to add per level.
         */
        [[nodiscard]] std::tuple<int, int, int> getPlayerLevelDefinition(shaiya::ShaiyaGameMode mode) const;

        /**
         * Gets the required experience for a specific game mode and level
         * @param mode      The game mode.
         * @param level     The level.
         * @return          The required experience.
         */
        [[nodiscard]] int getRequiredExperience(shaiya::ShaiyaGameMode mode, uint16_t level) const;

        /**
         * Gets the total experience required for a specific game mode's level.
         * @param level     The level.
         * @return          The sum of all experience values.
         */
        [[nodiscard]] int getTotalExperience(shaiya::ShaiyaGameMode mode, uint16_t level) const;

        /**
         * Gets the map repository
         * @return  The map repository
         */
        [[nodiscard]] const MapRepository& maps() const
        {
            return mapRepository_;
        }

        /**
         * Gets the command manager.
         * @return  The command manager.
         */
        [[nodiscard]] const CommandManager& commands() const
        {
            return commandManager_;
        }

        /**
         * Gets the item data.
         * @return  The item data.
         */
        [[nodiscard]] const shaiya::client::ItemSData& items() const
        {
            return itemDefs_;
        }

        /**
         * Gets the mob data.
         * @return  The mob data.
         */
        [[nodiscard]] const MobDefinitionService& mobDefs() const
        {
            return *mobDefs_;
        }

        /**
         * Gets the mobs that are active in the game world.
         * @return  The mobs.
         */
        [[nodiscard]] const EntityContainer<Mob>& mobs() const
        {
            return mobs_;
        }

    private:
        /**
         * Loads the base player definitions.
         * @param path  The path to the config file.
         */
        void loadBasePlayerDefinitions(const std::string& path);

        /**
         * Loads the level definitions for players.
         * @param path  The path to the config file.
         */
        void loadPlayerLevelDefinitions(const std::string& path);

        /**
         * Loads the experience definitions for players.
         * @param path  The path to the config file.
         */
        void loadPlayerExperienceDefinitions(const std::string& path);

        /**
         * If this service is running.
         */
        bool running_{ true };

        /**
         * The database service.
         */
        shaiya::database::DatabaseService& db_;

        /**
         * The item data.
         */
        shaiya::client::ItemSData itemDefs_;

        /**
         * The mob definitions.
         */
        std::unique_ptr<MobDefinitionService> mobDefs_;

        /**
         * A vector containing the players that are connected to this game world.
         */
        std::vector<std::shared_ptr<Player>> players_;

        /**
         * The players that are pending registration
         */
        std::queue<std::shared_ptr<Player>> newPlayers_;

        /**
         * The players that are pending unregistration
         */
        std::queue<std::shared_ptr<Player>> oldPlayers_;

        /**
         * A container that holds all of the ground items that exist in the world.
         */
        EntityContainer<GroundItem> groundItems_;

        /**
         * A container that holds all of the npcs that exist in the world.
         */
        EntityContainer<Npc> npcs_;

        /**
         * A container that holds all of the mobs that exist in the world.
         */
        EntityContainer<Mob> mobs_;

        /**
         * The client synchronizer
         */
        std::unique_ptr<ClientSynchronizer> synchronizer_;

        /**
         * The player serializer
         */
        std::unique_ptr<PlayerSerializer> playerSerializer_;

        /**
         * The map repository.
         */
        MapRepository mapRepository_;

        /**
         * The task scheduler
         */
        Scheduler scheduler_;

        /**
         * The command manager.
         */
        CommandManager commandManager_;

        /**
         * The mutex to be used for locking access to the actor vectors.
         */
        std::mutex mutex_;

        /**
         * A map of the base player definitions.
         */
        std::map<ShaiyaClass, BasePlayerDefinition> basePlayerDefs_;

        /**
         * A map of the level definitions.
         */
        std::map<ShaiyaGameMode, std::tuple<int, int, int>> playerLevelDefs_;

        /**
         * A map of the experience definitions.
         */
        std::map<int, int> playerExperienceDefs_;
    };
}