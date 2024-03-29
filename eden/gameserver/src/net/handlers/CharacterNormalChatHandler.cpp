#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/CharacterChatMessage.hpp>
#include <shaiya/game/net/GameSession.hpp>
#include <shaiya/game/service/GameWorldService.hpp>
#include <shaiya/game/model/actor/player/Player.hpp>

using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming normal chat message.
 * @param session   The session instance.
 * @param request   The inbound normal chat message.
 */
void handleNormalChat(Session& session, const CharacterChatMessage& request)
{
    auto& game  = dynamic_cast<GameSession&>(session);
    auto player = game.player();

    // The chat message
    auto message = request.message.str();

    // Attempt to execute a command.
    if (message.starts_with('/'))
    {
        auto& commands = player->world().commands();
        commands.execute(*player, message);
        return;
    }

    // Flag the character for a chat update
    player->setAttribute(Attribute::LastChatMessage, message);
    player->flagUpdate(UpdateFlag::Chat);
}

/**
 * A template specialization used for registering a normal chat message handler.
 */
template<>
void PacketRegistry::registerPacketHandler<PlayerChatMessageOpcode>()
{
    registerHandler<PlayerChatMessageOpcode, CharacterChatMessage>(&handleNormalChat);
}

/**
 * A template specialization used for registering a normal chat message handler.
 */
template<>
void PacketRegistry::registerPacketHandler<AdminChatMessageOpcode>()
{
    registerHandler<AdminChatMessageOpcode, CharacterChatMessage>(&handleNormalChat);
}