#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/game/net/GameSession.hpp>

using namespace shaiya::net;
using namespace shaiya::game;

/**
 * Handles an incoming normal chat message.
 * @param session   The session instance.
 * @param request   The inbound normal chat message.
 */
void handleNormalChat(Session& session, const CharacterChatMessage& request)
{
    auto& game     = dynamic_cast<GameSession&>(session);
    auto character = game.character();

    // The chat message
    auto message = request.message.str();

    // Flag the character for a chat update
    character->setAttribute(Attribute::LastChatMessage, message);
    character->flagUpdate(UpdateMask::Chat);
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