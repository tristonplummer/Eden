#pragma once
#include <shaiya/common/db/DatabaseService.hpp>

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
         * The mutex to be used for locking access to the actor vectors.
         */
        std::mutex mutex_;
    };
}