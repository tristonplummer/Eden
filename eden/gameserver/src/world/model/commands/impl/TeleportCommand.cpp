#include <shaiya/common/net/packet/game/CharacterMapTeleport.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>
#include <shaiya/game/world/model/commands/impl/TeleportCommand.hpp>

#include <glog/logging.h>

#include <memory>

using namespace shaiya::game;
using namespace shaiya::net;

/**
 * Handles the execution of this command.
 * @param character     The character that executed this command.
 * @param args          The command arguments
 */
void TeleportCommand::execute(Player& character, const std::vector<std::string>& args)
{
    if (args.size() != 3)
        return;

    auto map = std::stoi(args.at(0));
    auto x   = std::stof(args.at(1));
    auto z   = std::stof(args.at(2));

    Position dest(map, x, character.position().y(), z);

    CharacterMapTeleport teleport;
    teleport.id  = character.id();
    teleport.map = dest.map();
    teleport.x   = dest.x();
    teleport.y   = dest.y();
    teleport.z   = dest.z();

    character.session().write(teleport);
    character.setPosition(dest);
}

/**
 * Gets the identifier of the command.
 * @return  The identifier.
 */
const std::string TeleportCommand::identifier() const
{
    return "teleport";
}