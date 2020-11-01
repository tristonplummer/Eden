#include <shaiya/game/model/actor/player/Player.hpp>
#include <shaiya/game/model/commands/impl/HeightmapCommand.hpp>
#include <shaiya/game/model/map/Map.hpp>
#include <shaiya/game/service/GameWorldService.hpp>

#include <memory>

using namespace shaiya::game;

/**
 * Handles the execution of this command.
 * @param character     The character that executed this command.
 * @param args          The command arguments
 */
void HeightmapCommand::execute(Player& character, const std::vector<std::string>& args)
{
    auto& heightmap = character.map()->heightmap();
    auto& pos       = character.position();

    LOG(INFO) << "X: " << pos.x() << ", Z: " << pos.z() << ", Floor Y: " << heightmap.y(pos.x(), pos.z());
    std::exit(0);
}

/**
 * Gets the identifier of the command.
 * @return  The identifier.
 */
const std::string HeightmapCommand::identifier() const
{
    return "heightmap";
}