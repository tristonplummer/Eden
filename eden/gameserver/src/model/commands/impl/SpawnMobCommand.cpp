#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/commands/impl/SpawnMobCommand.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

#include <glog/logging.h>

#include <memory>

using namespace shaiya::game;

/**
 * Handles the execution of this command.
 * @param character     The character that executed this command.
 * @param args          The command arguments
 */
void SpawnMobCommand::execute(Player& player, const std::vector<std::string>& args)
{
    if (args.size() != 1)
        return;

    auto id     = std::stoi(args.at(0));
    auto& world = player.world();
    auto def    = world.mobDefs().forId(id);

    if (!def)
        return;

    auto mob = std::make_shared<Mob>(*def, Area{ player.position(), player.position() }, world, false);
    mob->setPosition(player.position());

    world.registerMob(mob);
}

/**
 * Gets the identifier of the command.
 * @return  The identifier.
 */
const std::string SpawnMobCommand::identifier() const
{
    return "mob";
}