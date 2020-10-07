#pragma once
#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/game/io/CharacterSerializer.hpp>
#include <shaiya/game/service/ItemDefinitionService.hpp>
#include <shaiya/game/world/model/map/MapRepository.hpp>
#include <shaiya/game/world/scheduling/Scheduler.hpp>
#include <shaiya/game/world/sync/ParallelClientSynchronizer.hpp>

#include <boost/property_tree/ptree.hpp>

#include <memory>
#include <mutex>
#include <vector>

namespace shaiya::game
{
    class Character;
    class GameWorldService
    {
    public:
        /**
         * Initialise this game world service.
         * @param db            The database service.
         * @param itemService   The item definition service.
         * @param worldId       The id of this world service.
         */
        explicit GameWorldService(shaiya::database::DatabaseService& db, ItemDefinitionService& itemService, size_t worldId);

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
         * Handles the registration of a character to this game world.
         * @param character The character to register.
         */
        void registerCharacter(std::shared_ptr<Character> character);

        /**
         * Removes a character from this game world.
         * @param character The character to remove.
         */
        void unregisterCharacter(std::shared_ptr<Character>& character);

        /**
         * Schedules a task to be executed in the future.
         * @param task  The task.
         */
        void schedule(std::shared_ptr<ScheduledTask> task);

        /**
         * Gets the map repository
         * @return  The map repository
         */
        [[nodiscard]] const MapRepository& maps() const
        {
            return mapRepository_;
        }

    private:
        /**
         * If this service is running.
         */
        bool running_{ true };

        /**
         * The database service.
         */
        shaiya::database::DatabaseService& db_;

        /**
         * A vector containing the characters that are connected to this game world.
         */
        std::vector<std::shared_ptr<Character>> characters_;

        /**
         * The client synchronizer
         */
        std::unique_ptr<ClientSynchronizer> synchronizer_;

        /**
         * The character serializer
         */
        std::unique_ptr<CharacterSerializer> characterSerializer_;

        /**
         * The map repository.
         */
        MapRepository mapRepository_;

        /**
         * The task scheduler
         */
        Scheduler scheduler_;

        /**
         * The mutex to be used for locking access to the actor vectors.
         */
        std::mutex mutex_;
    };
}