#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/XorEncryptionSignal.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/world/model/actor/character/Character.hpp>

using namespace shaiya::net;

/**
 * Handles an incoming XOR encryption signal.
 * @param session   The session instance.
 * @param request   The inbound XOR encryption signal.
 */
void handleXorSignal(Session& session, const XorEncryptionSignal& request)
{
    auto& game     = dynamic_cast<GameSession&>(session);
    auto character = game.character();

    // Send the character their action bar
    character->actionBar().sync();
    game.initXorEncryption();
}

/**
 * A template specialization used for registering a XOR encryption signal handler.
 */
template<>
void PacketRegistry::registerPacketHandler<XorEncryptionSignalOpcode>()
{
    registerHandler<XorEncryptionSignalOpcode, XorEncryptionSignal>(&handleXorSignal);
}