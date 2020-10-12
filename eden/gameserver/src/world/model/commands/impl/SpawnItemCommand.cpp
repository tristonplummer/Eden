#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/world/model/actor/character/Character.hpp>
#include <shaiya/game/world/model/commands/impl/SpawnItemCommand.hpp>

#include <glog/logging.h>

#include <memory>

using namespace shaiya::game;

/**
 * Handles the execution of this command.
 * @param character     The character that executed this command.
 * @param args          The command arguments
 */
void SpawnItemCommand::execute(Character& character, const std::vector<std::string>& args)
{
    if (args.size() != 2)
        return;

    // The item definition service
    auto& items = character.world().items();

    // The type and type id
    auto type   = std::stoi(args.at(0));
    auto typeId = std::stoi(args.at(1));

    // Attempt to add the item to the player's inventory
    auto def = items.forId(type, typeId);
    if (!def)
        return;

    character.inventory().add(std::make_shared<Item>(*def));
}

/**
 * Gets the identifier of the command.
 * @return  The identifier.
 */
const std::string SpawnItemCommand::identifier()
{
    return "item";
}