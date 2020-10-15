#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/CharacterMovement.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/service/ServiceContext.hpp>
#include <shaiya/game/world/model/actor/character/Character.hpp>

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

    // Get the character instance
    auto character = game.character();
    if (!character)
        return;

    // Update the motion and direction
    character->setDirection(movement.direction);
    character->setMotion(movement.motion);

    // Update the character's position
    auto map = character->position().map();
    character->setPosition(Position(map, movement.x, movement.y, movement.z));
}

/**
 * A template specialization used for registering a character movement handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterMovementOpcode>()
{
    registerHandler<CharacterMovementOpcode, CharacterMovement>(&handleCharacterMovement);
}