#pragma once
#include <shaiya/game/world/model/commands/Command.hpp>

#include <map>
#include <memory>

namespace shaiya::game
{
    /**
     * Handles the initialisation and execution of commands.
     */
    class CommandManager
    {
    public:
        /**
         * Initialises the command manager.
         */
        explicit CommandManager();

        /**
         * Attempts to execute a command.
         * @param character The character that is trying to execute a command.
         * @param text      The command text.
         */
        void execute(Player& character, const std::string& text) const;

    private:
        /**
         * Registers a command instance.
         * @param command   The command instance.
         */
        void registerCommand(std::shared_ptr<Command> command);

        /**
         * A map of command identifiers to command instances.
         */
        std::map<std::string, std::shared_ptr<Command>> commands_;
    };
}