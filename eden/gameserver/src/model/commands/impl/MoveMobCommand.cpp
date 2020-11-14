#include <shaiya/game/model/actor/mob/Mob.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/commands/impl/MoveMobCommand.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

#include <glog/logging.h>

#include <memory>

using namespace shaiya::game;

/**
 * Handles the execution of this command.
 * @param character     The character that executed this command.
 * @param args          The command arguments
 */
void MoveMobCommand::execute(Player& player, const std::vector<std::string>& args)
{
    auto range = std::stof(args.at(0));

    if (!player.combat().inCombat())
        return;

    auto victim = player.combat().victim();
    if (victim->type() != EntityType::Mob)
        return;

    auto mob = std::dynamic_pointer_cast<Mob>(victim);
    mob->setPosition(mob->spawnArea().randomPoint(range));
}

/**
 * Gets the identifier of the command.
 * @return  The identifier.
 */
const std::string MoveMobCommand::identifier() const
{
    return "movemob";
}