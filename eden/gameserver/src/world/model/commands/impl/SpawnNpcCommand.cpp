#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/model/actor/npc/Npc.hpp>
#include <shaiya/game/world/model/commands/impl/SpawnNpcCommand.hpp>

#include <glog/logging.h>

#include <memory>

using namespace shaiya::game;

/**
 * Handles the execution of this command.
 * @param character     The character that executed this command.
 * @param args          The command arguments
 */
void SpawnNpcCommand::execute(Character& character, const std::vector<std::string>& args)
{
    if (args.size() != 2)
        return;

    auto& pos   = character.position();
    auto type   = std::stoi(args.at(0));
    auto typeId = std::stoi(args.at(1));

    auto* def   = new NpcDefinition();
    def->type   = type;
    def->typeId = typeId;

    auto& world = character.world();
    auto npc    = std::make_shared<Npc>(*def, world);
    npc->setPosition(pos);

    world.registerNpc(std::move(npc));
}

/**
 * Gets the identifier of the command.
 * @return  The identifier.
 */
const std::string SpawnNpcCommand::identifier() const
{
    return "npc";
}