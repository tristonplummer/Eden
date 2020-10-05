#pragma once
#include <shaiya/common/db/DatabaseService.hpp>
#include <shaiya/game/world/model/map/MapRepository.hpp>

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
         * @param db    The database service.
         */
        explicit GameWorldService(shaiya::database::DatabaseService& db);

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
        void registerCharacter(const std::shared_ptr<Character>& character);

        /**
         * Removes a character from this game world.
         * @param character The character to remove.
         */
        void unregisterCharacter(std::shared_ptr<Character>& character);

        /**
         * Gets the map repository
         * @return  The map repository
         */
        [[nodiscard]] MapRepository& maps()
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
         * The map repository.
         */
        MapRepository mapRepository_;

        /**
         * The mutex to be used for locking access to the actor vectors.
         */
        std::mutex mutex_;
    };
}