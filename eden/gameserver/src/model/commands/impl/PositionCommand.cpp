#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/commands/impl/PositionCommand.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/model/map/MapCell.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

#include <boost/format.hpp>

using namespace shaiya::game;

/**
 * Handles the execution of this command.
 * @param player    The character that executed this command.
 * @param args      The command arguments
 */
void PositionCommand::execute(Player& player, const std::vector<std::string>& args)
{
    auto map         = player.map();
    auto& heightmap  = map->heightmap();
    auto& pos        = player.position();
    auto cell        = map->getCell(pos);
    auto* cellPtr    = cell ? cell.get() : nullptr;
    auto entityCount = cell->entities().size();
    auto playerCount = cell->players().size();

    boost::format format("[map=%1%, x=%2%, y=%3%, floorY=%4%, z=%5%, cell=%6%, entities=%7%, players=%8%]");
    format % map->id() % pos.x() % pos.y() % heightmap.y(pos.x(), pos.z()) % pos.z() % cellPtr % entityCount % playerCount;

    player.notice(format.str());
}

/**
 * Gets the identifier of the command.
 * @return  The identifier.
 */
const std::string PositionCommand::identifier() const
{
    return "pos";
}