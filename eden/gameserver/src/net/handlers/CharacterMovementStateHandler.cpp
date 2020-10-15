#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/service/ServiceContext.hpp>
#include <shaiya/game/world/model/actor/character/Character.hpp>

using namespace shaiya;
using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming character movement state update.
 * @param session   The session instance.
 * @param request   The inbound character movement state update..
 */
void handleMovementState(Session& session, const MovementStateUpdate& movement)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto& world = game.context().getGameWorld();

    // Get the character instance
    auto character = game.character();
    if (!character)
        return;

    // Movement state value can't be higher than a backflip
    if (movement.state > MovementState::Backflip)
        return;

    // Update the character's movement state
    character->setMovementState(movement.state);
}

/**
 * A template specialization used for registering a character movement handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterMovementStateOpcode>()
{
    registerHandler<CharacterMovementStateOpcode, MovementStateUpdate>(&handleMovementState);
}