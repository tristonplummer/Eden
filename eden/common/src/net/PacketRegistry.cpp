#include <shaiya/common/net/packet/PacketRegistry.hpp>

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