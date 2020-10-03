#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/game/net/GameSession.hpp>

using namespace shaiya;
using namespace shaiya::net;

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
    response.charId = request.charId;
    game.write(response);

    // The character instance
    auto character = std::make_shared<shaiya::game::Character>(game, request.charId);
    game.setCharacter(character);

    // Register the character
    world.registerCharacter(game.character());
}

/**
 * A template specialization used for registering a character selection request handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterSelectOpcode>()
{
    registerHandler<CharacterSelectOpcode, CharacterSelectionRequest>(&handleCharacterSelect, ExecutionType::Asynchronous);
}