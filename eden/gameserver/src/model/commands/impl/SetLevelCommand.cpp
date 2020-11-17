#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/commands/impl/SetLevelCommand.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

#include <glog/logging.h>

#include <memory>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Handles the execution of this command.
 * @param character     The character that executed this command.
 * @param args          The command arguments
 */
void SetLevelCommand::execute(Player& player, const std::vector<std::string>& args)
{
    if (args.size() != 1)
        return;

    auto level = std::stoi(args.at(0));
    player.levelling().setLevel(level, true);
}

/**
 * Gets the identifier of the command.
 * @return  The identifier.
 */
const std::string SetLevelCommand::identifier() const
{
    return "level";
}