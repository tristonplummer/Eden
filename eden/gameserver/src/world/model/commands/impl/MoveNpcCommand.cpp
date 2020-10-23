#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/world/model/actor/player/Player.hpp>
#include <shaiya/game/world/model/actor/npc/Npc.hpp>
#include <shaiya/game/world/model/commands/impl/MoveNpcCommand.hpp>
#include <shaiya/game/world/model/map/Map.hpp>

#include <glog/logging.h>

#include <memory>

using namespace shaiya::game;

/**
 * Handles the execution of this command.
 * @param character     The character that executed this command.
 * @param args          The command arguments
 */
void MoveNpcCommand::execute(Player& character, const std::vector<std::string>& args)
{
    auto id          = std::stoi(args.at(0));
    auto operation   = args.at(1);
    auto destination = args.at(2);

    if (operation != "to")
        return;

    auto& pos   = character.position();
    auto map    = character.map();
    auto entity = map->get(pos, id, EntityType::Npc);

    if (!entity)
        return;

    auto npc = std::dynamic_pointer_cast<Npc>(entity);

    if (destination == "me")
    {
        npc->setPosition(pos);
    }
    else
    {
        auto x = std::stof(args.at(2));
        auto z = std::stof(args.at(3));

        auto& entityPos = npc->position();
        npc->setPosition(Position(entityPos.map(), x, entityPos.y(), z));
    }
}

/**
 * Gets the identifier of the command.
 * @return  The identifier.
 */
const std::string MoveNpcCommand::identifier() const
{
    return "movenpc";
}