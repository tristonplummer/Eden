#pragma once
#include <shaiya/game/model/commands/Command.hpp>

namespace shaiya::game
{
    /**
     * Represents a command that can be executed by a player.
     */
    class HeightmapCommand: public Command
    {
    public:
        /**
         * Handles the execution of this command.
         * @param character     The character that executed this command.
         * @param args          The command arguments
         */
        void execute(Player& character, const std::vector<std::string>& args) override;

        /**
         * Gets the identifier of the command.
         * @return  The identifier.
         */
        [[nodiscard]] const std::string identifier() const override;
    };
}