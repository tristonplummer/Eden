#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/CharacterSelection.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/service/ServiceContext.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>

using namespace shaiya;
using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming character selection request.
 * @param session   The session instance.
 * @param request   The inbound character selection request.
 */
void handleCharacterSelect(Session& session, const CharacterSelectionRequest& request)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto& world = game.context().getGameWorld();

    // Send a successful selection
    CharacterSelectionResponse response;
    response.success = true;
    response.charId  = request.charId;
    game.write(response);

    // The character instance
    auto player = std::make_shared<Player>(game, request.charId);
    game.setPlayer(player);

    // Register the character
    world.registerPlayer(game.player());
}

/**
 * A template specialization used for registering a character selection request handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterSelectOpcode>()
{
    registerHandler<CharacterSelectOpcode, CharacterSelectionRequest>(&handleCharacterSelect, ExecutionType::Asynchronous);
}