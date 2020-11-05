#include <shaiya/common/net/packet/PacketRegistry.hpp>
#include <shaiya/common/net/packet/game/AccountFaction.hpp>
#include <shaiya/common/net/packet/game/CharacterChatMessage.hpp>
#include <shaiya/common/net/packet/game/CharacterCreation.hpp>
#include <shaiya/common/net/packet/game/CharacterEnteredViewport.hpp>
#include <shaiya/common/net/packet/game/CharacterMoveItem.hpp>
#include <shaiya/common/net/packet/game/CharacterMovement.hpp>
#include <shaiya/common/net/packet/game/CharacterMovementState.hpp>
#include <shaiya/common/net/packet/game/CharacterPickupItem.hpp>
#include <shaiya/common/net/packet/game/CharacterRemoveItem.hpp>
#include <shaiya/common/net/packet/game/CharacterSelection.hpp>
#include <shaiya/common/net/packet/game/CharacterStatAllocation.hpp>
#include <shaiya/common/net/packet/game/CharacterTradeConfirm.hpp>
#include <shaiya/common/net/packet/game/CharacterTradeGold.hpp>
#include <shaiya/common/net/packet/game/CharacterTradeItem.hpp>
#include <shaiya/common/net/packet/game/GameHandshake.hpp>
#include <shaiya/common/net/packet/game/MobState.hpp>
#include <shaiya/common/net/packet/game/TargetSelectMob.hpp>
#include <shaiya/common/net/packet/game/XorEncryptionSignal.hpp>
#include <shaiya/common/net/packet/login/AccountLoginRequest.hpp>
#include <shaiya/common/net/packet/login/LoginHandshake.hpp>
#include <shaiya/common/net/packet/login/WorldSelectPacket.hpp>

#include <boost/format.hpp>
#include <glog/logging.h>

#include <sstream>

using namespace shaiya::net;

/**
 * Initialises the opcodes to handle.
 */
PacketRegistry::PacketRegistry()
{
    // Register the login server packets
    registerPacketHandler<LoginHandshakeOpcode>();
    registerPacketHandler<LoginRequestOpcode>();
    registerPacketHandler<WorldSelectOpcode>();

    // Register the game server packets
    registerPacketHandler<GameHandshakeOpcode>();
    registerPacketHandler<AccountFactionOpcode>();
    registerPacketHandler<CharacterCreateOpcode>();
    registerPacketHandler<CharacterSelectOpcode>();
    registerPacketHandler<CharacterMovementOpcode>();
    registerPacketHandler<CharacterMovementStateOpcode>();
    registerPacketHandler<CharacterEnteredViewportOpcode>();
    registerPacketHandler<CharacterMoveItemOpcode>();
    registerPacketHandler<PlayerChatMessageOpcode>();
    registerPacketHandler<AdminChatMessageOpcode>();
    registerPacketHandler<XorEncryptionSignalOpcode>();
    registerPacketHandler<CharacterStatAllocationOpcode>();
    registerPacketHandler<CharacterPickupItemRequestOpcode>();
    registerPacketHandler<CharacterRemoveItemRequestOpcode>();
    registerPacketHandler<TradeRequestOpcode>();
    registerPacketHandler<TradeResponseOpcode>();
    registerPacketHandler<TradeFinaliseOpcode>();
    registerPacketHandler<TradeConfirmOpcode>();
    registerPacketHandler<TradeAddGoldOpcode>();
    registerPacketHandler<TradeOfferItemOpcode>();
    registerPacketHandler<TradeRemoveItemOpcode>();
    registerPacketHandler<TargetSelectMobOpcode>();
    registerPacketHandler<MobStateOpcode>();
}

/**
 * Attempts to handle an inbound packet.
 * @param session   The session instance.
 * @param opcode    The opcode of the packet.
 * @param length    The length of the packet.
 * @param payload   The raw packet data.
 */
void PacketRegistry::execute(Session& session, uint16_t opcode, uint16_t length, const char* payload)
{
    if (handlers_.count(opcode) == 0)
    {
        return logUnhandled(session, opcode, length, payload);
    }

    auto func = handlers_.at(opcode);
    func(session, length, payload);
}

/**
 * Logs an unhandled packet.
 * @param session   The session that sent the packet.
 * @param opcode    The opcode of the packet.
 * @param length    The length of the packet.
 * @param payload   The payload of the packet.
 */
void PacketRegistry::logUnhandled(Session& session, size_t opcode, size_t length, const char* payload)
{
    std::stringstream payloadStream;
    auto byteFmt   = boost::format("%02X");
    auto opcodeFmt = boost::format("%04X");

    for (size_t i = 2; i < length; i++)  // Loop from the bytes starting after the opcode
    {
        payloadStream << (byteFmt % (payload[i] & 0xFF)) << " ";
    }

    // Trim trailing whitespace
    auto payloadString = payloadStream.str();
    payloadString.erase(
        std::find_if(payloadString.rbegin(), payloadString.rend(), [](int ch) { return !std::isspace(ch); }).base(),
        payloadString.end());

    // Log the unhandled packet
    LOG(INFO) << "Unhandled packet [opcode=" << (opcodeFmt % opcode) << ", length=" << (length - 2) << ", payload=("
              << payloadString << ")] received from session with the ip " << session.remoteAddress();
}