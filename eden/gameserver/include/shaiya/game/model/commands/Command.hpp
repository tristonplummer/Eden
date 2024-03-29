#pragma once
#include <shaiya/game/Forward.hpp>

#include <string>
#include <vector>

namespace shaiya::game
{
    /**
     * Represents a command that can be executed by a player.
     */
    class Command
    {
    public:
        /**
         * Handles the execution of this command.
         * @param player        The character that executed this command.
         * @param args          The command arguments
         */
        virtual void execute(Player& player, const std::vector<std::string>& args) = 0;

        /**
         * Gets the identifier of the command.
         * @return  The identifier.
         */
        [[nodiscard]] virtual const std::string identifier() const = 0;
    };
}