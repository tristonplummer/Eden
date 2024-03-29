#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/CharacterMovement.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/service/ServiceContext.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>

using namespace shaiya;
using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming character movement request.
 * @param session   The session instance.
 * @param request   The inbound character movement request.
 */
void handleCharacterMovement(Session& session, const CharacterMovement& movement)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto& world = game.context().getGameWorld();
    auto player = game.player();

    // Update the motion and direction
    player->setDirection(movement.direction);
    player->setMotion(movement.motion);

    // Update the character's position
    auto map = player->position().map();
    player->setPosition(Position(map, movement.x, movement.y, movement.z));
}

/**
 * A template specialization used for registering a character movement handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterMovementOpcode>()
{
    registerHandler<CharacterMovementOpcode, CharacterMovement>(&handleCharacterMovement);
}