#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/CharacterEnteredViewport.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>

using namespace shaiya::net;

/**
 * Handles an incoming loaded viewport signal.
 * @param session   The session instance.
 * @param request   The inbound loaded viewport signal.
 */
void handleViewportLoaded(Session& session, const ViewportLoadedNotification& request)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto player = game.player();

    player->activate();
}

/**
 * A template specialization used for registering a viewport loaded handler.
 */
template<>
void PacketRegistry::registerPacketHandler<CharacterEnteredViewportOpcode>()
{
    registerHandler<CharacterEnteredViewportOpcode, ViewportLoadedNotification>(&handleViewportLoaded);
}