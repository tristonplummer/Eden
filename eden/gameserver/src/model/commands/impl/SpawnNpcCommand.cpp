#include <shaiya/game/model/actor/npc/Npc.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/commands/impl/SpawnNpcCommand.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

#include <glog/logging.h>

#include <memory>

using namespace shaiya::game;

/**
 * The height of the player character.
 */
constexpr auto PlayerHeightOffset = 0.88f;

/**
 * Handles the execution of this command.
 * @param character     The character that executed this command.
 * @param args          The command arguments
 */
void SpawnNpcCommand::execute(Player& character, const std::vector<std::string>& args)
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
    npc->setPosition(pos.translate(0, -PlayerHeightOffset, 0));

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